#include "datacontroll.h"
#include "usercontroll.h"

DataControll::DataControll(quint16 port, QObject* parent) : QTcpServer(parent)
{
        listen(QHostAddress::LocalHost, port);
        this->isConnectedService = false;
        this->configName = "";
        this->configPath = "";
        this->configPwd = "";
        this->configUser = "";
        this->config = "";
        this->disconnectSend = false;
        this->button = NULL;
        this->scScriptRun = false;

        this->onConnect = true;
        this->timer = new QTimer(this);
        connect(this->timer, SIGNAL(timeout()), this, SLOT(refreshStatus()));
        this->timer->start(2000);
}

void DataControll::discardClient() {
     //Log::write("CLIENT: DISCARD CLIENT");
     QTcpSocket* socket = (QTcpSocket*)sender();
     socket->deleteLater();
}

void DataControll::readClient() {
    // Should be replaced with a better solution
    // sending and reading blocks.
    // Maybe next release
    //Log::write("CLIENT: READY READ");
    QTcpSocket* socket = (QTcpSocket*)sender();
    if (socket->canReadLine()) {        
        QStringList tokens = QString(socket->readAll()).split(QRegExp("[\r\n][\r\n]*"));
        for (int i=0; i<tokens.size();i++) {
            QString log = "";
            if (QString(tokens[i]).indexOf("log->") != -1) {
                // Command found
                if (QString(tokens[i]).split("->").size() == 2) {
                    QStringList spl = QString(tokens[i]).split("->");
                    log = spl[1];                    
                    if (log.left(7) != "STATUS:" && log.left(5) != "USER:" && log.left(5) != "UPWD:" && log.indexOf("DISCONNECTED") == -1)
                        ServiceLog::getInstance()->appendToLog(log.replace("#<>#","\n"));
                    if (log.indexOf("DISCONNECTED") != -1) {
                        //QMessageBox::information(0,"","received dis");
                        if (this->disconnectSend) {
                            QTimer::singleShot(0, this, SLOT(makeDisconnectSingleShoot()));
                        }
                        this->isConnectedService = false;
                    } else if (log.contains("Notified TAP-Win32 driver to set a DHCP IP", Qt::CaseInsensitive)) {
                        // IP Suchen und speichern
                        QString connIP = log.mid(log.indexOf("Notified TAP-Win32 driver to set a DHCP IP")+54,15);
                        // IP durchlaufen und / und spaces entfernen
                        int indexOfChar = 0;
                        indexOfChar = connIP.indexOf("/",0);
                        if (indexOfChar != 0) {
                            // Maskeabschneiden
                            connIP = connIP.left(indexOfChar);
                        } else {
                            // Lerrzeichen da?
                            indexOfChar = connIP.indexOf(" ",0);
                            if (indexOfChar != 0) {
                                // Leerzeichen abschneiden
                                connIP = connIP.left(indexOfChar);
                            }
                        }
                        QIcon icon = QIcon(":/images/connected.png");
                        MainWindowControll::getInstance()->setTrayIcon(icon);
                        this->isConnectedService = true;
                        this->onConnect = false;

                        this->button->getParentItemCast()->getOpenVPN()->setIsConnecting(false);
                        this->button->getParentItemCast()->getOpenVPN()->setIsConnected(true);
                        this->button->getParentItemCast()->getOpenVPN()->setIsError(false);
                        this->button->getParentItemCast()->getOpenVPN()->setErrorString("");
                        this->button->getParentItemCast()->getOpenVPN()->setConnectedIP(connIP);
                        this->button->getParentItemCast()->getOpenVPN()->setConnectedSinceDate(QDate::currentDate());
                        this->button->getParentItemCast()->getOpenVPN()->setConnectedSinceTime(QTime::currentTime());
                        this->button->setConnected(connIP);

                        if (!Settings::getInstance()->getIsShowNoBallonMessage())
                            MainWindowControll::getInstance()->showTrayMessage(QString("Connnected to VPN.\nName: ") + this->vpnName + QString("\nIP: ") + connIP);

                        MainWindowControll::getInstance()->enableTreeButtons();
                        if (!scScriptRun) {
                            QString timeOut = this->getScript("TO");
                            int scriptDelay = 0;
                            if (timeOut != "") {
                                scriptDelay = timeOut.trimmed().toInt();
                            }
                            QTimer::singleShot(scriptDelay, this, SLOT(startAfterConnectDelayed()));
                        }
                        this->scScriptRun = true;

                        if (!this->timer->isActive())
                            this->timer->start(2000);

                    } else if (log.left(6) == "ERROR:") {
                        // Error occured
                        QIcon icon = QIcon(":/images/disconnected.png");
                        MainWindowControll::getInstance()->setTrayIcon(icon);
                        this->isConnectedService = false;
                        QString errMes = log.right(log.size () - 6);
                        this->button->getParentItemCast()->getOpenVPN()->setIsConnecting(false);
                        this->button->getParentItemCast()->getOpenVPN()->setIsConnected(false);
                        this->button->getParentItemCast()->getOpenVPN()->setIsError(true);
                        this->button->getParentItemCast()->getOpenVPN()->setErrorString(errMes);
                        this->button->getParentItemCast()->getOpenVPN()->setConnectedIP("");
                        this->button->setError(errMes);
                        if (!Settings::getInstance()->getIsShowNoBallonMessage())
                            MainWindowControll::getInstance()->showTrayMessage(errMes);
                        MainWindowControll::getInstance()->enableTreeButtons();
                        this->button->getParentItemCast()->getOpenVPN()->runScript("EC");
                    } else if (log.left(5) == "USER:") {
                        // User angefordert
                        ServiceLog::getInstance()->appendToLog(tr("Username requested.\n"));
                        if (!Settings::getInstance()->getIsStartWithUser()) {
                            // Es wurde kein User angegeben, nun den User ermitteln
                            // entweder aus der Crypt, keine User Auth oder über den
                            // Dialog einlesen
                            if (Settings::getInstance()->getUseCryptedData()) {
                                // Crypt wird benutzt
                                // Ist eine Crypt vorhanden
                                UserControll::getInstance()->setConfigDirectory(this->button->getParentItemCast()->getOpenVPN()->configPath);
                                if (!UserControll::getInstance()->isUserNameConfigWithCryptAvailable()) {
                                    // Kein gespeicherter Username vorhanden
                                    // Ist die Anzeige an?
                                    if (!Settings::getInstance()->getIsShowNoUserAuth()) {
                                        // Die Daten nun einlesen
                                        this->userDataLock = true;
                                        disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                        connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                        FrmGetUserData::getInstance()->setFrmType(0);
                                        FrmGetUserData::getInstance()->show();
                                        while (this->userDataLock) {
                                            FrmGetUserData::getInstance()->exec();
                                            QApplication::processEvents();
                                        }
                                        // Die Daten sind nun da, wurde Speichern aktiviert?
                                        if (FrmGetUserData::getInstance()->isSaveChecked()) {
                                            // Speichern wurde aktiviert
                                            // Usernamen zwischenspeichern, falls der Key mit dem Dialog eingelesen werden soll
                                            QString _dataField = FrmGetUserData::getInstance()->getDataField();
                                            // Ist der Private Key schon bekannt?
                                            if (UserControll::getInstance()->getPrivateKeyPub() == "") {
                                                // Key ist leer, einlesen
                                                this->userDataLock = true;
                                                disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                                connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                                FrmGetUserData::getInstance()->setFrmType(4);
                                                FrmGetUserData::getInstance()->show();
                                                while (this->userDataLock) {
                                                    FrmGetUserData::getInstance()->exec();
                                                    QApplication::processEvents();
                                                }
                                                // Key in der Klasse setzen und für den Rest der Anwedung merken
                                                UserControll::getInstance()->setPrivateKey(FrmGetUserData::getInstance()->getDataField());
                                            }
                                            // Key und Daten da, speichern
                                            UserControll::getInstance()->setConfigDirectory(this->button->getParentItemCast()->getOpenVPN()->configPath);
                                            UserControll::getInstance()->setUserNameCrypt(_dataField);
                                            this->button->getParentItemCast()->getOpenVPN()->setIsCryptFileAvailable(true);
                                            MainWindowControll::getInstance()->refreshDialog();
                                        }
                                    }
                                } else {
                                    // Crypt ist da, Private Key auch?
                                    if (UserControll::getInstance()->getPrivateKeyPub() == "" || Settings::getInstance()->getIsForcePrivateKey()) {
                                        // Key ist leer, einlesen
                                        this->userDataLock = true;
                                        disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                        connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                        FrmGetUserData::getInstance()->setFrmType(4);
                                        FrmGetUserData::getInstance()->show();
                                        while (this->userDataLock) {
                                            FrmGetUserData::getInstance()->exec();
                                            QApplication::processEvents();
                                        }
                                        // Key in der Klasse setzen und für den Rest der Anwedung merken
                                        UserControll::getInstance()->setPrivateKey(FrmGetUserData::getInstance()->getDataField());
                                    }
                                }
                            } else {
                                // Crypt wurde nicht aktiviert
                                // Ist die Anzeige aktiv
                                if (!Settings::getInstance()->getIsShowNoUserAuth()) {
                                    // Dialog anzeigen und Usernamen auslesen
                                    this->userDataLock = true;
                                    disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                    connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                    FrmGetUserData::getInstance()->setFrmType(0);
                                    FrmGetUserData::getInstance()->show();
                                    while (this->userDataLock) {
                                        FrmGetUserData::getInstance()->exec();
                                        QApplication::processEvents();
                                    }
                                    // Daten werden in der sendUser ausgelesen
                                }
                            }
                        }
                        // Daten senden
                        this->sendUser();
                    } else if (log.left(5) == "UPWD:") {
                        // User PWD angefordert
                        ServiceLog::getInstance()->appendToLog(tr("User password requested.\n"));
                        if (!Settings::getInstance()->getIsStartWithPwd()) {
                            // Es wurde kein User angegeben, nun den User ermitteln
                            // entweder aus der Crypt, keine User Auth oder über den
                            // Dialog einlesen
                            if (Settings::getInstance()->getUseCryptedData()) {
                                // Crypt wird benutzt
                                // Ist eine Crypt vorhanden
                                UserControll::getInstance()->setConfigDirectory(this->button->getParentItemCast()->getOpenVPN()->configPath);
                                if (!UserControll::getInstance()->isUserPasswordConfigWithCryptAvailable()) {
                                    // Kein gespeicherter Username vorhanden
                                    // Ist die Anzeige an?
                                    if (!Settings::getInstance()->getIsShowNoUserAuth()) {
                                        // Die Daten nun einlesen
                                        this->userDataLock = true;
                                        disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                        connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                        FrmGetUserData::getInstance()->setFrmType(1);
                                        FrmGetUserData::getInstance()->show();
                                        while (this->userDataLock) {
                                            FrmGetUserData::getInstance()->exec();
                                            QApplication::processEvents();
                                        }
                                        // Die Daten sind nun da, wurde Speichern aktiviert?
                                        if (FrmGetUserData::getInstance()->isSaveChecked()) {
                                            // Speichern wurde aktiviert
                                            // Usernamen zwischenspeichern, falls der Key mit dem Dialog eingelesen werden soll
                                            QString _dataField = FrmGetUserData::getInstance()->getDataField();
                                            // Ist der Private Key schon bekannt?
                                            if (UserControll::getInstance()->getPrivateKeyPub() == "") {
                                                // Key ist leer, einlesen
                                                this->userDataLock = true;
                                                disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                                connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                                FrmGetUserData::getInstance()->setFrmType(4);
                                                FrmGetUserData::getInstance()->show();
                                                while (this->userDataLock) {
                                                    FrmGetUserData::getInstance()->exec();
                                                    QApplication::processEvents();
                                                }
                                                // Key in der Klasse setzen und für den Rest der Anwedung merken
                                                UserControll::getInstance()->setPrivateKey(FrmGetUserData::getInstance()->getDataField());
                                            }
                                            // Key und Daten da, speichern
                                            UserControll::getInstance()->setConfigDirectory(this->button->getParentItemCast()->getOpenVPN()->configPath);
                                            UserControll::getInstance()->setUserPasswordCrypt(_dataField);
                                            this->button->getParentItemCast()->getOpenVPN()->setIsCryptFileAvailable(true);
                                            MainWindowControll::getInstance()->refreshDialog();
                                        }
                                    }
                                } else {
                                    // Crypt ist da, Private Key auch?
                                    if (UserControll::getInstance()->getPrivateKeyPub() == "" || Settings::getInstance()->getIsForcePrivateKey()) {
                                        // Key ist leer, einlesen
                                        this->userDataLock = true;
                                        disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                        connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                        FrmGetUserData::getInstance()->setFrmType(4);
                                        FrmGetUserData::getInstance()->show();
                                        while (this->userDataLock) {
                                            FrmGetUserData::getInstance()->exec();
                                            QApplication::processEvents();
                                        }
                                        // Key in der Klasse setzen und für den Rest der Anwedung merken
                                        UserControll::getInstance()->setPrivateKey(FrmGetUserData::getInstance()->getDataField());
                                    }
                                }
                            } else {
                                // Crypt wurde nicht aktiviert
                                // Ist die Anzeige aktiv
                                if (!Settings::getInstance()->getIsShowNoUserAuth()) {
                                    // Dialog anzeigen und Usernamen auslesen
                                    this->userDataLock = true;
                                    disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                    connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                    FrmGetUserData::getInstance()->setFrmType(1);
                                    FrmGetUserData::getInstance()->show();
                                    while (this->userDataLock) {
                                        FrmGetUserData::getInstance()->exec();
                                        QApplication::processEvents();
                                    }
                                    // Daten werden in der sendUser ausgelesen
                                }
                            }
                        }
                        // Daten senden
                        this->sendPwd();
                    } else if (log.left(6) == "PKPWD:") {
                        // Priv Key angefordert
                        ServiceLog::getInstance()->appendToLog(tr("PKCS12 key requested.\n"));
                        this->userDataLock = true;
                        disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                        connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                        FrmGetUserData::getInstance()->setFrmType(3);
                        FrmGetUserData::getInstance()->show();
                        while (this->userDataLock) {
                            FrmGetUserData::getInstance()->exec();
                            QApplication::processEvents();
                        }
                        this->sendPrivKey();
                    } else if (log.left(6) == "CKPWD:") {
                        // Challenge Key angefordert
                        // Verbindung wieder auf Connecting setzen
                        this->button->setConnecting();
                        ServiceLog::getInstance()->appendToLog(tr("Challenge key requested.\n"));
                        this->userDataLock = true;
                        disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                        connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                        FrmGetUserData::getInstance()->setFrmType(2);
                        FrmGetUserData::getInstance()->show();
                        while (this->userDataLock) {
                            FrmGetUserData::getInstance()->exec();
                            QApplication::processEvents();
                        }
                        this->sendChallengeKey();
                    } else if (log.left(8) == "RESTART:") {
                        // Restart aufgetreten
                        // Verbindung wieder auf Connecting setzen
                        this->button->setConnecting();
                        QIcon icon = QIcon(":/images/connecting.png");
                        MainWindowControll::getInstance()->setTrayIcon(icon);
                        ServiceLog::getInstance()->appendToLog(tr("Received Restart.\n"));
                    } else if (log.left(7) == "STATUS:") {
                        // Status received
                        QStringList statuslist = log.right(log.size () - 7).split("<>");
                        this->isConnectedService = (statuslist[0] == "true" ? true : false);
                        if (!this->onConnect)
                            if (!this->isConnectedService)
                                if (Settings::getInstance()->getIsAutoReconnect())
                                    if (this->config != "") {
                                        // reconnet to service
                                        this->onConnect = true;
                                        ServiceLog::getInstance()->appendToLog(tr("Connection lost! Try restart ..."));
                                        QTimer::singleShot(2000, this, SLOT(restartConnection()));
                                    }
                        this->configPath = statuslist[1];
                        this->configName = statuslist[2];
                        //pref->refreshData();
                    }
                }
            }
        }
    }

}

void DataControll::userDataIsAvailable() {
    this->userDataLock = false;
}

void DataControll::incomingConnection(int socket) {
    //Log::write("CLIENT: INCOMMING CONNECTION");
    QTcpSocket* s = new QTcpSocket(this);
    connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
    //Log::write("CLIENT: SET SOCKET DESCTIPTOR: " + QString::number(socket));
    s->setSocketDescriptor(socket);
    //Log::write("CLIENT: WAIT FOR READY READ");
}

void DataControll::sendConnect() {
    int rc = 0;
    foreach (OpenVpn *obj, Configs::getInstance()->getConfigsObjects()) {
        if (obj == this->button->getParentItemCast()->getOpenVPN()) {
            if (rc != 0) {
                Configs::getInstance()->swap(rc, 0);
                break;
            }
        }
        rc++;
    }
    this->scScriptRun = false;
    this->button->getParentItemCast()->getOpenVPN()->runScript("BC");
    ServiceLog::getInstance()->appendToLog(tr("Sending connect ..."));
    this->button->getParentItemCast()->getOpenVPN()->setIsConnecting(true);
    this->button->getParentItemCast()->getOpenVPN()->setIsConnected(false);
    this->button->getParentItemCast()->getOpenVPN()->setIsError(false);
    this->button->getParentItemCast()->getOpenVPN()->setErrorString("");
    this->button->getParentItemCast()->getOpenVPN()->setConnectedIP("");
    if (!this->onConnect) {
        // Wenn es schon true ist, kommt es durch einen reconnect
        // dann das Log behalten
        ServiceLog::getInstance()->clearLog();
    }

    this->onConnect = true;
    this->disconnectSend = false;
    QString conPath = "";
    QString config = this->config;
    // From Win to QT Path
    config = config.replace("\\", "/");
    QFile haveUserRights(config);

    if (!haveUserRights.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QIcon icon = QIcon(":/images/disconnected.png");
        MainWindowControll::getInstance()->setTrayIcon(icon);
        this->button->setReadyToConnect();
        if (!Settings::getInstance()->getIsShowNoBallonMessage()) {
            QString errMes = QString("Error: No permission to open config file!");
            this->button->setError(errMes);
            MainWindowControll::getInstance()->showTrayMessage(errMes);
        }
        return;
    }
    if (this->timer->isActive())
        this->timer->stop();

    QString ConfigName = "";

    if (config.lastIndexOf("\\") != -1) {
        ConfigName = config.right(config.size() - config.lastIndexOf("\\") -1);
        conPath = config.left(config.lastIndexOf("\\"));
        conPath = conPath.replace("\\", "/");
    } else {
        ConfigName = config.right(config.size() - config.lastIndexOf("/") -1);
        conPath = config.left(config.lastIndexOf("/"));
    }
    this->configPath = conPath;

    ConfigName = ConfigName.left(ConfigName.size()-5);

    this->vpnName = ConfigName;


    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(QString("127.0.0.1"), this->servicePort);

    if (socket->waitForConnected(1000)) {
        QTextStream os(socket);

        os.setAutoDetectUnicode(true);
        os << "command->open\r\n"
           << "config->" + config + "\r\n"
           << "user->" + this->configUser + "\r\n"
           << "pwd->" + this->configPwd + "\r\n"
           << "usenointact->" + (Settings::getInstance()->getUseNoInteract() ? QString("1") : QString("0")) + "\r\n"
           << QDateTime::currentDateTime().toString() << "\n";

        socket->close();
        QIcon icon = QIcon(":/images/connecting.png");
        MainWindowControll::getInstance()->setTrayIcon(icon);
        this->button->setConnecting();
    } else {
        this->serviceNotAvailable();
    }
    while (socket->state() != QTcpSocket::UnconnectedState) {
        qApp->processEvents();
    }
    if (socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
    }
    MainWindowControll::getInstance()->refreshDialog();
}

void DataControll::sendDisconnect() {
    this->button->getParentItemCast()->getOpenVPN()->runScript("BD");
    ServiceLog::getInstance()->appendToLog(tr("Sending disconnect ..."));
    //QMessageBox::information(0,"","disconenct");
    this->onConnect = true;
    this->disconnectSend = true;
    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(QString("127.0.0.1"), this->servicePort);

    if (socket->waitForConnected(1000)) {
        QTextStream os(socket);

        os.setAutoDetectUnicode(true);
        os << "command->close\r\n"
           << QDateTime::currentDateTime().toString() << "\n";

        socket->close();
    } else {
        this->serviceNotAvailable();
    }
    while (socket->state() != QTcpSocket::UnconnectedState) {
        qApp->processEvents();
    }
    if (socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
    }
}

void DataControll::sendDisconnectWithoutGUI() {
    this->disconnectSend = false;
    this->onConnect = true;
    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(QString("127.0.0.1"), this->servicePort);

    if (socket->waitForConnected(1000)) {
        QTextStream os(socket);

        os.setAutoDetectUnicode(true);
        os << "command->close\r\n"
           << QDateTime::currentDateTime().toString() << "\n";

        socket->close();
    } else {
        this->serviceNotAvailable();
    }
    while (socket->state() != QTcpSocket::UnconnectedState) {
        qApp->processEvents();
    }
    if (socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
    }
}

void DataControll::refreshStatus() {
    this->sendGetStatus();
}

void DataControll::sendGetStatus() {
    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(QString("127.0.0.1"), this->servicePort);

    if (socket->waitForConnected(1000)) {
        QTextStream os(socket);

        os.setAutoDetectUnicode(true);
        os << "command->getstatus\r\n"
           << QDateTime::currentDateTime().toString() << "\n";

        socket->close();
    } else {
        this->serviceNotAvailable();
    }
    this->timer->stop();
    while (socket->state() != QTcpSocket::UnconnectedState) {
        qApp->processEvents();
    }
    this->timer->start(2000);
    if (socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
    }
}

void DataControll::sendUser() {
    ServiceLog::getInstance()->appendToLog(tr("Sending user ..."));
    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(QString("127.0.0.1"), this->servicePort);

    if (socket->waitForConnected(1000)) {
        QTextStream os(socket);

        os.setAutoDetectUnicode(true);
        QString _user = "";        
        if (Settings::getInstance()->getIsStartWithUser()) {
            // Kommandozeilen user hat höchste Priorität
            _user = Settings::getInstance()->getStartWithUser();
        } else {
            // Wurde der Client mit Crypt unterstützung gestartet?
            if (Settings::getInstance()->getUseCryptedData()) {
                // Kein Kommandozeilen User angegeben, ist eine
                // Crypt Datei da?
                UserControll::getInstance()->setConfigDirectory(this->button->getParentItemCast()->getOpenVPN()->configPath);
                if (UserControll::getInstance()->isUserNameConfigWithCryptAvailable()) {
                    // Der Username ist in der Crypt vorhanden.
                    UserControll::getInstance()->setConfigDirectory(this->button->getParentItemCast()->getOpenVPN()->configPath);
                    _user = UserControll::getInstance()->getUserNamePlain();
                } else {
                    // Crypt Datei ist nicht vorhanden, wurde die Anzeige der Auth abgeschaltet
                    if (Settings::getInstance()->getIsShowNoUserAuth()) {
                        // Wenn keine Auth an ist, wird ein leerer String übertragen
                        _user = "";
                    } else {
                        // Authentifizierung ist an, nun den Wert des Dialoges nehmen
                        // Gespeichert wurde schon bei save flag vorher,
                        // deswegen kann hier direkt der Wert genommen werden.
                        _user = FrmGetUserData::getInstance()->getDataField();
                    }
                }
            } else {
                // Es wird keine Crypt benutzt, eine vorhandene wird ignoriert
                // Wurde die Anzeige der Auth abgeschaltet
                if (Settings::getInstance()->getIsShowNoUserAuth()) {
                    // Wenn keine Auth an ist, wird ein leerer String übertragen
                    _user = "";
                } else {
                    // Authentifizierung ist an, nun den Wert des Dialoges nehmen
                    // Gespeichert wurde schon bei save flag vorher,
                    // deswegen kann hier direkt der Wert genommen werden.
                    _user = FrmGetUserData::getInstance()->getDataField();
                }
            }
        }
        os << "uneeded->" << _user << "\r\n"
           << QDateTime::currentDateTime().toString() << "\n";

        socket->close();
    } else {
        this->serviceNotAvailable();
    }
    this->timer->stop();
    while (socket->state() != QTcpSocket::UnconnectedState) {
        qApp->processEvents();
    }
    this->timer->start(2000);
    if (socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
    }
}

void DataControll::sendPwd() {
    ServiceLog::getInstance()->appendToLog(tr("Sending password ..."));
    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(QString("127.0.0.1"), this->servicePort);

    if (socket->waitForConnected(1000)) {
        QTextStream os(socket);

        os.setAutoDetectUnicode(true);
        QString _pwd = "";
        if (Settings::getInstance()->getIsStartWithPwd()) {
            // Kommandozeilen pwd hat höchste Priorität
            _pwd = Settings::getInstance()->getStartWithPwdPwd();
        } else {
            // Wurde der Client mit Crypt unterstützung gestartet?
            if (Settings::getInstance()->getUseCryptedData()) {
                // Kein Kommandozeilen Pwd angegeben, ist eine
                // Crypt Datei da?
                UserControll::getInstance()->setConfigDirectory(this->button->getParentItemCast()->getOpenVPN()->configPath);
                if (UserControll::getInstance()->isUserPasswordConfigWithCryptAvailable()) {
                    // Das Passwort ist in der Crypt vorhanden.
                    UserControll::getInstance()->setConfigDirectory(this->button->getParentItemCast()->getOpenVPN()->configPath);
                    _pwd = UserControll::getInstance()->getUserPasswordPlain();
                } else {
                    // Crypt Datei ist nicht vorhanden, wurde die Anzeige der Auth abgeschaltet
                    if (Settings::getInstance()->getIsShowNoUserAuth()) {
                        // Wenn keine Auth an ist, wird ein leerer String übertragen
                        _pwd = "";
                    } else {
                        // Authentifizierung ist an, nun den Wert des Dialoges nehmen
                        // Gespeichert wurde schon bei save flag vorher,
                        // deswegen kann hier direkt der Wert genommen werden.
                        _pwd = FrmGetUserData::getInstance()->getDataField();
                    }
                }
            } else {
                // Es wird keine Crypt benutzt, eine vorhandene wird ignoriert
                // Wurde die Anzeige der Auth abgeschaltet
                if (Settings::getInstance()->getIsShowNoUserAuth()) {
                    // Wenn keine Auth an ist, wird ein leerer String übertragen
                    _pwd = "";
                } else {
                    // Authentifizierung ist an, nun den Wert des Dialoges nehmen
                    // Gespeichert wurde schon bei save flag vorher,
                    // deswegen kann hier direkt der Wert genommen werden.
                    _pwd = FrmGetUserData::getInstance()->getDataField();
                }
            }
        }
        os << "pneeded->" << _pwd << "\r\n"
           << QDateTime::currentDateTime().toString() << "\n";

        socket->close();
    } else {
        this->serviceNotAvailable();
    }
    this->timer->stop();
    while (socket->state() != QTcpSocket::UnconnectedState) {
        qApp->processEvents();
    }
    this->timer->start(2000);
    if (socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
    }
}

void DataControll::sendPrivKey () {
    ServiceLog::getInstance()->appendToLog(tr("Sending PKCS12 ..."));
    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(QString("127.0.0.1"), this->servicePort);

    if (socket->waitForConnected(1000)) {
        QTextStream os(socket);

        os.setAutoDetectUnicode(true);
        os << "pkneeded->" << FrmGetUserData::getInstance()->getDataField() << "\r\n"
           << QDateTime::currentDateTime().toString() << "\n";

        socket->close();
    } else {
        this->serviceNotAvailable();
    }
    this->timer->stop();
    while (socket->state() != QTcpSocket::UnconnectedState) {
        qApp->processEvents();
    }
    this->timer->start(2000);
    if (socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
    }
}

void DataControll::sendChallengeKey () {
    ServiceLog::getInstance()->appendToLog(tr("Sending Challenge Key ..."));
    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(QString("127.0.0.1"), this->servicePort);

    if (socket->waitForConnected(1000)) {
        QTextStream os(socket);

        os.setAutoDetectUnicode(true);
        os << "ckneeded->" << FrmGetUserData::getInstance()->getDataField() << "\r\n"
           << QDateTime::currentDateTime().toString() << "\n";

        socket->close();
    } else {
        this->serviceNotAvailable();
    }
    this->timer->stop();
    while (socket->state() != QTcpSocket::UnconnectedState) {
        qApp->processEvents();
    }
    this->timer->start(2000);
    if (socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
    }
}

void DataControll::serviceNotAvailable() {    
    if (!Settings::getInstance()->getIsShowNoBallonMessage())
        MainWindowControll::getInstance()->showTrayMessage( QString("Service not available!"));

    QIcon icon = QIcon(":/images/disconnected.png");
    ServiceLog::getInstance()->appendToLog(QString("Service not available!"));
    MainWindowControll::getInstance()->setTrayIcon(icon);

    this->isConnectedService = false;

    if (this->timer->isActive())
        this->timer->stop();
}

void DataControll::openConnectionToService(bool showlog) {

}

QString DataControll::getScript(QString type) {
    QString scriptFilePath = this->configPath + QString("/scripts.conf");
    QString retVal = "";

    QFile scrtiptFile (scriptFilePath);

    if (scrtiptFile.exists()) {        
        // Öffnen und auslesen
        if (!scrtiptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::information(0, QString("Securepoint VPN Client"),
                                QString("Can't read scriptconfig file!"));
             return QString("");
        }
        // Datei offen
        QTextStream sin (&scrtiptFile);

        while (!sin.atEnd()){
            QString line = sin.readLine();
            if (line.left(3) == type.toUpper() + ":") {
                retVal = line.right(line.size()-3);
                break;
            }
        }
        scrtiptFile.close();
    }
    return retVal;
}

void DataControll::runScript(QString type) {
    if (this->getScript(type) != "") {
        ServiceLog::getInstance()->appendToLog(QString ("Starting : ") + type);
        procScripts = new QProcess(this);
        procScripts->execute(this->getScript(type));
        qApp->processEvents() ;
    }
}

void DataControll::startAfterConnectDelayed() {
    this->button->getParentItemCast()->getOpenVPN()->runScript("AC");
}

void DataControll::restartConnection() {
    ServiceLog::getInstance()->appendToLog(tr("Restart Connection ..."));
    this->sendConnect();
}

void DataControll::setTreeButton(TreeButton *but) {
    this->button = but;
}

TreeButton *DataControll::getTreeButton() {
    return this->button;
}

void DataControll::makeDisconnectSingleShoot() {
    this->button->setReadyToConnect();
    QIcon icon = QIcon(":/images/inaktiv.png");
    MainWindowControll::getInstance()->setTrayIcon(icon);
    this->button->getParentItemCast()->getOpenVPN()->setIsConnecting(false);
    this->button->getParentItemCast()->getOpenVPN()->setIsConnected(false);
    this->button->getParentItemCast()->getOpenVPN()->setIsError(false);
    this->button->getParentItemCast()->getOpenVPN()->setErrorString("");
    this->button->getParentItemCast()->getOpenVPN()->setConnectedIP("");
    MainWindowControll::getInstance()->enableTreeButtons();
    this->button->getParentItemCast()->getOpenVPN()->runScript("AD");
}
