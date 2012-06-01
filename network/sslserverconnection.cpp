#include "sslserverconnection.h"
#include "sslserver.h"
#include <QtNetwork/QSslKey>

#include "../debug.h"
#include "../servicelogdata.h"

// We need in the next 10000 year no new one ;D
quint64 SslServerConnection::internalId = 0;

SslServerConnection::SslServerConnection(quint16 socketDescriptor, QMutex *mut, QObject *parent)
    : QThread(parent),
      blockSize (0),
      mutex (mut)
{
    // Set the new internal id
    internalId++;
    // Init the ssl socket
    this->socket = new QSslSocket(this);
    this->socket->setProtocol(QSsl::AnyProtocol);
    this->socket->setSocketDescriptor(socketDescriptor);

    // Now bind some signal of the ssl socket
    QObject::connect(socket, SIGNAL(connected()), SLOT(slotAcceptedClient()));
    QObject::connect(socket, SIGNAL(disconnected()), SLOT(slotConnectionClosed()));
    QObject::connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    QObject::connect(socket, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(slotModeChanged(QSslSocket::SslMode)));
    QObject::connect(socket, SIGNAL(readyRead()), SLOT(slotStartRead()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));

    // Set the certificate and the key
    if (!SslKeyContent::instance()->keyIsValid()) {
        // Without a valid key, disconnect
        socket->disconnectFromHost();
    } else {
        // Key is valid
        QSslKey key(SslKeyContent::instance()->getKey(), QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "server");
        // If key is null disconnect and close the socket
        if (key.isNull()) {
            socket->disconnectFromHost();
            // Close the socket
            socket->close();
        } else {
            if (socket->addCaCertificates(QCoreApplication::applicationDirPath() + QLatin1Char('/') + CACERTIFICATES_FILE)) {
                socket->setLocalCertificate(QCoreApplication::applicationDirPath() + QLatin1Char('/') + LOCALCERTIFICATE_FILE);
                socket->setPrivateKey(key);
                socket->startServerEncryption();
            }
        }
    }   
}

void SslServerConnection::run()
{
    //
    // When the thread is started, we only need to start the eventloop.
    // All other control is now via signal and slots
    //
    this->exec();
}


void SslServerConnection::slotModeChanged (QSslSocket::SslMode mode)
{
    //
    // The socket change the mode - Only enable with debuging
    //
    return;

    // Erstmal DC
    QString modeMes ("");
    if (mode == QSslSocket::UnencryptedMode) {
        modeMes = QLatin1String("The socket is unencrypted.");
    } else if (mode == QSslSocket::SslClientMode) {
        modeMes = QLatin1String("The socket is a client-side SSL socket.");
    } else if (mode == QSslSocket::SslServerMode) {
        modeMes = QLatin1String("The socket is a server-side SSL socket.");
    }
}

SslServerConnection::~SslServerConnection()
{
    //
    // dtor has noting to do
    //
}

void SslServerConnection::slotAcceptedClient()
{
    // Provide feedback to the user about incoming connections. This
    // slot is only called if the connection was established, so all
    // communication is now encrypted.
}

void SslServerConnection::slotStartRead()
{
    //
    // Read the block until the complete block is avaiable
    //

    Debug::log (QLatin1String("Start reading"));    

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

    in >> params;
    in >> command;    


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
            ServiceLogData::instance()->append(cId.toInt(), cMessage);
        }
    } else if (command == QLatin1String ("DISCONNECTED")) {
        // Aufbau Param: ID
        // Den Button neu setzen
        emit setDisconnected(params.toInt());
        // Meldung ins Log schreiben
        ServiceLogData::instance()->append(params.toInt(), QLatin1String("Disconnected"));
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
    } else {
        // Nichts kein gültiges Kommando
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
    if (socket->state() != QAbstractSocket::ConnectedState){
        if (socket->state() == QAbstractSocket::UnconnectedState) {
            slotConnectionClosed();
        } else {
            slotConnectionClosed();
        }
    }
}

const quint64 SslServerConnection::threadId() const
{
    //
    // Return the internal id of this thread
    //
    return internalId;
}
