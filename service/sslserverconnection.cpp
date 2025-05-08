#include "sslserverconnection.h"
#include "sslserver.h"
#include <QtNetwork/QSslKey>
#include <QCryptographicHash>

#include "debug.h"
#include "openvpn.h"
#include "srvcli.h"
#include "tapdriver.h"
#include "service.h"

#include <QUuid>

extern bool g_bPortable;

// We need in the next 10000 years no new one ;D
quint64 SslServerConnection::internalId = 0;

SslServerConnection::SslServerConnection(quint16 socketDescriptor, QObject *parent)
    : QThread (parent),
      blockSize (0),
      id (0)
{
    // Set the new internal id
    _threadId = internalId++;
    //
    Debug::log(QString("Create new connection with internal thread id %1 and socket descriptor %2")
               .arg(_threadId)
               .arg(socketDescriptor));

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
    Debug::log(QString("Set connection id %1")
               .arg(id));
    //
    this->id = id;
}

quint32 SslServerConnection::getId()
{
    return this->id;
}

void SslServerConnection::setName(QString name)
{
    Debug::log(QString("Set connection name %1")
               .arg(name));
    //
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

QString SslServerConnection::rewriteConfigPath(QString origin, QString newPathDirectory, ConfigPathTypes type)
{
    // Takes a configuration path and rewrite it to the new destination
    // Origin pathes looks like this:
    // ca filename
    // ca "filename"
    // ca 'filename'
    // ca "c:\\foo\\bar\\filename"
    // ca 'c:\\foo\\bar\\filename'

    // No destination dir, no new path...
    if (newPathDirectory.isEmpty()) {
        return "";
    }

    // We always need 2 parts, find the first space and take the right part of it
    int firstSpacePosition = origin.indexOf(" ");
    //
    if (firstSpacePosition == -1) {
        return "";
    }
    // Magic number alert ;)
    // -1 is to fix the position of the indexOf, due to it starts by 0
    // with the right method we need it by starting at 1,so we need to subtract one more char
    QString pathOrFilename (origin.right(origin.size() - firstSpacePosition - 1));
    // Cleanup string, remove all ",' and spaces
    pathOrFilename = pathOrFilename
                        .replace("'", "")
                        .replace("\"", "")
                        .simplified();

    // String should now looks like this
    // filename, c:\\foo\\bar\\filename
    // Check for a path
    if (pathOrFilename.indexOf("\\") > -1) {
        // Its a path, remove it, magic number explanation see above
        pathOrFilename = pathOrFilename.right(pathOrFilename.size() - pathOrFilename.lastIndexOf(("\\") - 1));
    }

    // We got only a filename, build the new
    QString setting;
    switch (type) {
        case ConfigPathTypes::CA:
            setting = "ca";
            break;
        case ConfigPathTypes::CERT:
            setting = "cert";
            break;
        case ConfigPathTypes::KEY:
            setting = "key";
            break;
        case ConfigPathTypes::AUTH:
            setting = "auth-user-pass";
            break;
        case ConfigPathTypes::EXTRA_CERTS:
            setting = "extra-certs";
            break;
        case ConfigPathTypes::SECRET:
            setting = "secret";
            break;
        case ConfigPathTypes::TLS_AUTH:
            setting = "tls-auth";
            break;
        case ConfigPathTypes::DH:
            setting = "dh";
            break;
        case ConfigPathTypes::PKCS12:
            setting = "pkcs12";
            break;
        case ConfigPathTypes::TLS_CRYPT:
            setting = "tls-crypt";
            break;
        default:
            return "";
    }

    return QString("%1 \"%2\\\\%3\"")
            .arg(setting)
            .arg(newPathDirectory.replace("/", "\\\\") /* OpenVPN needs escaped \ instead of / */)
            .arg(pathOrFilename);
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
        if (fields.size() == 8) {
            // 0: ID, 1: Username, 2: SID, 3: Path, 4: Content as Base64, 5: Interact, 6: Proxy-String, 7: isPortable
            // Ist der Eintrag schon in der Liste
            int configId (fields.at(0).toInt());
            QString configFullPath (fields.at(3));
            QString configPath = configFullPath.left(configFullPath.lastIndexOf("/"));
            QString originConfigPath (configPath);
            QString configName = configFullPath.right(configFullPath.size() - configFullPath.lastIndexOf("/") - 1);
            configName = configName.left(configName.size() - 5);
            QString usersVaultDirName;
            QString allVaultsPath;

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

            if (!g_bPortable) {
                // Check if its exists an read the content
                QString username (fields.at(1));
                QString userSID (fields.at(2));                
                QString configContent (QByteArray::fromBase64(fields.at(4).toUtf8()));

                // Check if we got content
                if(configContent.isEmpty()) {
                    _srvCLI->send(QString("%1;Empty configuration file found %2\n")
                                            .arg(configId)
                                            .arg(configFullPath), QLatin1String("ERROR"));
                    //
                    return;
                }

                // Create hash of the config content
                QString hashOfContent(QCryptographicHash::hash(configContent.toUtf8(), QCryptographicHash::Sha256).toHex());

                // Now check if its the same like in the registry
                // If no hash is found, set this hash as new one
                QSettings registry(QSettings::NativeFormat, QSettings::SystemScope, "Securepoint", "SSLVPN");

                QString registryHash = registry.value(QString("Users/%1/Security/%2/Checksum").arg(userSID).arg(configId)).toString();
                // Do we got an hash?
                if(registryHash.simplified().isEmpty()) {
                    // No, set the current as new hash
                    registry.setValue(QString("Users/%1/Security/%2/Checksum").arg(userSID).arg(configId), hashOfContent);
                    // Write a log message to notify the user
                    _srvCLI->send(QString("%1;Set new checksum %2 of %3 for user %4[%5]\n")
                                            .arg(configId)
                                            .arg(hashOfContent)
                                            .arg(configFullPath)
                                            .arg(username)
                                            .arg(userSID), QLatin1String("LOG"));

                } else {
                    // We got a checksum, both must be equal!
                    if (registryHash != hashOfContent) {
                        // Thats really bad stuff
                        _srvCLI->send(QString("%1;Checksum error, rejecting connect request! %2 Getting %3 expecting %4.\n")
                                                .arg(configId)
                                                .arg(configFullPath)
                                                .arg(hashOfContent)
                                                .arg(registryHash), QLatin1String("ERROR"));
                        //
                        return;
                    }

                    // Write a short notice
                    _srvCLI->send(QString("%1;Checksum %2 is okay\n")
                                            .arg(configId)
                                            .arg(hashOfContent), QLatin1String("LOG"));
                }

                // Now read the config, rewrite the ca, cert, key,auth file paths and write it to a save space in %Programms%
                // This flag controls the connection reject if an invalid script security is detected

                // Additionally we check the config to some "special" settings which are unsafe fo rmany reasons.
                // Particularly we skip or reject settings which make it possible to do a local privilege escalation!
                // This happens already partly in 2.0.31(only rejecting custom scrips). This version rejects more settings.
                // If this feature are needed, please take the source of 2.0.30 and fork it.

                QStringList lines = configContent
                                    /* Replace possible crappy windows line ends with \n*/
                                    .replace("\r\n", "\n")
                                    /* And now split the lines into a list */
                                    .split("\n");

                int lineCount (lines.size());
                QStringList connectConfigLines;
                // Loop all lines and handle some of them (comments, ca, cert, key, auth-user-pass, script security etc.)
                // Please note that the client can't handle inline certs, keys and similar stuff.
                for (int lineIndex = 0; lineIndex < lineCount; ++lineIndex) {
                    QString line = lines.value(lineIndex);
                    // Cleanup spaces and other crap, replace all multi whitespaces with a single one
                    line = line.simplified();

                    // Check for comments
                    if (line.startsWith("#")) {
                        continue;
                    } else if (line.toLower().startsWith("win-sys")) {
                        // OpenVpn ignore this line. In case of rebuild-in this feature in future we skip it, due to security reasons.
                        continue;
                    } else if (line.toLower().startsWith("script-security")) {
                        // The user sets a new script security. Check the value
                        // Values possible:
                        // 0 - no execution allowd
                        // 1 - only build-in tools
                        // 2 - script file like vbs, bat ...
                        // 3 - password passed to scripts bei env variables
                        //
                        // Only 0 and 1 is allowed! All other connections are rejected!
                        // Split in key value
                        QStringList keyValue = line.split(" ");
                        // If its not 2 its not a valid value, reject it
                        bool isScriptSecurityError (false);
                        if (keyValue.size() != 2) {
                            isScriptSecurityError = true;
                        } else {
                            // Its a valid value check the value for level 0 and 1
                            QString value = keyValue.value(1);
                            //
                            if (!(value == "0" || value == "1")) {
                                isScriptSecurityError = true;
                            }
                        }

                        // Do we have an script security error?
                        if (isScriptSecurityError) {
                            // Yes, reject the connection! We'll never execute scripts with a script security higher than 1
                            // see: https://openvpn.net/community-resources/reference-manual-for-openvpn-2-4 @ –script-security level
                            _srvCLI->send(QString("%1;%2")
                                             .arg(configId)
                                             .arg("Unsafe configuration found: use of script-security greater than 1. This connection is rejected! For further informations see: https://openvpn.net/community-resources/reference-manual-for-openvpn-2-4, section: script-security level"), QLatin1String("ERROR"));
                            return;
                        }
                    }

                    // All checks are done, append line to the connection config.
                    connectConfigLines.append(line);
                }

                // Rewrite config data
                configContent = connectConfigLines.join("\n");

                // Get this app dir
                QString serviceDir (qApp->applicationDirPath());
                QString allUsersVault (serviceDir + "/vault");
                QString usersVault (serviceDir + "/vault/" + userSID);
                QString vaultConfigName = QUuid::createUuid().toString()
                                                             .replace("{", "")
                                                             .replace("}", "");

                // Remove dir
                QDir allUsersVaultDir(allUsersVault);
                if (allUsersVaultDir.exists(allUsersVault)) {
                    allUsersVaultDir.removeRecursively();
                }

                // Create a new clean one
                QDir usersVaultDir (usersVault);
                if (!usersVaultDir.mkpath(usersVault)) {
                    _srvCLI->send(QString("%1;Can't create configuration vault.\n")
                                            .arg(configId), QLatin1String("ERROR"));
                    //
                    return;
                }

                {
                    // All fine, write file
                    QFile writer (usersVault + "/" + vaultConfigName + ".ovpn");
                    if (!writer.open(QFile::WriteOnly | QFile::Truncate)) {
                        _srvCLI->send(QString("%1;Can't write configuration file.\n")
                                                .arg(configId), QLatin1String("ERROR"));
                        //
                        return;
                    }

                    writer.write(configContent.toUtf8());
                    writer.flush();
                    writer.waitForBytesWritten(500);

                    writer.close();
                }

                // After this pass the new file to openvpn
                configPath = usersVault;
                configName = vaultConfigName;

                //
                usersVaultDirName = userSID;
                allVaultsPath = allUsersVault;

            } // End of portable check

            // Resume with normal connect
            // Alls ok, wir bauen us ein neues Objekt
            OpenVpn *sslVpn = new OpenVpn (_srvCLI);
            // Nun die Parameter setzen            
            sslVpn->setConfigPath(configPath);
            sslVpn->setConfigName(configName);
            sslVpn->setUseInteract(fields.at(5));
            sslVpn->setProxyString(fields.at(6));
            sslVpn->setId(configId);
            sslVpn->setUserVaultDirName(usersVaultDirName);
            sslVpn->setVaultDirName(allVaultsPath);
            sslVpn->setOriginConfigPath(originConfigPath);

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

        OpenVpn *item = 0;
        // We need to check if we need to use another connection
        // because we got a cli command from a client
        if (params.indexOf(";") > 0) {
            Debug::error(QLatin1String("Close: CLI command"));
            // Get the client id and the conneciton id
            QStringList ids = params.split(";");
            //
            if (ids.size() != 2) {
                Debug::error(QLatin1String("Close: invalid client;connectionid."));
                goto check_available;
            }

            int clientId = QString(ids.value(0)).toInt();
            int connectionId = QString(ids.value(1)).toInt();

            auto list = Service::instance()->GetServer()->connectionsList();
            //
            for(auto i = 0; i < list->count(); ++i) {
                if(list->element(i) != this) {
                    if (list->element(i)->getId() == clientId) {
                        Debug::error(QLatin1String("Close: Find connection"));
                        item = list->element(i)->foundItemInList(connectionId);
                        break;
                    }
                }
            }

            // Check
            if (!item) {
                Debug::error(QLatin1String("Close: No valid item."));
                goto check_available;
            }

        } else {
            Debug::error(QLatin1String("Close: Nornmal client command"));
            // Normal client, no cli command
            // Get the connection
            item = this->foundItemInList(params.toInt());

            Q_ASSERT(item);

            if (!item) {
                Debug::error(QLatin1String("Close: No valid item."));
                goto check_available;
            }
        }

        // Disconnect triggern
        if (item) {
            item->disconnectVpn();
        }
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
         Debug::log(QLatin1String("_srvCLI->send tapcount"));
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
    } else if (command == QLatin1String("WRITE_CHECKSUM")) {
        //
        // Create a checksum of the configuration and store it into the registry
        //

        // ID, Path, Content as Base64
        QStringList fields (params.split(";"));
        if (fields.size() == 6) {
            // ID, Username, SID, Path, Content as Base64, isPortable
            int configId (fields.at(0).toInt());
            QString username (fields.at(1));
            QString userSID (fields.at(2));
            QString configPath (fields.at(3));
            QString configContent (fields.at(4));

            // In Portable mode we are always run in admin context otherwise
            // netherthanless it doesn't matter who we are,
            // because we are running in user context
            if (g_bPortable) {
                return;
            }

            // Now write this hash into the registry
            // We write in HKLM due to protect the checksum of manipulation.

            QSettings registry(QSettings::NativeFormat, QSettings::SystemScope, "Securepoint", "SSLVPN");
            registry.setValue(QString("Users/%1/Security/%2/Checksum").arg(userSID).arg(configId), configContent);

            // Write a log message to notify the user
            _srvCLI->send(QString("%1;Write checksum %2 of %3 for user %4[%5]\n")
                                    .arg(configId)
                                    .arg(configContent)
                                    .arg(configPath)
                                    .arg(username)
                                    .arg(userSID), QLatin1String("LOG"));
            //
        }
    } else if (command == QLatin1String("CHECKSUM")) {
        //
        // Create a checksum of the configuration and store it into the registry
        //

        // ID, Path, Content as Base64
        QStringList fields (params.split(";"));
        if (fields.size() == 6) {
            // ID, Username, SID, Path, Content as Base64, isPortable
            int configId (fields.at(0).toInt());
            QString username (fields.at(1));
            QString userSID (fields.at(2));
            QString configPath (fields.at(3));
            QString configContent (QByteArray::fromBase64(fields.at(4).toUtf8()));

            // In Portable mode we are always run in admin context otherwise
            // netherthanless it doesn't matter who we are,
            // because we are running in user context
            if (g_bPortable) {
                return;
            }

            // Create hash of the config content
            QString hashOfContent(QCryptographicHash::hash(configContent.toUtf8(), QCryptographicHash::Sha256).toHex());

            // Now write this hash into the registry
            // We write in HKLM due to protect the checksum of manipulation.

            QSettings registry(QSettings::NativeFormat, QSettings::SystemScope, "Securepoint", "SSLVPN");
            registry.setValue(QString("Users/%1/Security/%2/Checksum").arg(userSID).arg(configId), hashOfContent);

            // Write a log message to notify the user
            _srvCLI->send(QString("%1;Create checksum %2 of %3 for user %4[%5]\n")
                                    .arg(configId)
                                    .arg(hashOfContent)
                                    .arg(configPath)
                                    .arg(username)
                                    .arg(userSID), QLatin1String("LOG"));
            //
        }
    } else if (command == QLatin1String("REMOVE_CHECKSUM")) {
        //
        // Create a checksum of the configuration and store it into the registry
        //

        // ID, Path, Content as Base64
        QStringList fields (params.split(";"));
        if (fields.size() == 4) {
            // ID, Username, SID, isPortable
            int configId (fields.at(0).toInt());
            QString username (fields.at(1));
            QString userSID (fields.at(2));

            // In Portable mode we are always run in admin context otherwise
            // netherthanless it doesn't matter who we are,
            // because we are running in user context
            if (g_bPortable) {
                return;
            }

            {
                QSettings registryConfig(QSettings::NativeFormat, QSettings::SystemScope, "Securepoint", "SSLVPN");
                registryConfig.remove(QString("Users/%1/Security/%2").arg(userSID).arg(configId));
            }

            // Check if we need to remove the userkey, too
            QSettings registryUser(QSettings::NativeFormat, QSettings::SystemScope, "Securepoint", "SSLVPN");
            // Start a group and count the sub keys
            registryUser.beginGroup(QString("Users/%1").arg(userSID));
            QStringList keys = registryUser.allKeys();
            registryUser.endGroup();
            //
            if(keys.size() == 0) {
                // No more checksum available for this user, remove the entry
                registryUser.remove(QString("Users/%1").arg(userSID));
            }
        }
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
