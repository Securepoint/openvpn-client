#include "sslserverconnection.h"
#include "sslserver.h"
#include <QtNetwork/QSslKey>

#include "debug.h"
#include "openvpn.h"
#include "srvcli.h"
#include "tapdriver.h"

// We need in the next 10000 years no new one ;D
quint64 SslServerConnection::internalId = 0;

// OpenVPN alive list
QList<QPair<int, OpenVpn*> > SslServerConnection::vpnList;

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
    QObject::connect(socket, SIGNAL(connected()), SLOT(slotAcceptedClient()));
    QObject::connect(socket, SIGNAL(disconnected()), SLOT(slotConnectionClosed()));
    QObject::connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    QObject::connect(socket, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(slotModeChanged(QSslSocket::SslMode)));
    QObject::connect(socket, SIGNAL(readyRead()), SLOT(slotStartRead()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));

    // Set the certificate and the key
    if (!SslKeyContent::instance()->keyIsValid()) {
        // Without a valid key, disconnect
        Debug::error(QLatin1String("No valid key"));
        socket->disconnectFromHost();
    } else {
        // Key is valid
        QSslKey key(SslKeyContent::instance()->getKey(), QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey, "server");
        // If key is null disconnect and close the socket
        if (key.isNull()) {
            Debug::error(QLatin1String("Key is null"));
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

    Debug::log(QLatin1String("Mode changed: ") + modeMes);
}

SslServerConnection::~SslServerConnection()
{
    //
    // dtor has noting to do
    //
    Debug::log(QLatin1String("Delete ssl connection object"), DebugLevel::Destruktor);
}

void SslServerConnection::slotAcceptedClient()
{
    // Provide feedback to the user about incoming connections. This
    // slot is only called if the connection was established, so all
    // communication is now encrypted.
    Debug::log(QLatin1String("Connection is now encrypted"));
}

void SslServerConnection::recDig(int id)
{
    Debug::log(QString("receive sig from %1").arg(id));
}

void SslServerConnection::slotStartRead()
{
    //
    // Read the block until the complete block is avaiable
    //

    Debug::log (QLatin1String("Start reading"));

    this->blockSize = 0;
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
    if (command.isEmpty()) {
        // Kein Befehl keine Aktion
        return;
    }

    //
    Debug::log(QLatin1String("Command: ") + command);
    //

    command = command.trimmed().toUpper();
    if (command == QLatin1String("OPEN")) {
        //
        // Öffnet eine Verbindung
        //

        // Hat die Parameterliste die richtige Länge
        QStringList fields (params.split(";"));
        if (fields.size() == 4) {
            // 0: ID; 1: Config Path; 2: Interact
            // Ist der Eintrag schon in der Liste
            int configId (fields.at(0).toInt());
            OpenVpn *item = this->foundItemInList(configId);
            Q_ASSERT(item);

            if (!item) {
                Debug::log(QLatin1String("Open: No valid item. Build new one"));
            } else {
                // Objekt ist schon in der Liste, nicht gut = Fehler
                Debug::error(QLatin1String("Open: Object is already in list. Id: ") + fields.at(0));
                // Wenn es nicht connected oder connting status hat, löschen und ein neues anlegen
                if (item->isConnectionStable() || item->isConnecting()) {
                    Debug::log(QLatin1String("Connecting or Connection is stable"));
                    // Nur den Status an den Client senden
                    item->sendStatus();
                    return;
                }

                // Item ist nicht verbunden oder verbindet sich gerade auch nicht
                // Deswegen wird das Objekt geklöscht und ein neues angelegt
                this->removeItemFromList(item->id());
            }

            Debug::log(QLatin1String("Build new object"));

            // Alls ok, wir bauen us ein neues Objekt
            OpenVpn *sslVpn = new OpenVpn ();
            // Nun die Parameter setzen
            QString configPath = fields.at(1).left(fields.at(1).lastIndexOf("/"));
            QString configName = fields.at(1).right(fields.at(1).size() - fields.at(1).lastIndexOf("/") - 1);
            configName = configName.left(configName.size() - 5);
            sslVpn->setConfigPath(configPath);
            sslVpn->setConfigName(configName);
            sslVpn->setUseInteract(fields.at(2));
            sslVpn->setProxyString(fields.at(3));
            sslVpn->setId(configId);

            QObject::connect(sslVpn, SIGNAL(foobar(int)), this, SLOT(recDig(int)));

            // Noch ein paar Signale binden
            //
            // Das Objekt hat sich zum löschen markiert
            QObject::connect(sslVpn, SIGNAL(removeItemFromList(int)), this, SLOT(removeItemFromList(int)));
            //
            // Nachrichten aus dem Objekt senden
            QObject::connect(sslVpn, SIGNAL(sendMessage(QString,QString)), SrvCLI::instance(), SLOT(send(QString,QString)));

            Debug::log(QLatin1String("Add item to list"));
            // Nun das Objekt an die Liste anfügen
            vpnList.append(qMakePair(configId, sslVpn));
            //
            // Nun das OpenVpn verbinden
            Debug::log(QLatin1String("Start connection"));

            SrvCLI::instance()->send(QString("%2;Try to start OpenVPN connection %1 \n").arg(configName).arg(configId), QLatin1String("LOG"));
            // Ohne Verzögerung den connect aufrufen
            sslVpn->connectToVpn();
            //
        }
    } else if (command == QLatin1String("CLOSE")) {
        //
        // Aktive Verbindung schliessen und die OpenVpn Instanz beenden
        //
        if (params.isEmpty()) {
            Debug::error(QLatin1String("Close: No params."));
            return;
        }

        // Das Objekt suchen und holen
        OpenVpn *item = this->foundItemInList(params.toInt());
        Q_ASSERT(item);
        if (!item) {
            Debug::error(QLatin1String("Close: No valid item."));
            return;
        }

        // Disconnect triggern
        item->disconnectVpn();

    } else if (command == QLatin1String("STATUS")) {

        if (params.isEmpty()) {
            return;
        }

        OpenVpn *item = this->foundItemInList(params.toInt());
        Q_ASSERT(item);

        if (!item) {
            return;
        }

        // Den Status des Objektes senden
        item->sendStatus();

    } else if (command == QLatin1String("UNEEDED")) {

        if (params.isEmpty()) {
            return;
        }

        QStringList fields (params.split(";"));
        if (fields.size() == 2) {
            // 0: ID; 1: Username;
            OpenVpn *item = this->foundItemInList(fields.at(0).toInt());
            Q_ASSERT(item);

            if (!item) {
                Debug::error(QLatin1String("USER NEEDED: No valid item."));
                return;
            }

            item->setUsername(fields.at(1));
        }

    } else if (command == QLatin1String("PKNEEDED")) {
        if (params.isEmpty()) {
            return;
        }

        QStringList fields (params.split(";"));
        if (fields.size() == 2) {
            // 0: ID; 1: Username;
            OpenVpn *item = this->foundItemInList(fields.at(0).toInt());
            Q_ASSERT(item);

            if (!item) {
                Debug::error(QLatin1String("Private NEEDED: No valid item."));
                return;
            }

            item->setPrivateKey(fields.at(1));
        }
    } else if (command == QLatin1String("PWDNEEDED")) {
        if (params.isEmpty()) {
            return;
        }

        QStringList fields (params.split(";"));
        if (fields.size() == 2) {
            // 0: ID; 1: Username;
            OpenVpn *item = this->foundItemInList(fields.at(0).toInt());
            Q_ASSERT(item);

            if (!item) {
                Debug::error(QLatin1String("PASS NEEDED: No valid item."));
                return;
            }

            item->setPassword(fields.at(1));
        }
    } else if (command == QLatin1String("CKNEEDED")) {
        if (params.isEmpty()) {
            return;
        }

        QStringList fields (params.split(";"));
        if (fields.size() == 2) {
            // 0: ID; 1: Username;
            OpenVpn *item = this->foundItemInList(fields.at(0).toInt());
            Q_ASSERT(item);

            if (!item) {
                Debug::error(QLatin1String("OTP NEEDED: No valid item."));
                return;
            }

            item->setChallengeKey(fields.at(1));
        }
    } else if (command == QLatin1String("HTTPUSERNEEDED")) {
        if (params.isEmpty()) {
            return;
        }

        QStringList fields (params.split(";"));
        if (fields.size() == 2) {
            // 0: ID; 1: Username;
            OpenVpn *item = this->foundItemInList(fields.at(0).toInt());
            Q_ASSERT(item);

            if (!item) {
                Debug::error(QLatin1String("HTTP NEEDED: No valid item."));
                return;
            }

            item->setUsername(fields.at(1));
        }
    } else if (command == QLatin1String("HTTPPASSNEEDED")) {
        if (params.isEmpty()) {
            return;
        }

        QStringList fields (params.split(";"));
        if (fields.size() == 2) {
            // 0: ID; 1: Username;
            OpenVpn *item = this->foundItemInList(fields.at(0).toInt());
            Q_ASSERT(item);

            if (!item) {
                Debug::error(QLatin1String("HTTP Pass NEEDED: No valid item."));
                return;
            }

            item->setPassword(fields.at(1));
        }
    } else if (command == QLatin1String("ADDTAP")) {
        //
        // Installiert ein neues Tap Device
        Debug::log(QLatin1String("Add a tap device is triggered."));
        //
        TapDriver tapDriver;
        if (!tapDriver.installTapDriver()) {
            // Tap Device konnte nicht installiert werden
            Debug::error(QLatin1String("Can't install tap device"));
            SrvCLI::instance()->send(QLatin1String("ERROR"), QLatin1String("TAPINSTALL"));
            return;
        }
        Debug::log(QLatin1String("Tap device installed successfully"));
        SrvCLI::instance()->send(QLatin1String("OK"), QLatin1String("TAPINSTALL"));
    } else if (command == QLatin1String("REMOVETAP")) {
        //
        // Installiert ein neues Tap Device
        Debug::log(QLatin1String("Remove all tap devices is triggered."));
        //
        TapDriver tapDriver;
        if (!tapDriver.removeTapDriver()) {
            // Die Tap Devices konnten nicht entfernt werden
            Debug::error(QLatin1String("Can't remove tap devices"));
            SrvCLI::instance()->send(QLatin1String("ERROR"), QLatin1String("TAPREMOVE"));
            return;
        }
        Debug::log(QLatin1String("Tap devices removes successfully"));
        SrvCLI::instance()->send(QLatin1String("OK"), QLatin1String("TAPREMOVE"));
    } else if (command == QLatin1String("DUMMY")) {
        //
        // Test, für ist der service vorhanden
        Debug::log(QLatin1String("Test command is triggered."));
        SrvCLI::instance()->send(QLatin1String("OK"), QLatin1String("DUMMY"));
    } else {
        // unknown command error
        Debug::error(QLatin1String("Unknown command: ") + command + QLatin1String(" - P: ") + params);
    }
}

OpenVpn *SslServerConnection::foundItemInList(const int &id)
{
    //
    // Search an id on the openvpn list
    // and return on success a pointer to the object
    //
    OpenVpn *ret (0);

    Debug::log(QString("Find item -> list size %1").arg(vpnList.size()));

    // Liste durchlaufen
    QListIterator<QPair<int, OpenVpn*> > i(vpnList);
    while (i.hasNext()) {
        // Wenn die Id stimmt, das Objekt zurückgeben
        QPair <int, OpenVpn*> item = i.next();
        //
        Debug::log(QString ("List item -> Id: %1").arg(item.first));
        if (item.first == id) {
            // Item gefunden, das OpenVpn-Objekt zurückgeben
            ret = item.second;
            // Raus hier
            break;
        }
    }

    Debug::log(QString("Find item end -> list size %1").arg(vpnList.size()));

    return ret;
}

void SslServerConnection::removeItemFromList(int id)
{
    //
    // Search an OpenVPN object with the given id
    // delete and remove it from the list
    //
    Debug::log(QString("Remove item  %2 -> list size %1").arg(vpnList.size()).arg(id));
    // Liste durchlaufen
    QMutableListIterator<QPair<int, OpenVpn*> > i(vpnList);
    while (i.hasNext()) {
        // Wenn die Id stimmt, das Objekt zurückgeben
        QPair <int, OpenVpn*> item = i.next();
        //
        Debug::log(QString ("Rm list item -> Id: %1 - %2").arg(item.first).arg(id));
        if (item.first == id) {
            Debug::log(QString ("Delete list item -> Id: %1").arg(item.first));
            OpenVpn *ssl = item.second;
            // Alle Signale diconnecten
            QObject::disconnect(ssl, 0, 0, 0);
            // Aus der Liste entfernen
            i.remove();
            // Löschen
            QTimer::singleShot(250, ssl, SLOT(deleteLater()));
            // Raus hier
            break;
        }
    }
    Debug::log(QString("Remove item end -> list size %1").arg(vpnList.size()));
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
