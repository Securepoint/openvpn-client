#include "srvcli.h"
#include "servicelogdata.h"

#include <widgets/mainview/MainView.h>
#include <widgetfactory.h>
#include <FrmMain.h>

SrvCLI *SrvCLI::mInst = NULL;

SrvCLI *SrvCLI::instance() {
    if (!mInst)
        mInst = new SrvCLI ();
    return mInst;
}

SrvCLI::SrvCLI()
    : connectionIsOnline (false),
    blockSize (0)
{
    QObject::connect (&this->sslSocket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
    QObject::connect (&this->sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    QObject::connect (&this->sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slot_sslErrors(QList<QSslError>)));
    QObject::connect (&this->sslSocket, SIGNAL(peerVerifyError(QSslError)), this, SLOT(slot_peerVerifyError(QSslError)));
    QObject::connect (&this->sslSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));

    QObject::connect(&this->sslSocket, SIGNAL(disconnected()), SLOT(slotConnectionClosed()));
    QObject::connect(&this->sslSocket, SIGNAL(readyRead()), SLOT(slotStartRead()));
    QObject::connect(&this->sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));
}

bool SrvCLI::send(const QString &command, const QString &params)
{
    if (!this->connectionIsOnline){
        if (!this->makeFastConnection()) {
            // Keine Verbindung
            return false;
        }
    }

    this->sendRequest(command, params);

    return true;
}

bool SrvCLI::sendPass(const QString &command, const QString &params)
{
    if (!this->connectionIsOnline){
        if (!this->makeFastConnection()) {
            // Keine Verbindung
            return false;
        }
    }

    this->sendRequestPass(command, params);

    return true;
}

void SrvCLI::sendRequestPass (const QString &command, const QString &params)
{
    // Neuen Block zum Senden erstellen
    QByteArray block;
    // Datasteam an den Block binden
    QDataStream out (&block, QIODevice::WriteOnly);
    // DataStream version setzen, hier aktuelle 4.6 = DataStream version 10
    out.setVersion(QDataStream::Qt_4_6);
    // Größe des Blockes erstmal mit 0 initieren und Aktion angeben
    out << quint64(0);
    out << command;
    out << params;

    // Wieder an die erste Stelle des Blockes springen und die Größe neu setzen
    out.device()->seek(0);
    out << quint64(block.size() - sizeof(quint64));
    this->sslSocket.write(block);
    this->sslSocket.flush();

    // Reset Stream and ByteArray
    out << quint64(0);
    out.device()->seek(0);
    out.device()->close();
    block.clear();

}

void SrvCLI::sendRequest (const QString &command, const QString &params)
{
    // Neuen Block zum Senden erstellen
    QByteArray block;
    // Datasteam an den Block binden
    QDataStream out (&block, QIODevice::WriteOnly);
    // DataStream version setzen, hier aktuelle 4.6 = DataStream version 10
    out.setVersion(QDataStream::Qt_4_6);
    // Größe des Blockes erstmal mit 0 initieren und Aktion angeben
    out << quint64(0);
    out << command;
    out << params ;

    // Wieder an die erste Stelle des Blockes springen und die Größe neu setzen
    out.device()->seek(0);
    out << quint64(block.size() - sizeof(quint64));
    // Block an das Socket schicken und senden

    this->sslSocket.write(block);
    this->sslSocket.flush();
}


void SrvCLI::slot_peerVerifyError(const QSslError &error)
{
    Q_UNUSED (error)
}

void SrvCLI::slot_sslErrors(const QList<QSslError> &errors)
{

    Q_UNUSED(errors)
    //
    this->sslSocket.ignoreSslErrors();
}

void SrvCLI::slot_stateChanged(QAbstractSocket::SocketState state)
{
    QString stateMessage ("");
    if (state == QAbstractSocket::UnconnectedState) {
        stateMessage = "UnconnectedState: The socket is not connected.";
        this->connectionIsOnline = false;
    } else if (state == QAbstractSocket::HostLookupState) {
        stateMessage = "HostLookupState: The socket is performing a host name lookup.";
    } else if (state == QAbstractSocket::ConnectingState) {
        stateMessage = "ConnectingState: The socket has started establishing a connection.";
    } else if (state == QAbstractSocket::ConnectedState) {
        stateMessage = "ConnectedState: A connection is established.";
    } else if (state == QAbstractSocket::BoundState) {
        stateMessage = "BoundState: The socket is bound to an address and port (for servers).";
    } else if (state == QAbstractSocket::ClosingState) {
        stateMessage = "ClosingState: The socket is about to close (data may still be waiting to be written).";
    } else if (state == QAbstractSocket::ListeningState) {
        stateMessage = "ListeningState: For internal use only.";
    }

    QThread::msleep(100);
}

bool SrvCLI::isOnline() const
{
    return (this->sslSocket.state() == QSslSocket::ConnectedState);
}

void SrvCLI::stopRequest ()
{
    this->response = QString("--stop request");
    this->closeConnection();
}

void SrvCLI::connectionClosedByServer ()
{
    this->closeConnection();
}

void SrvCLI::error ()
{
    this->response = this->sslSocket.errorString();
    this->closeConnection();
}

void SrvCLI::closeConnection()
{
    this->sslSocket.close();
    this->connectionIsOnline = false; // neu zum Testen
}

void SrvCLI::resetConnection()
{
    if (this->connectionIsOnline) {
        this->closeConnection();
    }
}

bool SrvCLI::makeFastConnection()
{
    if (this->sslSocket.state() != QAbstractSocket::UnconnectedState) {
        qDebug() << "socket is not in unconnected state";
        //
        return true;
    }

    //
    this->sslSocket.connectToHost(QLatin1String("127.0.0.1"), 3656);
    //
    if (!this->sslSocket.waitForConnected(1900)) {
        qDebug() << "Connect to service failed";
        //
        return false;
    }

    // All fine, set new state
    this->connectionIsOnline = true;

    return true;
}

void SrvCLI::slotStartRead()
{
    //
    // Read the block until the complete block is avaiable
    //


    QDataStream in(&sslSocket);
    in.setVersion(QDataStream::Qt_4_6);

    if (blockSize == 0) {
        if ((quint32) sslSocket.bytesAvailable() < sizeof(qint64)) {
            return;
        }
        in >> blockSize;
    }

    if (sslSocket.bytesAvailable() < blockSize) {
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
        QStringList fields (params.split(";"));
        if (fields.size() == 2) {
            int cId (fields.at(0).toInt());
            emit receivedDummy(cId);
        }
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
        emit needUserInput(params.toInt(), -1);
    } else if(command == QLatin1String("GETLOG")) {
        QStringList fields (params.split(";"));
        if(fields.size() == 2)
        {
            // Neuen Block zum Senden erstellen
            QByteArray block;
            // Datasteam an den Block binden
            QDataStream out (&block, QIODevice::WriteOnly);
            // DataStream version setzen, hier aktuelle 4.6 = DataStream version 10
            out.setVersion(QDataStream::Qt_4_6);
            // Größe des Blockes erstmal mit 0 initieren und Aktion angeben
            out << quint64(0);
            out << QString("GETLOG_RESP");
            out << (fields.at(0) + ";" + ServiceLogData::instance()->logs(fields.at(1).toInt()).join("\n"));
            // Wieder an die erste Stelle des Blockes springen und die Größe neu setzen
            out.device()->seek(0);
            out << quint64(block.size() - sizeof(quint64));
            // Block an das Socket schicken und senden
            this->sslSocket.write(block);
            this->sslSocket.waitForBytesWritten();
            this->sslSocket.flush();
        }
    } else if(command == QLatin1String("GET_ERROR")) {
        QStringList fields (params.split(";"));
        if(fields.size() == 2)
        {
            // Neuen Block zum Senden erstellen
            QByteArray block;
            // Datasteam an den Block binden
            QDataStream out (&block, QIODevice::WriteOnly);
            // DataStream version setzen, hier aktuelle 4.6 = DataStream version 10
            out.setVersion(QDataStream::Qt_4_6);
            // Größe des Blockes erstmal mit 0 initieren und Aktion angeben
            out << quint64(0);
            out << QString("GET_ERROR_RESP");

            auto pConnection = ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.GetConnection(fields.at(1).toInt());

            QString errorMsg = "";

            if(pConnection)
            {
                if(pConnection->GetState() != ConnectionState::Connected)
                    errorMsg = pConnection->GetError();
                else
                    errorMsg = "";
            }


            out <<  (fields.at(0) + ";" + errorMsg);
            // Wieder an die erste Stelle des Blockes springen und die Größe neu setzen
            out.device()->seek(0);
            out << quint64(block.size() - sizeof(quint64));
            // Block an das Socket schicken und senden
            this->sslSocket.write(block);
            this->sslSocket.waitForBytesWritten();
            this->sslSocket.flush();
        }
    } else if(command == QLatin1String("STOP_CLIENT")) {
        FrmMain::instance()->sendClose();
        return;
    } else {
        // Nichts kein gültiges Kommando
        qDebug() << "Unknown command";
    }

    FrmMain::instance()->refreshUI();

    if(sslSocket.bytesAvailable())
    {
        QCoreApplication::processEvents();

        emit slotStartRead();
    }

}

void SrvCLI::slotConnectionClosed()
{
    //
    // Connection is closed emit signal after we wait a short time
    //
    emit connectionIsClosed();
}

void SrvCLI::slotError(QAbstractSocket::SocketError err)
{
    //
    // Error while conntecting
    //

    Q_UNUSED(err)

    if (sslSocket.state() != QAbstractSocket::ConnectedState){
        slotConnectionClosed();
    }
}
