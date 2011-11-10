#include "openvpn.h"
#include <windows.h>

OpenVpn::OpenVpn () {
    this->connectionStable = false;
    this->onDisconnect = false;
    this->isLinked = false;
    this->errorHasOccurred = false;
    this->errMessage = "";
    this->onConnect = false;
    this->runAsService = false;
    this->proc = new QProcess(this);
    this->advName = "";
    this->connectedSinceDate = QDate::currentDate();
    this->connectedSinceTime = QTime::currentTime();
    this->isCryptFileAvailable = false;
    this->menu = NULL;
    connect(this, SIGNAL(configSignalIsChanged()), this, SLOT(configIsChanged()));
}

void OpenVpn::openConnect() {
    this->disableMenues();
    this->connectToVpn(false);
    if (Settings::getInstance()->getIsPortableClient())
        emit connectionReconnectFromOpenVPNSig();
}

void OpenVpn::configIsChanged() {
    // Nix
}

QStringList OpenVpn::makeProxyString() {
    QStringList retList;
    QFile pINI (QApplication::applicationDirPath() + QString("/proxy.ini"));
    if (pINI.exists()) {
        QSettings proxIni (QApplication::applicationDirPath() + QString("/proxy.ini"), QSettings::IniFormat);
        if (proxIni.value("proxy-use","").toString() == "CONFIG") {
            // Nothing
        } else if (proxIni.value("proxy-use","").toString() == "IE") {
            // IE
            #ifdef Q_OS_WIN32
                QSettings regIE ("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", QSettings::NativeFormat);
                QString regVal = regIE.value("ProxyServer", "").toString();
                QStringList regData = regVal.split(":");
                if (regData.size() == 2) {
                    retList << QString ("--http-proxy")
                            << regData[0]
                            << regData[1];
                }
            #endif
        } else {
            // MANUAL
            if (proxIni.value("proxy-port","").toString() != "" && proxIni.value("proxy-ip","").toString() != "") {
                if (proxIni.value("proxy-type","").toString() == "HTTP")
                    retList << QString ("--http-proxy")
                            << proxIni.value("proxy-ip","").toString()
                            << proxIni.value("proxy-port","").toString();
                else
                    retList << QString ("--socks-proxy")
                            << proxIni.value("proxy-ip","").toString()
                            << proxIni.value("proxy-port","").toString();
            }
        }
    }
    return retList;
}

QString OpenVpn::getScript(QString type) {
    QString scriptFilePath = this->configPath + QString("/scripts.conf");
    QFile scrtiptFile (scriptFilePath);
    if (scrtiptFile.exists()) {
        // Öffnen und auslesen
        if (!scrtiptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::information(0, QString(tr("Securepoint SSL VPN")),
                                QString(tr("Can't read scriptconfig file!")));
             return QString("");
        }
        // Datei offen
        QTextStream sin (&scrtiptFile);

        while (!sin.atEnd()){
            QString line = sin.readLine();
            if (line.left(3) == type.toUpper() + ":") {
                scrtiptFile.close();
                return line.right(line.size()-3);
            }
        }
        scrtiptFile.close();
        // Keine Daten gefunden
        return QString("");
    } else {
        // Keine Datei da, leeren String zurückgeben
        return QString("");
    }

}

void OpenVpn::runScript(QString type){
    if (this->getScript(type) != "") {
        this->openVpnLogData.append(QString (tr("Starting : ")) + type);
        procScripts = new QProcess(this);
        procScripts->start(this->getScript(type));
        //connect( procScripts, SIGNAL(error ( QProcess::ProcessError) ), this, SLOT(showProcessError (QProcess::ProcessError)));
        qApp->processEvents() ;
    }
}

void OpenVpn::showProcessScriptError(QProcess::ProcessError error) {
    QString errMessage;
    switch (error) {
        case QProcess::FailedToStart:
            errMessage = QString (tr("Run Script:\nThe process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program."));
            break;
        case QProcess::Crashed:
            errMessage = QString (tr("Run Script:\nThe process crashed some time after starting successfully."));
            break;
        case QProcess::Timedout:
            errMessage = QString (tr("Run Script:\nThe last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again."));
            break;
        case QProcess::WriteError:
            errMessage = QString (tr("Run Script:\nAn error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel."));
            break;
        case QProcess::ReadError:
            errMessage = QString (tr("Run Script:\nAn error occurred when attempting to read from the process. For example, the process may not be running."));
            break;
        case QProcess::UnknownError:
            errMessage = QString (tr("Run Script:\nAn unknown error occurred. This is the default return value of error()."));
            break;
        default:
            errMessage = QString (tr("Run Script:\nNo valid error code!"));
            break;
    }

    // Daten ausgeben
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
    msgBox.setText(tr("An error has been occured!"));
    msgBox.setWindowIcon(QIcon(":/images/logo.png"));
    msgBox.setInformativeText(errMessage);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    msgBox.exec();
}

void OpenVpn::connectToVpn(bool openLog){
    this->onDisconnect = false;
    // Connect
    if (!this->isConnectionStable()) {
        this->onConnect = true;
        this->errorHasOccurred = false;
        this->errMessage = "";
        this->openVpnLogData.clear();
        if (openLog)
            if (!mLog.isVisible()) {
                this->openVpnLog();
            }
        this->runScript("BC");
        // Icon auf connecting setzen
        //this->setConnected();
        this->setIcon(Connecting);
        // connect to VPN
        QString cFile;
        QStringList arguments;
        QString program = "./app/bin/openvpn";

        // Pfad für die Config bauen
        cFile = this->configPath + QString("/") + this->configName + QString(".ovpn");
        // Als Argument adden
        arguments << QString ("--service");
        arguments << QString ("openvpngui_exit");
        arguments << QString ("0");
        arguments << QString ("--config");
        arguments << cFile;
        QStringList proxyStr = this->makeProxyString();
        if (proxyStr.length() == 3) {
            arguments << proxyStr[0];
            arguments << proxyStr[1];
            arguments << proxyStr[2];
        }

        if (!Settings::getInstance()->getUseNoInteract()) {
            arguments << QString ("--auth-retry");
            arguments << QString ("interact");
        }

        // Prozesssignale zur Überwachung an die Slots binden
        connect( this->proc, SIGNAL(error ( QProcess::ProcessError) ), this, SLOT(showProcessError (QProcess::ProcessError)));
        connect( this->proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessData()));
        connect( this->proc, SIGNAL(readyReadStandardError()), this, SLOT(readProcessData()));
        connect( this->proc, SIGNAL(finished (int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
        // Programm starten im Config Verzeichnis, sonst findet OpenVPN keine Zertifikate
        this->proc->setWorkingDirectory(this->configPath + QString("/"));
        this->proc->start(program, arguments);
   } else {
        // Connection already stable
          QMessageBox::information(0, QString(tr("Securepoint SSL VPN")),
                                QString(tr("Already connected to this network")));
   }
}

void OpenVpn::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (!this->onDisconnect) {
        this->connectionStable = false;
        this->setIcon(Error);
        this->enableAllMenus();
        emit errorOccuredSig("1025");
    }

}

void OpenVpn::showTrayMessage(QString message) {
    MainWindowControll::getInstance()->showTrayMessage(message);
}

void OpenVpn::setDisconnected() {
    this->onConnect = false;
}

void OpenVpn::setIcon(int index)
{
    QIcon icon;
    switch (index) {
        case Inaktiv:
            icon = QIcon(":/images/inaktiv.png");
            break;
        case Connected:
            icon = QIcon(":/images/connected.png");
            break;
        case Error:
            icon = QIcon(":/images/disconnected.png");
            break;
        case Connecting:
            icon = QIcon(":/images/connecting.png");
            break;
        default:
            icon = QIcon(":/images/inaktiv.png");
            break;
    }
    MainWindowControll::getInstance()->setTrayIcon(icon);
}

bool OpenVpn::isConnectionStable () {
    return this->connectionStable;
}

void OpenVpn::setObjectToConnected() {
    this->setIcon(Connected);
}

void OpenVpn::showProcessError(QProcess::ProcessError error) {
    QString errMessage;
    switch (error) {
        case QProcess::FailedToStart:
            errMessage = QString (tr("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program."));
            break;
        case QProcess::Crashed:
            errMessage = QString (tr("The process crashed some time after starting successfully."));
            break;
        case QProcess::Timedout:
            errMessage = QString (tr("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again."));
            break;
        case QProcess::WriteError:
            errMessage = QString (tr("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel."));
            break;
        case QProcess::ReadError:
            errMessage = QString (tr("An error occurred when attempting to read from the process. For example, the process may not be running."));
            break;
        case QProcess::UnknownError:
            errMessage = QString (tr("An unknown error occurred. This is the default return value of error()."));
            break;
        default:
            errMessage = QString (tr("No valid error code!"));
            break;
    }
    if (!this->onDisconnect) {
        // Log schreiben
        this->runScript("EC");
        this->openVpnLogData.append(errMessage);
        this->errorHasOccurred = true;
        this->errMessage = errMessage;
        this->enableAllMenus();
        emit errorOccuredSig(errMessage);
        // Daten ausgeben
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("An error has been occured!"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(errMessage);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
    }
    this->onConnect = false;
}

void OpenVpn::disconnectVpn() {
    // Skript before Disconnect
    this->runScript("BD");
    this->onDisconnect = true;
    // Process abschiessen
    HANDLE exitEvent;
    QString eventName = "openvpngui_exit";
    exitEvent = CreateEvent(NULL, TRUE, FALSE, (LPCTSTR)eventName.utf16());
    if (!exitEvent) {
        return;
    }
    SetEvent(exitEvent);
    CloseHandle (exitEvent);
    // Progess sollte beendet sein
    this->connectionStable = false;
    // Tray Icon setzen
    this->setIcon(Inaktiv);
    this->setDisconnected();
    this->showTrayMessage(QString(tr("Disconnected from VPN.\nName: ")) + this->configName);
    // Skript after Connect ausführen
    this->runScript("AD");
    // Signal für den Dialog werfen
    this->enableAllMenus();
    emit connectionIsDisconnectedSig();
}

void OpenVpn::startAfterConnectDelayed() {
    // Diese Funktion wird von einem
    // Singleshot timer aufgerufen, um
    // sicherstellen, dass die IP-Adreese
    // zugewiesen ist.
    this->runScript("AC");
}

void OpenVpn::readProcessData() {
    QByteArray line;
    QString output;
    bool showLine = true;
    line = this->proc->readAllStandardError();
    if (line == "")
        line = this->proc->readAllStandardOutput();
    if (line != "") {
        //Enter Private Key Password:
        QString pkkey (line);
        if (pkkey.contains("Enter Private Key Password:", Qt::CaseInsensitive)) {
            if (this->proc->isWritable()) {
                this->isUserDataAvailable = false;
                disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                FrmGetUserData::getInstance()->setFrmType(3);
                FrmGetUserData::getInstance()->show();
                while (!this->isUserDataAvailable) {
                    FrmGetUserData::getInstance()->exec();
                    QApplication::processEvents();
                }
                // Daten da, weiter ...
                QByteArray ba;
                ba.append(FrmGetUserData::getInstance()->getDataField() + "\n");
                this->proc->write(ba);
                showLine = false;
            }

        }
        // Enter Username?
        if (line == "Enter Auth Username:" || line == "Enter Challenge Username:") {
            if (this->proc->isWritable()) {
                QString user = "";
                if (Settings::getInstance()->getIsStartWithUser()) {
                    // User wurde von Kommandozeile übergeben
                    user = Settings::getInstance()->getStartWithUser();
                } else {
                    // Es wurde kein User übergeben nun die Art ermitteln,
                    // woher der User kommt
                    if (Settings::getInstance()->getUseCryptedData()) {
                        // Crypt ist an
                        // Ist eine Crpyt da
                        UserControll::getInstance()->setConfigDirectory(this->configPath);
                        if (!UserControll::getInstance()->isUserNameConfigWithCryptAvailable()) {
                            // Crypt ist nicht vorhanden
                            // Usernamen holen
                            // Wurde die noAuth option gesetzt
                            if (Settings::getInstance()->getIsShowNoUserAuth()) {
                                // Keine Auth
                                user = "";
                            } else {
                                // Auth ist gesetzt
                                this->isUserDataAvailable = false;
                                disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                FrmGetUserData::getInstance()->setFrmType(0);
                                FrmGetUserData::getInstance()->show();
                                while (!this->isUserDataAvailable) {
                                    FrmGetUserData::getInstance()->exec();
                                    QApplication::processEvents();
                                }
                                // Daten sind da, speichern?
                                user = FrmGetUserData::getInstance()->getDataField();
                                // Nun kann gespeichert werden
                                if (FrmGetUserData::getInstance()->isSaveChecked()) {
                                    // Daten sollen gespeichert werden
                                    // Private key schon bekannt oder force an?
                                    if (UserControll::getInstance()->getPrivateKeyPub() == "" || Settings::getInstance()->getIsForcePrivateKey()) {
                                        // Private Ke ermitteln
                                        this->isUserDataAvailable = false;
                                        disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                        connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                        FrmGetUserData::getInstance()->setFrmType(4);
                                        FrmGetUserData::getInstance()->show();
                                        while (!this->isUserDataAvailable) {
                                            FrmGetUserData::getInstance()->exec();
                                            QApplication::processEvents();
                                        }
                                        // Key in der Klasse setzen und für den Rest der Anwedung merken, ausser bei force key
                                        UserControll::getInstance()->setPrivateKey(FrmGetUserData::getInstance()->getDataField());
                                        // Nun Speichern
                                    }
                                    UserControll::getInstance()->setConfigDirectory(this->configPath);
                                    UserControll::getInstance()->setUserNameCrypt(user);
                                    this->isCryptFileAvailable = true;
                                    MainWindowControll::getInstance()->refreshDialog();
                                }
                            }
                        } else {
                            // Crypt ist da, priv key auch?
                            if (UserControll::getInstance()->getPrivateKeyPub() == "" || Settings::getInstance()->getIsForcePrivateKey()) {
                                // Key ist nicht da, oder es wird force key verwendet
                                this->isUserDataAvailable = false;
                                disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                FrmGetUserData::getInstance()->setFrmType(4);
                                FrmGetUserData::getInstance()->show();
                                while (!this->isUserDataAvailable) {
                                    FrmGetUserData::getInstance()->exec();
                                    QApplication::processEvents();
                                }
                                // Key in der Klasse setzen und für den Rest der Anwedung merken, ausser bei force key
                                UserControll::getInstance()->setPrivateKey(FrmGetUserData::getInstance()->getDataField());
                            }
                            // Alles da user holen
                            user = UserControll::getInstance()->getUserNamePlain();
                        }
                    } else {
                        // Keine Crypt Daten verwenden
                        // Ist die Auth Anzeige an?
                        if (!Settings::getInstance()->getIsShowNoUserAuth()) {
                            // Auth Anzeige ist an
                            this->isUserDataAvailable = false;
                            disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                            connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                            FrmGetUserData::getInstance()->setFrmType(0);
                            FrmGetUserData::getInstance()->show();
                            while (!this->isUserDataAvailable) {
                                FrmGetUserData::getInstance()->exec();
                                QApplication::processEvents();
                            }
                            // Daten da
                            user = FrmGetUserData::getInstance()->getDataField();
                        } else {
                            // Es gibt keine Authentifizierung
                            user = "";
                        }
                    }
                }
                QByteArray ba;
                ba.append(user + "\n");
                this->proc->write(ba);
                showLine = false;
            }

        }
        // Enter Password
        if (line == "Enter Auth Password:") {
            if (this->proc->isWritable()) {
                QString pwd = "";
                if (Settings::getInstance()->getIsStartWithPwd()) {
                    // User wurde von Kommandozeile übergeben
                    pwd = Settings::getInstance()->getStartWithPwdPwd();
                } else {
                    // Es wurde kein User übergeben nun die Art ermitteln,
                    // woher der User kommt
                    if (Settings::getInstance()->getUseCryptedData()) {
                        // Crypt ist an
                        // Ist eine Crpyt da
                        UserControll::getInstance()->setConfigDirectory(this->configPath);
                        if (!UserControll::getInstance()->isUserPasswordConfigWithCryptAvailable()) {
                            // Crypt ist nicht vorhanden
                            // Usernamen holen
                            // Wurde die noAuth option gesetzt
                            if (Settings::getInstance()->getIsShowNoUserAuth()) {
                                // Keine Auth
                                pwd = "";
                            } else {
                                // Auth ist gesetzt
                                this->isUserDataAvailable = false;
                                disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                FrmGetUserData::getInstance()->setFrmType(1);
                                FrmGetUserData::getInstance()->show();
                                while (!this->isUserDataAvailable) {
                                    FrmGetUserData::getInstance()->exec();
                                    QApplication::processEvents();
                                }
                                // Daten sind da, speichern?
                                pwd = FrmGetUserData::getInstance()->getDataField();
                                // Nun kann gespeichert werden
                                if (FrmGetUserData::getInstance()->isSaveChecked()) {
                                    // Daten sollen gespeichert werden
                                    // Private key schon bekannt oder force an?
                                    if (UserControll::getInstance()->getPrivateKeyPub() == "" || Settings::getInstance()->getIsForcePrivateKey()) {
                                        // Private Ke ermitteln
                                        this->isUserDataAvailable = false;
                                        disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                        connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                        FrmGetUserData::getInstance()->setFrmType(4);
                                        FrmGetUserData::getInstance()->show();
                                        while (!this->isUserDataAvailable) {
                                            FrmGetUserData::getInstance()->exec();
                                            QApplication::processEvents();
                                        }
                                        // Key in der Klasse setzen und für den Rest der Anwedung merken, ausser bei force key
                                        UserControll::getInstance()->setPrivateKey(FrmGetUserData::getInstance()->getDataField());
                                        // Nun Speichern
                                    }
                                    UserControll::getInstance()->setConfigDirectory(this->configPath);
                                    UserControll::getInstance()->setUserPasswordCrypt(pwd);
                                    this->isCryptFileAvailable = true;
                                    MainWindowControll::getInstance()->refreshDialog();
                                }
                            }
                        } else {
                            // Crypt ist da, priv key auch?
                            if (UserControll::getInstance()->getPrivateKeyPub() == "" || Settings::getInstance()->getIsForcePrivateKey()) {
                                // Key ist nicht da, oder es wird force key verwendet
                                this->isUserDataAvailable = false;
                                disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                                connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                                FrmGetUserData::getInstance()->setFrmType(4);
                                FrmGetUserData::getInstance()->show();
                                while (!this->isUserDataAvailable) {
                                    FrmGetUserData::getInstance()->exec();
                                    QApplication::processEvents();
                                }
                                // Key in der Klasse setzen und für den Rest der Anwedung merken, ausser bei force key
                                UserControll::getInstance()->setPrivateKey(FrmGetUserData::getInstance()->getDataField());
                            }
                            // Alles da user holen
                            pwd = UserControll::getInstance()->getUserPasswordPlain();
                        }
                    } else {
                        // Keine Crypt Daten verwenden
                        // Ist die Auth Anzeige an?
                        if (!Settings::getInstance()->getIsShowNoUserAuth()) {
                            // Auth Anzeige ist an
                            this->isUserDataAvailable = false;
                            disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                            connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                            FrmGetUserData::getInstance()->setFrmType(1);
                            FrmGetUserData::getInstance()->show();
                            while (!this->isUserDataAvailable) {
                                FrmGetUserData::getInstance()->exec();
                                QApplication::processEvents();
                            }
                            // Daten da
                            pwd = FrmGetUserData::getInstance()->getDataField();
                        } else {
                            // Es gibt keine Authentifizierung
                            pwd = "";
                        }
                    }
                }
                QByteArray ba;
                ba.append(pwd + "\n");
                this->proc->write(ba);
                showLine = false;
            }
        }
        // Enter Challenge Password
        if (line == "Enter Challenge Password:") {
            if (this->proc->isWritable()) {
                //
                QString pwd = "";
                this->isUserDataAvailable = false;
                disconnect (FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), 0, 0);
                connect(FrmGetUserData::getInstance(), SIGNAL(dataIsAvailable()), this, SLOT(userDataIsAvailable()));
                FrmGetUserData::getInstance()->setFrmType(2);
                FrmGetUserData::getInstance()->show();
                while (!this->isUserDataAvailable) {
                    FrmGetUserData::getInstance()->exec();
                    QApplication::processEvents();
                }
                // Daten sind da, speichern?
                pwd = FrmGetUserData::getInstance()->getDataField();
                QByteArray ba;
                ba.append(pwd + "\n");
                this->proc->write(ba);
                showLine = false;
            }
        }
        // QByteArray in QString
        QString lineOut(line);
        // Verbindung da?
        if (lineOut.contains("Notified TAP-Win32 driver to set a DHCP IP", Qt::CaseInsensitive)) {
            // IP Suchen und speichern
            QString connIP = lineOut.mid(lineOut.indexOf("Notified TAP-Win32 driver to set a DHCP IP")+54,15);
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

            // Meldung zeigen connected
            this->connectionIP = connIP;
            this->showTrayMessage(QString(tr("Connnected to VPN.\nName: ")) + this->configName + QString(tr("\nIP: ")) + this->connectionIP);
            // Status speichern und Tray Icon setzen
            this->connectedSinceDate = QDate::currentDate();
            this->connectedSinceTime = QTime::currentTime();
            this->onConnect = false;
            this->connectionStable = true;
            emit connectionIsStableSig(this->connectionIP);
            this->setIcon(Connected);
            QString timeOut = this->getScript("TO");
            int scriptDelay = 0;
            if (timeOut != "") {
                scriptDelay = timeOut.trimmed().toInt();
            }
            QTimer::singleShot(scriptDelay, this, SLOT(startAfterConnectDelayed()));
        }


        // Fehler abfangen
        bool errorOcurred = false;
        bool _tlsHandshakeFailed (false);
        QString errorMessage = "";
        //"All TAP-Win32 adapters on this system are currently in use"
        if (lineOut.contains("All TAP-Win32 adapters on this system are currently in use", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("All TAP-Win32 adapters on this system are currently in use"));
            errorOcurred = true;
        } else if (lineOut.contains("TLS Error: Need PEM pass phrase for private key", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("TLS Error: Need PEM pass phrase for private key"));
            errorOcurred = true;
        } else if (lineOut.contains("TLS Error: TLS handshake failed", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("TLS error! See log for details"));
            errorOcurred = true;
            _tlsHandshakeFailed = true;
        } else if (lineOut.contains("EVP_DecryptFinal:bad decrypt", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("EVP_DecryptFinal:bad decrypt"));
            errorOcurred = true;
        } else if (lineOut.contains("RESOLVE: Cannot resolve host address:", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("Connection error! See log for details"));
            errorOcurred = true;
            _tlsHandshakeFailed = true;
        } else if (lineOut.contains("PKCS12_parse:mac verify failure", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("PKCS12_parse:mac verify failure"));
            errorOcurred = true;
        } else if (lineOut.contains("Received AUTH_FAILED control message", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("Received AUTH_FAILED control message"));
            errorOcurred = true;
        } else if (lineOut.contains("Auth username is empty", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("Auth username is empty"));
            errorOcurred = true;
        } else if (lineOut.contains("error=certificate has expired", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("error=certificate has expired"));
            errorOcurred = true;
        } else if (lineOut.contains("error=certificate is not yet valid", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("error=certificate is not yet valid"));
            errorOcurred = true;
        } else if (lineOut.contains("Cannot load certificate file", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("Cannot load certificate file"));
            errorOcurred = true;
        } else if (lineOut.contains("Exiting", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("Application Exiting!"));
            errorOcurred = true;
        } else if (lineOut.contains("Use --help for more information.", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("OpenVPN parameter error! See log for details"));
            errorOcurred = true;
        } else if (lineOut.contains("will try again in 5 seconds", Qt::CaseInsensitive)) {
            errorMessage = QString (tr("OpenVPN connection error! See log for details"));
            //errorOcurred = true;
        }

        // Fehler durch normalen Disconnect verhindern
        if (lineOut.contains("SIGTERM[hard,] received, process exiting", Qt::CaseInsensitive))
            errorOcurred = false;

        if (errorOcurred) {
            this->runScript("EC");
            this->showTrayMessage(errorMessage);
            this->connectionStable = false;
            this->setIcon(Error);
            this->setDisconnected();
            this->errorHasOccurred = true;
            this->errMessage = errorMessage;
            this->enableAllMenus();
            emit errorOccuredSig(errorMessage);
        }

        if (lineOut.contains("Restart pause", Qt::CaseInsensitive)) {            
            // Bei Restart Pause befinden wir uns immer noch im Connect auch wenn vorher ein Fehler aufgetreten ist!
            if (!_tlsHandshakeFailed) {
                this->setIcon(Connecting);
                this->disableMenues();
                emit connectionReconnectFromOpenVPNSig ();
            }
        }
        //Initialization Sequence Completed
        if (lineOut.contains("Initialization Sequence Completed", Qt::CaseInsensitive)) {
            this->onConnect = false;
            this->connectionStable = true;
            this->disableMenues();
            emit connectionIsStableSig(this->connectionIP);
            this->setIcon(Connected);
        }


        // Output ins Log?
        if (showLine) {
            output = QString::fromLatin1(line);
            this->openVpnLogData.append(output);
        }
    }

}

void OpenVpn::openVpnLog() {
    mLog.logList = &this->openVpnLogData;
    mLog.showDialog();
}

QString OpenVpn::getConnectionIP() {
    return this->connectionIP;
}

bool OpenVpn::isConnecting() {
    return this->onConnect;
}

bool OpenVpn::isError() {
    return this->errorHasOccurred;
}

QString OpenVpn::getErrorString() {
    return this->errMessage;
}

void OpenVpn::userDataIsAvailable() {
    this->isUserDataAvailable = true;
}

void OpenVpn::setConnectedIP(QString ip) {
    this->connectionIP = ip;
}

void OpenVpn::setIsConnected(bool flag) {
    this->connectionStable = flag;
}

void OpenVpn::setIsError(bool flag) {
    this->errorHasOccurred = flag;
}

void OpenVpn::setErrorString(QString errMes) {
    this->errMessage = errMes;
}

void OpenVpn::setIsConnecting(bool flag) {
    this->onConnect = flag;
}

bool OpenVpn::isOnConnect() {
    return this->onConnect;
}

void OpenVpn::setAdvName(QString name) {
    this->advName = name;
}

QString OpenVpn::getAdvName() {
    return this->advName;
}

void OpenVpn::setIsCryptFileAvailable(bool flag) {
    this->isCryptFileAvailable = flag;
}

bool OpenVpn::getIsCryptFileAvailable() {
    return this->isCryptFileAvailable;
}

void OpenVpn::setConnectedSinceDate(QDate since) {
    this->connectedSinceDate = since;
}

void OpenVpn::setConnectedSinceTime(QTime since) {
    this->connectedSinceTime = since;
}

QDate OpenVpn::getConnectedSinceDate() {
    return this->connectedSinceDate;
}

QTime OpenVpn::getConnectedSinceTime() {
    return this->connectedSinceTime;
}

void OpenVpn::enableAllMenus() {
    if (Settings::getInstance()->getIsPortableClient()) {
        foreach (QAction *act, MainWindowControll::getInstance()->getSystray()->actions()) {
            if (act->menu() != NULL && !act->menu()->actions().isEmpty()) {
                if (act == this->menu) {
                    foreach (QAction *subact, act->menu()->actions()) {
                        if (subact->objectName() == "CONNECT") {
                            subact->setEnabled(true);
                        } else if (subact->objectName() == "DISCONNECT") {
                            subact->setEnabled(false);
                        }
                    }
                }
                act->menu()->setEnabled(true);
            }
        }
        FrmGetUserData::getInstance()->closeMe();
    }
}

void OpenVpn::disableMenues() {
    if (Settings::getInstance()->getIsPortableClient()) {
        foreach (QAction *act, MainWindowControll::getInstance()->getSystray()->actions()) {
            if (act->menu() != NULL && !act->menu()->actions().isEmpty()) {
                if (act != this->menu) {
                    act->menu()->setDisabled(true);
                } else {
                    foreach (QAction *subact, act->menu()->actions()) {
                        if (subact->objectName() == "CONNECT") {
                            subact->setEnabled(false);
                        } else if (subact->objectName() == "DISCONNECT") {
                            subact->setEnabled(true);
                        }
                    }
                }
            }
        }
    }
}

void OpenVpn::openEditConfig() {
    EditConfig::getInstance()->setPath(this->configPath + "/" + this->configName + ".ovpn");
    EditConfig::getInstance()->show();
}

void OpenVpn::openExport() {
    ConfigExport::getInstance()->setConfigPath(this->configPath + "/" + this->configName + ".ovpn");
    ConfigExport::getInstance()->show();
}
