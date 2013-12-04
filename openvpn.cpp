#include "openvpn.h"
#include <windows.h>

#include "preferences.h"

#include "editconfig.h"
#include "configexport.h"
#include "manageconnection.h"

#include "message.h"
#include "servicelogdata.h"

#include "frmgetuserdata.h"

#include "tapdriver.h"
#include "crypt.h"
#include "debug.h"

OpenVpn::OpenVpn (Database *database)
{
    this->connectionStable = false;
    this->onDisconnect = false;
    this->errorHasOccurred = false;
    this->errMessage = "";
    this->onConnect = false;
    this->runAsService = false;
    this->connectedSinceDate = QDate::currentDate();
    this->connectedSinceTime = QTime::currentTime();
    this->menu = NULL;
    this->_id = -1;
    this->waitForCryptKey = false;
    this->isStartConfigValue = false;

    this->db = database;
}

int OpenVpn::id() const
{
    return this->_id;
}

void OpenVpn::setId(int id)
{
    this->_id = id;
}

void OpenVpn::openConnect()
{
    this->disableMenues();
    this->connectToVpn(false);
    if (Settings::getInstance()->getIsPortableClient()) {
        emit connectionReconnectFromOpenVPNSig();
    }
}

QStringList OpenVpn::makeProxyString()
{
    QStringList retList;
    QFile pINI (Settings::getInstance()->getProxyIniPath());
    if (pINI.exists()) {
        QSettings proxIni (Settings::getInstance()->getProxyIniPath(), QSettings::IniFormat);
        if (proxIni.value(QLatin1String("proxy-use"), QLatin1String("")).toString() == QLatin1String("CONFIG")) {
            // Nothing
        } else if (proxIni.value(QLatin1String("proxy-use"), QLatin1String("")).toString() == QLatin1String("IE")) {
            // IE
#ifdef Q_OS_WIN32
                QSettings regIE (QLatin1String("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), QSettings::NativeFormat);
                QString regVal = regIE.value(QLatin1String("ProxyServer"), QLatin1String("")).toString();
                QStringList regData = regVal.split(":");
                if (regData.size() == 2) {
                    retList << QLatin1String ("--http-proxy")
                            << regData[0]
                            << regData[1];
                }
#endif
        } else {
            // MANUAL
            if (proxIni.value(QLatin1String("proxy-port"), QLatin1String("")).toString() != "" && proxIni.value(QLatin1String("proxy-ip"), QLatin1String("")).toString() != "") {
                if (proxIni.value(QLatin1String("proxy-type"), QLatin1String("")).toString() == QLatin1String("HTTP")) {
                    retList << QLatin1String ("--http-proxy")
                            << proxIni.value(QLatin1String("proxy-ip"), QLatin1String("")).toString()
                            << proxIni.value(QLatin1String("proxy-port"), QLatin1String("")).toString();
                } else {
                    retList << QLatin1String ("--socks-proxy")
                            << proxIni.value(QLatin1String("proxy-ip"), QLatin1String("")).toString()
                            << proxIni.value(QLatin1String("proxy-port"), QLatin1String("")).toString();
                }
            }
        }
    }

    return retList;
}

QString OpenVpn::getScript(const QString &type)
{
    QFile scrtiptFile (this->getConfigDirectory() + QLatin1String("/scripts.conf"));

    if (scrtiptFile.exists()) {
        // Öffnen und auslesen
        if (!scrtiptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            Message::error(QObject::tr("Can't read scriptconfig file!"));
            return QLatin1String ("");
        }

        // Datei offen
        QTextStream sin (&scrtiptFile);
        while (!sin.atEnd()){
            QString line (sin.readLine());
            if (line.trimmed().left(3).toUpper() == type.toUpper() + QLatin1String(":")) {
                scrtiptFile.close();
                return line.right(line.size() - 3);
            }
        }
        scrtiptFile.close();
    }

    return QLatin1String("");
}

QString OpenVpn::getConfigPath() const
{
    return this->configPath;
}

QString OpenVpn::getConfigName() const
{
    return this->configName;
}

bool OpenVpn::isConnectionStable() const
{
    return this->connectionStable;
}

void OpenVpn::setConfigStable(const bool &flag)
{
    this->connectionStable = flag;
}

void OpenVpn::setConfigName(const QString &name)
{
    this->configName = name;
}

void OpenVpn::setConfigPath(const QString &path)
{
    this->configPath = path;
}

void OpenVpn::runScript(const QString &type)
{
    if (!this->getScript(type).isEmpty()) {
        QString logMessage (QObject::tr("Starting : ") + type);
        if (Settings::getInstance()->getIsPortableClient()) {
            this->openVpnLogData.append(logMessage);
        } else {
            // Sind im Dienst modus
            ServiceLogData::instance()->append(this->id(), logMessage);
        }

        procScripts = new QProcess(this);
        QObject::connect(this->procScripts, SIGNAL(error(QProcess::ProcessError)), this, SLOT(showProcessScriptError(QProcess::ProcessError)));
        QObject::connect(this->procScripts, SIGNAL(finished(int,QProcess::ExitStatus)), this->procScripts, SLOT(deleteLater()));

        procScripts->start(this->getScript(type));
        qApp->processEvents() ;
    }
}

void OpenVpn::showProcessScriptError(QProcess::ProcessError error)
{
    QString errMessage;
    switch (error) {
        case QProcess::FailedToStart:
            errMessage = QObject::tr("Run Script:\nThe process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
            break;
        case QProcess::Crashed:
            errMessage = QObject::tr("Run Script:\nThe process crashed some time after starting successfully.");
            break;
        case QProcess::Timedout:
            errMessage = QObject::tr("Run Script:\nThe last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.");
            break;
        case QProcess::WriteError:
            errMessage = QObject::tr("Run Script:\nAn error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.");
            break;
        case QProcess::ReadError:
            errMessage = QObject::tr("Run Script:\nAn error occurred when attempting to read from the process. For example, the process may not be running.");
            break;
        case QProcess::UnknownError:
            errMessage = QObject::tr("Run Script:\nAn unknown error occurred. This is the default return value of error().");
            break;
        default:
            errMessage = QObject::tr("Run Script:\nNo valid error code!");
            break;
    }

    // Daten ausgeben
    Message::error(errMessage);
    if (!Settings::getInstance()->getIsPortableClient()) {
        ServiceLogData::instance()->append(this->id(), errMessage);
    } else {
        this->openVpnLogData.append(errMessage);
    }
}

QString OpenVpn::getConfigFullPath() const
{
    return this->configPath;
}

void OpenVpn::connectToVpn(bool openLog)
{
    //
    // Die OpenVpn.exe mit der Config starten, um die Verbindung aufzubauen
    //

    // Haben wir eine ID?
    if (this->id() == -1) {
        // Das hier dürfte nie passieren!
        return;
    }

    this->onConnect = true;
    this->errorHasOccurred = false;
    this->errMessage.clear();
    this->openVpnLogData.clear();

    if (openLog) {
        if (!mLog.isVisible()) {
            this->openVpnLog();
        }
    }

    this->runScript("BC");
    // Icon setzen
    Preferences::instance()->setIcon();

    // Pfad für die Config bauen
    QString cFile (this->configPath);

    // Die Parameter für OpenVpn bauen
    QStringList arguments;
    arguments << QString ("--service");
    arguments << QString ("openvpngui_exit_%1").arg(this->id());
    arguments << QString ("0");
    arguments << QString ("--config");
    arguments << cFile;

    QStringList proxyStr = this->makeProxyString();
    if (proxyStr.length() == 3) {
        arguments << proxyStr[0];
        arguments << proxyStr[1];
        arguments << proxyStr[2];
    }

    // Interact setzen -> wegen OTP
    if (!Settings::getInstance()->getUseNoInteract()) {
        arguments << QString ("--auth-retry");
        arguments << QString ("interact");
    }

    // Prozesssignale zur Überwachung an die Slots binden
    if (!QObject::connect(&this->proc, SIGNAL(error ( QProcess::ProcessError)), this, SLOT(showProcessError (QProcess::ProcessError)))) {
        qDebug() << QLatin1String("OpenVPN: Can't connect process error signal");
    }

    if(!QObject::connect(&this->proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessData()))) {
        qDebug() << QLatin1String("OpenVPN: Can't connect read std signal");
    }

    if(!QObject::connect(&this->proc, SIGNAL(readyReadStandardError()), this, SLOT(readProcessData()))) {
        qDebug() << QLatin1String("OpenVPN: Can't connect read err signal");
    }

    if (!QObject::connect(&this->proc, SIGNAL(finished (int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)))) {
        qDebug() << QLatin1String("OpenVPN: Can't connect finished signal");
    }

    // Programm starten im Config Verzeichnis, sonst findet OpenVPN keine Zertifikate
    this->proc.setWorkingDirectory(this->getConfigDirectory());

    QString arch ("x32");
    if (Check64::isRunning64Bit()) {
        arch = QLatin1String("x64");
    }

    QString program (QCoreApplication::applicationDirPath() + QString("/bin/%1/openvpn.exe").arg(arch));

    // Ist die exe da
    if (!QFile::exists(program)) {
        return;
    }

    Debug::log(QString("OpenVpn: Open Connection. \nFile path: %1 \nParams: %2")
               .arg(program)
               .arg(arguments.join(" ")));

    // Programm starten
    this->proc.start(program, arguments);
    //
    if (!this->proc.waitForStarted()) {
        qDebug() << this->proc.errorString();
    }
}

void OpenVpn::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED (exitCode)
    Q_UNUSED(exitStatus)

    if (!this->onDisconnect) {
        this->connectionStable = false;
        this->enableAllMenus();
        emit errorOccuredSig("1025");
    }
    Preferences::instance()->setIcon();
}

void OpenVpn::showTrayMessage(const QString &message)
{
    Preferences::instance()->showTrayMessageChecked(message, QSystemTrayIcon::Information);
}

void OpenVpn::setDisconnected()
{
    this->onConnect = false;
}

bool OpenVpn::isConnectionStable ()
{
    return this->connectionStable;
}

void OpenVpn::setObjectToConnected()
{
    Preferences::instance()->setIcon();
}

void OpenVpn::showProcessError(QProcess::ProcessError error)
{
    QString errMessage;
    switch (error) {
        case QProcess::FailedToStart:
            errMessage = QObject::tr("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
            break;
        case QProcess::Crashed:
            errMessage = QObject::tr("The process crashed some time after starting successfully.");
            break;
        case QProcess::Timedout:
            errMessage = QObject::tr("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.");
            break;
        case QProcess::WriteError:
            errMessage = QObject::tr("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.");
            break;
        case QProcess::ReadError:
            errMessage = QObject::tr("An error occurred when attempting to read from the process. For example, the process may not be running.");
            break;
        case QProcess::UnknownError:
            errMessage = QObject::tr("An unknown error occurred. This is the default return value of error().");
            break;
        default:
            errMessage = QObject::tr("No valid error code!");
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
        Message::error(errMessage);
    }

    this->onConnect = false;
}

void OpenVpn::disconnectVpn()
{
    // OpenVpn beenden
    QObject::disconnect(&this->proc, 0, 0, 0);

    this->runScript("BD");
    this->onDisconnect = true;

    this->connectionStable = false;

    // Offende Dialoge schließen
    emit closeDialog();

    // Per Winapi killen
    HANDLE exitEvent;
    QString eventName (QLatin1String("openvpngui_exit_") + QString::number(this->id()));
    exitEvent = CreateEvent(NULL, TRUE, FALSE, (LPCTSTR)eventName.utf16());
    if (!exitEvent) {
        return;
    }
    SetEvent(exitEvent);
    CloseHandle (exitEvent);

    if(!this->proc.waitForFinished(2000)) {
        this->proc.kill();
        this->proc.waitForFinished(3000);
    }

    this->connectionStable = false;
    this->onConnect = false;

    this->showTrayMessage(QObject::tr("Disconnected from VPN.\nName: ") + this->configName);
    // Skript after Connect ausführen
    this->runScript("AD");
    // Signal für den Dialog werfen
    this->enableAllMenus();
    emit connectionIsDisconnectedSig();
}

void OpenVpn::runDelayStartScript()
{
    QString timeOut (this->getScript("TO"));
    int scriptDelay (0);
    // Ist ein Timeout da?
    if (timeOut.isEmpty()) {
        // Nein, Standard sind 2 Sekunden
        timeOut = QLatin1String ("2000");
    }
    //
    scriptDelay = timeOut.trimmed().toInt();

    QTimer::singleShot(scriptDelay, this, SLOT(startAfterConnectDelayed()));
}

void OpenVpn::startAfterConnectDelayed()
{
    // Diese Funktion wird von einem
    // Singleshot timer aufgerufen, um
    // sicherstellen, dass die IP-Adreese
    // zugewiesen ist.
    this->runScript("AC");
}

void OpenVpn::writeUserData(QString data)
{
    if (this->proc.isWritable()) {
        QByteArray ba;
        ba.append(data + "\n");
        this->proc.write(ba);
    }
}

void OpenVpn::userDataIsNeeded (int type) {
    InputType::UserInputType ntype;
    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key für Crypted User Data
    // 5 - Http Prxy user
    // 6 - Http Prxy pass
    switch (type) {
        case 0:
            ntype = InputType::Username;
            break;
        case 1:
            ntype = InputType::Password;
            break;
        case 2:
            ntype = InputType::Otp;
            break;
        case 3:
            ntype = InputType::Pkcs12;
            break;
        case 5:
            ntype = InputType::HttpUsername;
            break;
        case 6:
            ntype = InputType::HttpPassword;
            break;
        default:
            ntype = InputType::Username;
    }

    if (this->hasCrediantials(type)) {
        // Daten einlesen und schreiben
        this->writeUserData(this->getSavedUserData(type));
        return;
    }

    FrmGetUserData dialog (ntype, this->configName, this->id());
    QObject::connect(&dialog, SIGNAL(writeUserData(QString)), this, SLOT(writeUserData(QString)));
    // Sollen die Daten gespeichert werden
    QObject::connect(&dialog, SIGNAL(saveUserData(int,int,QString,bool)), this, SLOT(saveUserData(int,int,QString,bool)));
    // Wenn über das Kontextmenu disconnected wird, kann es sein das ein Dialog offen ist,
    // den Dialog nun schließen
    QObject::connect(this, SIGNAL(closeDialog()), &dialog, SLOT(receivedCloseMe()));

    dialog.exec();
}

void OpenVpn::readProcessData()
{
    QByteArray line;
    QString output;
    bool showLine (true);

    line = this->proc.readAllStandardError();
    if (line == "") {
        line = this->proc.readAllStandardOutput();
    }

    if (line != "") {
        //Enter Private Key Password:
        // 0 - Username
        // 1 - Pwd
        // 2 - OTP
        // 3 - PKCS12
        // 4 - Private Key für Crypted User Data
        QString pkkey (line);
        if (pkkey.contains("Enter Private Key Password:", Qt::CaseInsensitive)) {
            this->userDataIsNeeded(4);
            showLine = false;
        }

        // Enter Username?
        if (line == "Enter Auth Username:" || line == "Enter Challenge Username:") {
            this->userDataIsNeeded(0);
            showLine = false;
        }

        // Enter Password
        if (line == "Enter Auth Password:") {
            this->userDataIsNeeded(1);
            showLine = false;
        }
        // Enter Challenge Password
        if (line == "Enter Challenge Password:" || line.contains("CHALLENGE: Please enter token PIN")) {
            this->userDataIsNeeded(2);
            showLine = false;
        }

        // Http user
        if (line.contains("Enter HTTP Proxy Username:")) {
            this->userDataIsNeeded(5);
            showLine = false;
        }

        // Http pass
        if (line.contains("Enter HTTP Proxy Password:")) {
            this->userDataIsNeeded(6);
            showLine = false;
        }

        // QByteArray in QString
        QString lineOut(line);
        QStringList singleLines (lineOut.split("\n"));
        foreach(QString singleLine, singleLines) {
            // Verbindung da?
            if (singleLine.indexOf("Notified TAP-Windows driver") > -1) {
                // IP Suchen und speichern
                QString connIP = singleLine.mid(singleLine.indexOf("Notified TAP-Windows driver") + 56, 15);
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
                Preferences::instance()->showBallonMessage();
                // Status speichern und Tray Icon setzen
                this->connectedSinceDate = QDate::currentDate();
                this->connectedSinceTime = QTime::currentTime();
                this->onConnect = false;
                this->connectionStable = true;
                emit connectionIsStableSig(this->connectionIP);
                QString timeOut = this->getScript("TO");
                int scriptDelay = 0;
                if (timeOut != "") {
                    scriptDelay = timeOut.trimmed().toInt();
                }
                QTimer::singleShot(scriptDelay, this, SLOT(startAfterConnectDelayed()));
            }
        }

        // Fehler abfangen
        bool errorOcurred (false);
        bool _tlsHandshakeFailed (false);
        QString errorMessage ("");
        bool tapIsNeeded (false);
        //"All TAP-Win32 adapters on this system are currently in use"
        if (lineOut.contains("All TAP-Win32 adapters on this system are currently in use", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("All TAP-Win32 adapters on this system are currently in use");
            tapIsNeeded = true;
            errorOcurred = true;
        } else if (lineOut.contains("There are no TAP-Win32 adapters on this system", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("There are no TAP-Win32 adapters on this system");
            tapIsNeeded = true;
            errorOcurred = true;
        } else if (lineOut.contains("Proxy Authentication Required", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("Proxy Authentication Required");
            errorOcurred = true;
        } else if (lineOut.contains("TLS Error: Need PEM pass phrase for private key", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("TLS Error: Need PEM pass phrase for private key");
            errorOcurred = true;
        } else if (lineOut.contains("TLS Error: TLS handshake failed", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("TLS error! See log for details");
            errorOcurred = true;
            _tlsHandshakeFailed = true;
        } else if (lineOut.contains("EVP_DecryptFinal:bad decrypt", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("EVP_DecryptFinal:bad decrypt");
            errorOcurred = true;
        } else if (lineOut.contains("RESOLVE: Cannot resolve host address:", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("Connection error! See log for details");
            errorOcurred = true;
            _tlsHandshakeFailed = true;
        } else if (lineOut.contains("PKCS12_parse:mac verify failure", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("PKCS12_parse:mac verify failure");
            errorOcurred = true;
        } else if (lineOut.contains("Received AUTH_FAILED control message", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("Received AUTH_FAILED control message");
            errorOcurred = true;
        } else if (lineOut.contains("Auth username is empty", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("Auth username is empty");
            errorOcurred = true;
        } else if (lineOut.contains("error=certificate has expired", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("error=certificate has expired");
            errorOcurred = true;
        } else if (lineOut.contains("error=certificate is not yet valid", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("error=certificate is not yet valid");
            errorOcurred = true;
        } else if (lineOut.contains("Cannot load certificate file", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("Cannot load certificate file");
            errorOcurred = true;
        } else if (lineOut.contains("Exiting", Qt::CaseInsensitive)) {

            errorMessage = QObject::tr("Application Exiting!");
            errorOcurred = true;
        } else if (lineOut.contains("Use --help for more information.", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("OpenVPN parameter error! See log for details");
            errorOcurred = true;
        } else if (lineOut.contains("will try again in 5 seconds", Qt::CaseInsensitive)) {
            errorMessage = QObject::tr("OpenVPN connection error! See log for details");
            //errorOcurred = true;
        }

        if (tapIsNeeded) {
            if (Message::confirm(QObject::tr("A Tap device ist needed. Do you want to install a device?"))) {
                QApplication::setOverrideCursor(Qt::WaitCursor);
                if (TapDriver::instance()->installTapDriver()) {
                    Message::information(QObject::tr(("Tap device installed successfully.\nPlease reconnect the connection.")));
                } else {
                    Message::error(QObject::tr(("Tap device installed failed!")));
                }
                QApplication::setOverrideCursor(Qt::ArrowCursor);
            }
        }

        if (lineOut.contains("Proxy requires authentication") && !lineOut.contains("process exiting")) {
            // Kein Fehler
            errorOcurred = false;
            errorMessage = "";
        }

        // Fehler durch normalen Disconnect verhindern
        if (lineOut.contains("SIGTERM[hard,] received, process exiting", Qt::CaseInsensitive))
            errorOcurred = false;

        if (errorOcurred) {
            this->runScript("EC");
            this->showTrayMessage(errorMessage);
            this->connectionStable = false;
            this->setDisconnected();
            this->errorHasOccurred = true;
            this->errMessage = errorMessage;
            this->enableAllMenus();
            emit errorOccuredSig(errorMessage);
            Preferences::instance()->setIcon();
        }

        if (lineOut.contains("Restart pause", Qt::CaseInsensitive)) {
            // Bei Restart Pause befinden wir uns immer noch im Connect auch wenn vorher ein Fehler aufgetreten ist!
            if (!_tlsHandshakeFailed) {
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
            Preferences::instance()->showBallonMessage();
        }

        Preferences::instance()->setIcon();

        // Output ins Log?
        if (showLine) {
            output = QString::fromLatin1(line);
            this->openVpnLogData.append(output);
        }
    }

}

void OpenVpn::openVpnLog()
{
    mLog.logList = &this->openVpnLogData;
    mLog.showDialog();
}

QString OpenVpn::getConnectionIP()
{
    return this->connectionIP;
}

bool OpenVpn::isConnecting()
{
    return this->onConnect;
}

bool OpenVpn::isError()
{
    return this->errorHasOccurred;
}

QString OpenVpn::getErrorString()
{
    return this->errMessage;
}

void OpenVpn::setConnectedIP(const QString &ip)
{
    this->connectionIP = ip;
}

void OpenVpn::setIsConnected(bool flag)
{
    this->connectionStable = flag;
}

void OpenVpn::setIsError(bool flag)
{
    this->errorHasOccurred = flag;
}

void OpenVpn::setErrorString(const QString &errMes)
{
    this->errMessage = errMes;
}

void OpenVpn::setIsConnecting(bool flag)
{
    this->onConnect = flag;
}

bool OpenVpn::isOnConnect()
{
    return this->onConnect;
}

QString OpenVpn::getAdvName()
{
    //
    return (this->isStartConfigValue ? QLatin1String("\n") + QObject::tr("- Start config") : QLatin1String(""));
}

void OpenVpn::setConnectedSinceDate(const QDate &since)
{
    this->connectedSinceDate = since;
}

void OpenVpn::setConnectedSinceTime(const QTime &since)
{
    this->connectedSinceTime = since;
}

QDate OpenVpn::getConnectedSinceDate()
{
    return this->connectedSinceDate;
}

QTime OpenVpn::getConnectedSinceTime()
{
    return this->connectedSinceTime;
}

void OpenVpn::enableAllMenus()
{
    if (Settings::getInstance()->getIsPortableClient()) {
        foreach (QAction *act, (dynamic_cast<QMenu*>(Preferences::instance()->findChild<QMenu*>("SYSTRAYMENU")))->actions()) {
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
    }
}

void OpenVpn::disableMenues()
{
    if (Settings::getInstance()->getIsPortableClient()) {
        foreach (QAction *act, (dynamic_cast<QMenu*>(Preferences::instance()->findChild<QMenu*>("SYSTRAYMENU")))->actions()) {
            if (act->menu() != NULL && !act->menu()->actions().isEmpty()) {
                if (act == this->menu) {
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

void OpenVpn::openEditConfig()
{
    EditConfig editDialog (this->configPath);
    editDialog.exec();
}

void OpenVpn::openExport()
{
    ConfigExport exportDialog (this->configPath);
    exportDialog.exec();
}

void OpenVpn::openManageConnection()
{
    ManageConnection manageDialog (this);
    manageDialog.exec();
}

bool OpenVpn::hasCrediantials(int type)
{
    // Allgemeine Frage oder nach Typ
    if (type == -1) {
        // Allgemein
        if (this->hasCrediantials(0)) {
            return true;
        } else if (this->hasCrediantials(1)) {
            return true;
        } else if (this->hasCrediantials(3)) {
            return true;
        } else if (this->hasCrediantials(5)) {
            return true;
        } else if (this->hasCrediantials(6)) {
            return true;
        }
    }

    // Es wurde ein Typ übergeben
    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key für Crypted User Data
    QString field;
    if (type == 0) {
        field = QLatin1String("[vpn-user]");
    } else if (type == 1) {
        field = QLatin1String("[vpn-password]");
    } else if (type == 3) {
        field = QLatin1String("[vpn-pkcs12]");
    } else if (type == 5) {
        field = QLatin1String("[vpn-http-user]");
    } else if (type == 6) {
        field = QLatin1String("[vpn-http-password]");
    }

    if (field.isEmpty()) {
        // Für den typ kann man keine Daten speichern
        return false;
    }

    QString sql;
    sql = QLatin1String("SELECT ") + field + QLatin1String(" FROM vpn WHERE [vpn-id] = ") + QString::number(this->id());

    // Sind daten für die Verbindung hinterlegt
    QScopedPointer<QSqlQuery> query (this->db->openQuery(sql));
    if (query->first() && !query->value(0).toString().isEmpty()) {
        return true;
    }

    // Kein Daten da
    return false;
}

void OpenVpn::removeCredentials(bool refresh)
{
    QString sql (QLatin1String("UPDATE vpn SET [vpn-user] = '', [vpn-password] = '', [vpn-pkcs12] = '', [vpn-http-user] = '', [vpn-http-password] = '' WHERE [vpn-id] = ") + QString::number(this->id()));
    this->db->execute(sql);

    if (refresh) {
        Preferences::instance()->removeSavedDataIcon(this->id());
    }
}

void OpenVpn::saveUserData(int id, int type, QString value, bool save)
{
    Q_UNUSED(id)

    // Wenn save false ist den Wert überschreiben
    if (!save) {
        return;
    }
    // Welcher type wurde übergeben
    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key für Crypted User Data
    QString field;
    if (type == 0) {
        field = QLatin1String("[vpn-user]");
    } else if (type == 1) {
        field = QLatin1String("[vpn-password]");
    } else if (type == 3) {
        field = QLatin1String("[vpn-pkcs12]");
    } else if (type == 5) {
        field = QLatin1String("[vpn-http-user]");
    } else if (type == 6) {
        field = QLatin1String("[vpn-http-password]");
    }


    if (!value.isEmpty()) {
        if (!Settings::getInstance()->getCryptKey().isEmpty()) {
            Crypt crypt;
            crypt.setSecretKey(Settings::getInstance()->getCryptKey());
            value = crypt.cryptPlainTextExt(value.toAscii());
        }
    }

    QString sql;
    sql = QString("UPDATE vpn SET %1 = '%2' WHERE [vpn-id] = %3")
            .arg(field)
            .arg(value)
            .arg(this->id());
    this->db->execute(sql);

    if (!value.isEmpty() && save) {
        Preferences::instance()->setSavedDataIcon(id);
    }
}

void OpenVpn::getCryptKey(QString key) {
    Settings::getInstance()->setCryptKey(key);
    this->waitForCryptKey = false;
}

QString OpenVpn::getSavedUserData(int type)
{
    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key für Crypted User Data
    QString field;
    if (type == 0) {
        field = QLatin1String("[vpn-user]");
    } else if (type == 1) {
        field = QLatin1String("[vpn-password]");
    } else if (type == 3) {
        field = QLatin1String("[vpn-pkcs12]");
    } else if (type == 5) {
        field = QLatin1String("[vpn-http-user]");
    } else if (type == 6) {
        field = QLatin1String("[vpn-http-password]");
    }

    if (field.isEmpty()) {
        return QString ();
    }

    QString sql;
    sql = QString ("SELECT %1 FROM vpn WHERE [vpn-id] = %2")
            .arg(field)
            .arg(this->id());

    QScopedPointer<QSqlQuery> query (this->db->openQuery(sql));

    QString value;
    if (query->first()) {
        value = query->value(0).toString();
        value = value.trimmed();
    }

    if (!Settings::getInstance()->getCryptKey().isEmpty()) {
        Crypt crypt;
        crypt.setSecretKey(Settings::getInstance()->getCryptKey());
        value = crypt.cryptToPlainTextExt(value.toAscii());
    }

    return value;
}

bool OpenVpn::isStartConfig() const
{
    return this->isStartConfigValue;
}

void OpenVpn::setStartConfig(bool flag)
{
    this->isStartConfigValue = flag;
}

QString OpenVpn::getConfigDirectory() const
{
    return this->configPath.left(this->configPath.lastIndexOf("/"));
}
