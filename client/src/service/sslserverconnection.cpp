#include "sslserverconnection.h"
#include "sslserver.h"
#include <QtNetwork/QSslKey>
#include <FrmMain.h>
#include "servicelogdata.h"

#include <widgets/mainview/MainView.h>
#include <widgetfactory.h>


// We need in the next 10000 year no new one ;D
quint64 SslServerConnection::internalId = 0;

SslServerConnection::SslServerConnection(QObject *parent)
    : QThread(parent),
      blockSize (0)
{
    qDebug() << __FUNCSIG__;

    // Set the new internal id
    internalId++;
    // Init the ssl socket
    this->socket = new QSslSocket(this);
    this->socket->setProtocol(QSsl::AnyProtocol);

    // Now bind some signal of the ssl socket
    QObject::connect(socket, SIGNAL(connected()), SLOT(slotAcceptedClient()));
    QObject::connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    QObject::connect(socket, SIGNAL(disconnected()), SLOT(slotConnectionClosed()));
    QObject::connect(socket, SIGNAL(readyRead()), SLOT(slotStartRead()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));
}

SslServerConnection::SslServerConnection(quint16 socketDescriptor, QObject *parent)
    : QThread(parent),
      blockSize (0)
{
    // Set the new internal id
    internalId++;
    // Init the ssl socket
    this->socket = new QSslSocket(this);
    this->socket->setProtocol(QSsl::AnyProtocol);
    this->socket->setSocketDescriptor(socketDescriptor);

    // Now bind some signal of the ssl socket
    QObject::connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    QObject::connect(socket, SIGNAL(disconnected()), SLOT(slotConnectionClosed()));
    QObject::connect(socket, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(slotModeChanged(QSslSocket::SslMode)));
    QObject::connect(socket, SIGNAL(readyRead()), SLOT(slotStartRead()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));
}

SslServerConnection::~SslServerConnection()
{
}

void SslServerConnection::run()
{
    //
    // When the thread is started, we only need to start the eventloop.
    // All other control is now via signal and slots
    //
    this->exec();
}

#include <Windows.h>

void SslServerConnection::slotStartRead()
{
    //
    // Read the block until the complete block is avaiable
    //

    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_6);

    if (blockSize == 0) {
        if ((quint32) socket->bytesAvailable() < sizeof(qint64)) {
            return;
        }

        in >> blockSize;
    }

    if (socket->bytesAvailable() < blockSize) {
        return;
    }

    QString command("");
    QString params("");


    in >> command;
    in >> params;

    //
    // Ab hier kann man munter drauf los mit den Daten ;)
    //
    // Den Befehl auswerten
    this->blockSize = 0;
    if (command.isEmpty()) {
        // Kein Befehl keine Aktion
        return;
    }

    // Ein wenig aufräumen, ist eigentlich überflüssig
    command = command.trimmed().toUpper();

    // Hier die Antworten des Servers auswerten
    // Erstmal die Logdaten
    if (command == QLatin1String("LOG")) {
        // Aufbau param: id;message
        // Zu welcher Verbindung gehört die Meldung
        QStringList fields (params.split(";"));
        if (fields.size() > 1) {
            // Genug Felder da
            QString cId (fields.at(0));
            // Die ID entfernen
            fields.removeAt(0);
            // Die restlichen Elemente wiede mit ; zusammenfügen
            // Da diese auch im Original mit ; getrennt waren
            QString cMessage (fields.join(";"));
            // Nun an die Liste anhängen
            if(cMessage.at(cMessage.length()-1) != '\n')
                cMessage += "\n";
            ServiceLogData::instance()->append(cId.toInt(), cMessage);
        }
    } else if (command == QLatin1String ("DISCONNECTED")) {
        // Aufbau Param: ID
        // Den Button neu setzen
        emit setDisconnected(params.toInt());
        // Meldung ins Log schreiben
        ServiceLogData::instance()->append(params.toInt(), QLatin1String("Disconnected\n"));
    } else if (command == QLatin1String("ERROR")) {
        // Aufbau Param: id;message
        QStringList fields (params.split(";"));
        if (fields.size() > 1) {
            // Genug Felder da
            QString cId (fields.at(0));
            // Die ID entfernen
            fields.removeAt(0);
            // Die restlichen Elemente wiede mit ; zusammenfügen
            // Da diese auch im Original mit ; getrennt waren
            QString cMessage (fields.join(";"));
            // Nun an die Liste anhängen
            if(cMessage.at(cMessage.length()-1) != '\n')
                cMessage += "\n";

            ServiceLogData::instance()->append(cId.toInt(), QLatin1String("ERROR: ") + cMessage);
            // Den Button setzen
            emit setError(cId.toInt(), cMessage);
        }
    } else if (command == QLatin1String("INPUT")) {
        // Eine Benutzeraktion wird erwartet
        // Aufbau Param: id;type
        QStringList fields (params.split(";"));
        if (fields.size() == 2) {
            // Genug Felder da
            QString cId (fields.at(0));
            QString cType (fields.at(1));
            emit needUserInput(cId.toInt(), cType.toInt());
        }
    } else if (command == QLatin1String("STATUS")) {
        // Verbindungstatus wurde gesendet
        // Aufbau Param: id;connected[0,1];connecting[0,1]
        QStringList fields (params.split(";"));
        if (fields.size() == 5) {
            // Genug Felder da
            int cId (fields.at(0).toInt());
            bool cConnected ((fields.at(1) == QLatin1String("1") ? true : false));
            bool cConnecting ((fields.at(2) == QLatin1String("1") ? true : false));
            int cLastAction (fields.at(3).toInt());
            QString cIp (fields.at(4));

            emit receivedStatus(cId, cConnected, cConnecting, cLastAction, cIp);

        }
    } else if (command == QLatin1String("DUMMY")) {
        // Test Kommando für die Service abfrage
        emit receivedDummy();
    } else if (command == QLatin1String("RESTART")) {
        // Die Verbindung macht einen Reconnect
        emit receivedReconnect(params.toInt());
    } else if (command == QLatin1String("TAPINSTALL")) {
        // Tap-Device installation
        params = params.trimmed().toUpper();
        if (params == QLatin1String ("NEEDED")) {
            // Ein wird ein Tap Device gebraucht
            emit receivedTapControl(1);
        } else  if (params == QLatin1String ("OK")) {
            // Installation ist ok
            emit receivedTapControl(0);
        } else  if (params == QLatin1String ("ERROR")) {
            // Installation ist fehlgeschlagen
            emit receivedTapControl(-1);
        }
    } else if (command == QLatin1String("TAPREMOVE")) {
        // Tap-Device Remove
        params = params.trimmed().toUpper();
        emit receivedRemoveTap(params);

    } else if(command == "TAPCOUNT")
    {
        int count = params.toInt();

        emit receivedTapCount(count);

    } else if (command == QLatin1String("RECEIVEDIP")) {
        // Ip ist da, die Verbindung steht nun
        // Aufbau Param: id;ip
        QStringList fields (params.split(";"));
        if (fields.size() == 2) {
            // Genug Felder da
            QString cId (fields.at(0));
            QString cIP (fields.at(1));
            emit receivedIP(cId.toInt(), cIP);
        }
    } else if(command == QLatin1String("REMOVE_USER_DATA")) {
         qDebug() << "Unknown command";
        emit needUserInput(params.toInt(), -1);
    } else if(command == QLatin1String("GETLOG")) {
        // Neuen Block zum Senden erstellen
        QByteArray block;
        // Datasteam an den Block binden
        QDataStream out (&block, QIODevice::WriteOnly);
        // DataStream version setzen, hier aktuelle 4.6 = DataStream version 10
        out.setVersion(QDataStream::Qt_4_6);
        // Größe des Blockes erstmal mit 0 initieren und Aktion angeben
        out << quint64(0);
        out << QString("VPN_LOG");
        out << ServiceLogData::instance()->logs(params.toInt()).join("\n");
        // Wieder an die erste Stelle des Blockes springen und die Größe neu setzen
        out.device()->seek(0);
        out << quint64(block.size() - sizeof(quint64));
        // Block an das Socket schicken und senden
        this->socket->write(block);
        this->socket->waitForBytesWritten();
        this->socket->flush();

        return;
    } else if(command == QLatin1String("GET_ERROR")) {
        // Neuen Block zum Senden erstellen
        QByteArray block;
        // Datasteam an den Block binden
        QDataStream out (&block, QIODevice::WriteOnly);
        // DataStream version setzen, hier aktuelle 4.6 = DataStream version 10
        out.setVersion(QDataStream::Qt_4_6);
        // Größe des Blockes erstmal mit 0 initieren und Aktion angeben
        out << quint64(0);
        out << QString("VPN_ERROR");

        auto pConnection = ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.GetConnection(params.toInt());

        QString errorMsg = "";

        if(pConnection)
        {
            errorMsg = pConnection->GetError();
        }

        out <<  errorMsg;
        // Wieder an die erste Stelle des Blockes springen und die Größe neu setzen
        out.device()->seek(0);
        out << quint64(block.size() - sizeof(quint64));
        // Block an das Socket schicken und senden
        this->socket->write(block);
        this->socket->waitForBytesWritten();
        this->socket->flush();

        return;
    } else if(command == QLatin1String("STOP_CLIENT")) {
        MessageBoxA(NULL, "", "", 0);
        FrmMain::instance()->sendClose();
        return;
    } else {
        // Nichts kein gültiges Kommando
        qDebug() << "Unknown command";
    }

    FrmMain::instance()->refreshUI();

    if(socket->bytesAvailable())
    {
        QCoreApplication::processEvents();

        emit slotStartRead();
    }

}

void SslServerConnection::slotConnectionClosed()
{
    //
    // Connection is closed emit signal after we wait a short time
    //
    emit connectionIsClosed();
}

void SslServerConnection::slotError(QAbstractSocket::SocketError err)
{
    //
    // Error while conntecting
    //

    Q_UNUSED(err);

    if (socket->state() != QAbstractSocket::ConnectedState) {
        slotConnectionClosed();
    }
}

const quint64 SslServerConnection::threadId() const
{
    //
    // Return the internal id of this thread
    //
    return internalId;
}
