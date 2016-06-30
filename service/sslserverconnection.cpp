#include "sslserverconnection.h"
#include "sslserver.h"
#include <QtNetwork/QSslKey>

#include "debug.h"
#include "openvpn.h"
#include "srvcli.h"
#include "tapdriver.h"
#include "service.h"

// We need in the next 10000 years no new one ;D
quint64 SslServerConnection::internalId = 0;

SslServerConnection::SslServerConnection(quint16 socketDescriptor, QObject *parent)
    : QThread (parent),
      blockSize (0),
      id (0)
{
    // Set the new internal id
    _threadId = internalId++;
    // Init the ssl socket
    this->socket = new QSslSocket(this);
    this->socket->setProtocol(QSsl::AnyProtocol);
    this->socket->setSocketDescriptor(socketDescriptor);

    _srvCLI = new SrvCLI(socket);

    // Now bind some signal of the ssl socket
    QObject::connect(socket, SIGNAL(connected()), SLOT(slotAcceptedClient()));
    QObject::connect(socket, SIGNAL(disconnected()), SLOT(slotConnectionClosed()));
    QObject::connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    QObject::connect(socket, SIGNAL(readyRead()), SLOT(slotStartRead()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));
}

void SslServerConnection::run()
{
    //
    // When the thread is started, we only need to start the eventloop.
    // All other control is now via signal and slots
    //
    this->exec();
}

void SslServerConnection::setId(quint32 id)
{
    this->id = id;
}

quint32 SslServerConnection::getId()
{
    return this->id;
}

void SslServerConnection::setName(QString name)
{
    this->name = name;
}

QString SslServerConnection::getName()
{
    return this->name;
}

SslServerConnection::~SslServerConnection()
{
    Debug::log(QLatin1String("Delete ssl connection object"), DebugLevel::Destruktor);

    QListIterator<QPair<int, OpenVpn*> > i(vpnList);
    while (i.hasNext()) {
        QPair <int, OpenVpn*> item = i.next();
        item.second->disconnectVpn(true);
        item.second->deleteLater();
    }


    vpnList.clear();

    delete _srvCLI;
    _srvCLI = nullptr;
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

     Debug::log (QLatin1String("Start reading"));
     this->blockSize = 0;


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
        goto check_available;
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
           // Q_ASSERT(item);

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
                    goto check_available;
                }

                // Item ist nicht verbunden oder verbindet sich gerade auch nicht
                // Deswegen wird das Objekt geklöscht und ein neues angelegt
                this->removeItemFromList(item->id());
            }

            Debug::log(QLatin1String("Build new object"));

            // Alls ok, wir bauen us ein neues Objekt
            OpenVpn *sslVpn = new OpenVpn (_srvCLI);
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
            QObject::connect(sslVpn, SIGNAL(sendMessage(QString,QString)), _srvCLI, SLOT(send(QString,QString)));

            Debug::log(QLatin1String("Add item to list"));
            // Nun das Objekt an die Liste anfügen
            vpnList.append(qMakePair(configId, sslVpn));
            //
            // Nun das OpenVpn verbinden
            Debug::log(QLatin1String("Start connection"));

            _srvCLI->send(QString("%2;Try to start OpenVPN connection %1 %3\n").arg(configName).arg(configId).arg(configPath), QLatin1String("LOG"));
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
            goto check_available;
        }

        // Das Objekt suchen und holen
        OpenVpn *item = this->foundItemInList(params.toInt());
        Q_ASSERT(item);
        if (!item) {
            Debug::error(QLatin1String("Close: No valid item."));
             goto check_available;
        }

        // Disconnect triggern
        item->disconnectVpn();

    } else if (command == QLatin1String("STATUS")) {

        if (params.isEmpty()) {
             goto check_available;
        }

        OpenVpn *item = this->foundItemInList(params.toInt());
        Q_ASSERT(item);

        if (!item) {
            Debug::log(QLatin1String("Send status"));
            _srvCLI->send(QString("%1;%2;%3;%4;%5")
                .arg(params.toInt())
                .arg(QLatin1String("0"))
                .arg(QLatin1String("0"))
                .arg(-1)
                .arg(""), QLatin1String("STATUS"));
             goto check_available;
        }

        // Den Status des Objektes senden
        item->sendStatus();

    } else if (command == QLatin1String("UNEEDED")) {

        if (params.isEmpty()) {
            goto check_available;
        }

        auto index = params.indexOf(";");

        if(index != -1)
        {
            OpenVpn *item = this->foundItemInList(params.left(index).toInt());

            if (!item) {
                Debug::error(QLatin1String("USER NEEDED: No valid item."));
                goto check_available;
            }
            item->setUsername(params.right(params.length() - index - 1));
        }
    } else if (command == QLatin1String("PKNEEDED")) {
        if (params.isEmpty()) {
             goto check_available;
        }

        auto index = params.indexOf(";");

        if(index != -1)
        {
            OpenVpn *item = this->foundItemInList(params.left(index).toInt());

            if (!item) {
                Debug::error(QLatin1String("Private NEEDED: No valid item."));
                goto check_available;
            }
            item->setPrivateKey(params.right(params.length() - index - 1));
        }
    } else if (command == QLatin1String("PWDNEEDED")) {
        if (params.isEmpty()) {
             goto check_available;
        }

        auto index = params.indexOf(";");

        if(index != -1)
        {
             OpenVpn *item = this->foundItemInList(params.left(index).toInt());

             if (!item) {
                 Debug::error(QLatin1String("PASS NEEDED: No valid item."));
                 goto check_available;
             }
             item->setPassword(params.right(params.length() - index - 1));
        }
    } else if (command == QLatin1String("CKNEEDED")) {
        if (params.isEmpty()) {
             goto check_available;
        }

        auto index = params.indexOf(";");

        if(index != -1)
        {
            OpenVpn *item = this->foundItemInList(params.left(index).toInt());

            if (!item) {
                Debug::error(QLatin1String("OTP NEEDED: No valid item."));
                goto check_available;
            }
            item->setChallengeKey(params.right(params.length() - index - 1));
        }
    } else if (command == QLatin1String("HTTPUSERNEEDED")) {
        if (params.isEmpty()) {
             goto check_available;
        }

        auto index = params.indexOf(";");

        if(index != -1)
        {
            OpenVpn *item = this->foundItemInList(params.left(index).toInt());

            if (!item) {
                Debug::error(QLatin1String("HTTP NEEDED: No valid item."));
                goto check_available;
            }
            item->setUsername(params.right(params.length() - index - 1));
        }

    } else if (command == QLatin1String("HTTPPASSNEEDED")) {
        if (params.isEmpty()) {
             goto check_available;
        }

        auto index = params.indexOf(";");

        if(index != -1)
        {
            OpenVpn *item = this->foundItemInList(params.left(index).toInt());

            if (!item) {
                Debug::error(QLatin1String("HTTP Pass NEEDED: No valid item."));
                goto check_available;
            }
            item->setPassword(params.right(params.length() - index - 1));
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
           _srvCLI->send(QLatin1String("ERROR"), QLatin1String("TAPINSTALL"));
             goto check_available;
        }
        Debug::log(QLatin1String("Tap device installed successfully"));
        _srvCLI->send(QLatin1String("OK"), QLatin1String("TAPINSTALL"));
    } else if(command == QLatin1String("TAPCOUNT")) {
         TapDriver tapDriver;
         _srvCLI->send(QString("%1").arg(tapDriver.deviceCount()), QLatin1String("TAPCOUNT"));
    } else if (command == QLatin1String("REMOVETAP")) {
        //
        // Installiert ein neues Tap Device
        Debug::log(QLatin1String("Remove all tap devices is triggered."));
        //
        TapDriver tapDriver;
        if (!tapDriver.removeTapDriver()) {
            // Die Tap Devices konnten nicht entfernt werden
            Debug::error(QLatin1String("Can't remove tap devices"));
            _srvCLI->send(QLatin1String("ERROR"), QLatin1String("TAPREMOVE"));
            goto check_available;
        }
        Debug::log(QLatin1String("Tap devices removes successfully"));
        _srvCLI->send(QLatin1String("OK"), QLatin1String("TAPREMOVE"));
    } else if (command == QLatin1String("DUMMY")) {
        this->setName(params);
        //
        // Test, für ist der service vorhanden
        Debug::log(QLatin1String("Test command is triggered."));
        _srvCLI->send(QString("Ok;%1").arg(getId()), QLatin1String("DUMMY"));
        //_srvCLI->send(QLatin1String("OK"), QLatin1String("DUMMY"));
    } else if (command == QLatin1String("GET_CLIENTS")) {
        auto list = Service::instance()->GetServer()->connectionsList();
        QString clients;
        for(auto i = 0; i < list->count(); ++i)
        {
            if(list->element(i) != this)
            {
                QString entry = list->element(i)->getName();
                entry += QString(" %1").arg(list->element(i)->getId());
                entry += QLatin1String("\n");
                clients.append(entry);
            }
        }


        _srvCLI->send(clients, QLatin1String("GET_CLIENTS"));
     } else if(command == QLatin1String("GETLOG")) {

         auto index = params.indexOf(";");

         if(index != -1)
         {
             auto con = Service::instance()->GetServer()->connectionsList()->element(params.left(index).toInt());
             if(con)
             {
                 auto conIndex = params.right(params.length() - index - 1).toInt();
                 if(con->srvcli())
                     con->srvcli()->send(QString("%1;%2").arg(getId()).arg(conIndex), "GETLOG");
                 else
                     _srvCLI->send("GETLOG", "ERROR");
             }
             else
                 _srvCLI->send("GETLOG", "ERROR");
         }
    } else if(command == QLatin1String("GETLOG_RESP")) {

        auto index = params.indexOf(";");

         if(index != -1)
         {
             auto con = Service::instance()->GetServer()->connectionsList()->element(params.left(index).toInt());
             if(con)
             {
                 if(con->srvcli())
                     con->srvcli()->send(QString(params.right(params.length() - index - 1)), "VPN_LOG");
                 else
                     _srvCLI->send("GETLOG", "ERROR");
             }
             else
                 _srvCLI->send("GETLOG", "ERROR");
         }
        return;
    } else if(command == QLatin1String("GET_ERROR")) {

         auto index = params.indexOf(";");

         if(index != -1)
         {
             auto con = Service::instance()->GetServer()->connectionsList()->element(params.left(index).toInt());
             if(con)
             {
                 auto conIndex = params.right(params.length() - index - 1).toInt();
                 con->srvcli()->send(QString("%1;%2").arg(getId()).arg(conIndex), "GET_ERROR");
             }
             else
                 _srvCLI->send("GET_ERROR", "ERROR");
         }
    } else if(command == QLatin1String("GET_ERROR_RESP")) {

        auto index = params.indexOf(";");

         if(index != -1)
         {
             auto con = Service::instance()->GetServer()->connectionsList()->element(params.left(index).toInt());
             if(con)
             {
                 con->srvcli()->send(QString(params.right(params.length() - index - 1)), "VPN_ERROR");
             }
             else
                 _srvCLI->send("GET_ERROR", "ERROR");
         }
        return;
    } else if(command == QLatin1String("STOP_CLIENTS")) {
        auto list = Service::instance()->GetServer()->connectionsList();
        QString clients;
        for(auto i = 0; i < list->count(); ++i)
        {
            list->element(i)->srvcli()->send("", "STOP_CLIENT");
        }

        _srvCLI->send("STOP", "STUFF");
        return;
    } else if(command == QLatin1String("ALL_STATUS")) {
        auto list = Service::instance()->GetServer()->connectionsList();
        QString clients;
        for(auto i = 0; i < list->count(); ++i)
        {
            //list->element(i)->
        }

         _srvCLI->send("DATA", "ALL_STATUS");
        return;
    } else {
        // unknown command error
        Debug::error(QLatin1String("Unknown command: ") + command + QLatin1String(" - P: ") + params);
    }

check_available:

    if(socket->bytesAvailable())
    {
        QCoreApplication::processEvents();
        emit slotStartRead();
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

    Q_UNUSED(err)

    if (socket->state() != QAbstractSocket::ConnectedState) {
        slotConnectionClosed();
    }
}

const quint64 SslServerConnection::threadId() const
{
    //
    // Return the internal id of this thread
    //
    return _threadId;
}
