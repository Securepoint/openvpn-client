#include "openvpn.h"
#include <windows.h>

OpenVpn::OpenVpn () {
    this->connectionStable = false;
    this->onDisconnect = false;
    this->isLinked = false;
    this->configName = "";
    this->configPath = "";
    this->configPwd = "";
    this->configPrivKey = "";
    this->configUser = "";
    this->clientPort = 3655;
    this->appPath = "";
    this->waitForData = false;
    this->connectionLineBak = "";
    this->useNoInteract = false;
    this->challengeKey = "";
    this->proc = new QProcess(this);
}

void OpenVpn::connectToVpn(){
    // Connect
    this->connectionLineBak = "";
    this->onDisconnect = true;
    this->connectionStable = false;
    this->proc->kill();

    this->onDisconnect = false;

    // connect to VPN
    QString cFile;
    QStringList arguments;
    QString program = this->appPath + "/app/bin/openvpn";
    //this->writeLogDataToSocket(program);


    // Pfad für die Config bauen
    cFile = this->configPath + QString("/") + this->configName + QString(".ovpn");
    // Als Argument adden
    arguments << QString ("--service");
    arguments << QString ("openvpngui_exit");
    arguments << QString ("0");
    arguments << QString ("--config");
    arguments << cFile;

    // Interact setzen
    if (!this->useNoInteract) {
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
}

void OpenVpn::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    this->connectionStable = false;
    this->sendStatus();
}

bool OpenVpn::isConnectionStable () {
    return this->connectionStable;
}

void OpenVpn::sendStatus() {
    this->writeLogDataToSocket(QString("STATUS:") + (this->isConnectionStable() ? QString("true") : QString("false")) + QString("<>") + this->configPath + QString("<>") + this->configName);
}

void OpenVpn::showProcessError(QProcess::ProcessError error) {
    this->connectionStable = false;
    QString errMessage;
    switch (error) {
        case QProcess::FailedToStart:
            errMessage = QString ("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
            break;
        case QProcess::Crashed:
            errMessage = QString ("The process crashed some time after starting successfully.");
            break;
        case QProcess::Timedout:
            errMessage = QString ("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.");
            break;
        case QProcess::WriteError:
            errMessage = QString ("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.");
            break;
        case QProcess::ReadError:
            errMessage = QString ("An error occurred when attempting to read from the process. For example, the process may not be running.");
            break;
        case QProcess::UnknownError:
            errMessage = QString ("An unknown error occurred. This is the default return value of error().");
            break;
        default:
            errMessage = QString ("No valid error code!");
            break;
    }
    if (!this->onDisconnect)
        this->writeLogDataToSocket("ERROR:" + errMessage);
}

void OpenVpn::disconnectVpn() {
        this->configName = "";
        this->configPath = "";
        this->configPwd = "";
        this->configUser = "";
        this->onDisconnect = true;
        this->connectionStable = false;
        // Per Winapi killen
        HANDLE exitEvent;
        QString eventName = "openvpngui_exit";
        exitEvent = CreateEvent(NULL, TRUE, FALSE, (LPCTSTR)eventName.utf16());
        if (!exitEvent) {
            return;
        }
        SetEvent(exitEvent);
        CloseHandle (exitEvent);

        this->writeLogDataToSocket("DISCONNECTED:");
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
                QByteArray ba;
                this->waitForData = true;
                this->writeLogDataToSocket("PKPWD:NEEDED");
                while (this->waitForData) {
                    qApp->processEvents();
                }
                // Daten da
                ba.append(this->configPrivKey + "\n");

                this->proc->write(ba);
                showLine = false;
            }

        }
        // Enter Username?
        if (line == "Enter Auth Username:" || line == "Enter Challenge Username:" ) {
            if (this->proc->isWritable()) {
                QByteArray ba;
                this->waitForData = true;
                this->writeLogDataToSocket("USER:NEEDED");
                while (this->waitForData) {
                    qApp->processEvents();
                }
                // Daten da
                ba.append(this->configUser + "\n");

                this->proc->write(ba);
                showLine = false;
            }

        }
        // Enter Password
        if (line == "Enter Auth Password:") {
            if (this->proc->isWritable()) {
                QByteArray ba;
                this->waitForData = true;
                this->writeLogDataToSocket("UPWD:NEEDED");
                while (this->waitForData) {
                    qApp->processEvents();
                }
                ba.append(this->configPwd + "\n");
                this->proc->write(ba);
                showLine = false;
            }

        }

        // Challenge Key
        if (line == "Enter Challenge Password:") {
            if (this->proc->isWritable()) {
                QByteArray ba;
                this->waitForData = true;
                this->writeLogDataToSocket("CKPWD:NEEDED");
                while (this->waitForData) {
                    qApp->processEvents();
                }
                ba.append(this->challengeKey + "\n");
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
            // Status speichern und Tray Icon setzen
            this->connectionStable = true;
            // Line speichern
            this->connectionLineBak = lineOut;
        }
        
        // Fehler abfangen
        bool errorOcurred = false;
        QString errorMessage;
        //"All TAP-Win32 adapters on this system are currently in use"
        if (lineOut.contains("All TAP-Win32 adapters on this system are currently in use", Qt::CaseInsensitive)) {
            errorMessage = QString ("All TAP-Win32 adapters on this system are currently in use");
            errorOcurred = true;
        } else if (lineOut.contains("TLS Error: Need PEM pass phrase for private key", Qt::CaseInsensitive)) {
            errorMessage = QString ("TLS Error: Need PEM pass phrase for private key");
            errorOcurred = true;
        } else if (lineOut.contains("EVP_DecryptFinal:bad decrypt", Qt::CaseInsensitive)) {
            errorMessage = QString ("EVP_DecryptFinal:bad decrypt");
            errorOcurred = true;
        } else if (lineOut.contains("PKCS12_parse:mac verify failure", Qt::CaseInsensitive)) {
            errorMessage = QString ("PKCS12_parse:mac verify failure");
            errorOcurred = true;
        } else if (lineOut.contains("Received AUTH_FAILED control message", Qt::CaseInsensitive)) {
            errorMessage = QString ("Received AUTH_FAILED control message");
            errorOcurred = true;
        } else if (lineOut.contains("Auth username is empty", Qt::CaseInsensitive)) {
            errorMessage = QString ("Auth username is empty");
            errorOcurred = true;
        } else if (lineOut.contains("error=certificate has expired", Qt::CaseInsensitive)) {
            errorMessage = QString ("error=certificate has expired");
            errorOcurred = true;
        } else if (lineOut.contains("error=certificate is not yet valid", Qt::CaseInsensitive)) {
            errorMessage = QString ("error=certificate is not yet valid");
            errorOcurred = true;
        } else if (lineOut.contains("Cannot load certificate file", Qt::CaseInsensitive)) {
            errorMessage = QString ("Cannot load certificate file");
            errorOcurred = true;
        } else if (lineOut.contains("Exiting", Qt::CaseInsensitive)) {
            errorMessage = QString ("Application Exiting!");
            errorOcurred = true;
        } else if (lineOut.contains("Use --help for more information.", Qt::CaseInsensitive)) {
            errorMessage = QString ("OpenVPN parameter error!\nSee log for details");
            errorOcurred = true;
        } else if (lineOut.contains("will try again in 5 seconds", Qt::CaseInsensitive)) {
            errorMessage = QString ("OpenVPN connection error!\nSee log for details");
            //errorOcurred = true;
        } else if (lineOut.contains("No Route to Host", Qt::CaseInsensitive)) {
            errorMessage = QString ("No Route to Host!\nSee log for details");
            this->writeLogDataToSocket("ERROR:" + errorMessage);
            this->onDisconnect = true;
            this->connectionStable = false;
            this->proc->kill();
        }

        // Fehler durch normalen Disconnect verhindern
        if (lineOut.contains("SIGTERM[hard,] received, process exiting", Qt::CaseInsensitive))
            errorOcurred = false;

        if (errorOcurred) {
            this->connectionStable = false;
            this->writeLogDataToSocket("ERROR:" + errorMessage);
        } else {
            if (showLine)
                this->writeLogDataToSocket(line);
        }

        if (lineOut.contains("Restart pause", Qt::CaseInsensitive)) {
            // Bei Restart Pause befinden wir uns immer noch im Connect auch wenn vorher ein Fehler aufgetreten ist!
            // Status speichern und Tray Icon setzen
            this->writeLogDataToSocket("RESTART:ACTION");
        }
        //Initialization Sequence Completed
        if (lineOut.contains("Initialization Sequence Completed", Qt::CaseInsensitive)) {
            this->connectionStable = true;
            this->writeLogDataToSocket(this->connectionLineBak);
        }

    }

}

void OpenVpn::writeLogDataToSocket(QString logData) {
    QTcpSocket *socket = new QTcpSocket(this);

    socket->connectToHost(QString("127.0.0.1"), this->clientPort);

    if (socket->waitForConnected(500)) {
        QTextStream os(socket);

        os.setAutoDetectUnicode(true);
        os << "log->" + logData.replace("\r\n", "#<>#") + "\r\n"
           << QDateTime::currentDateTime().toString() << "\n";

        socket->close();
    }
    if (socket->state() == QAbstractSocket::UnconnectedState ||
                           socket->waitForDisconnected(500)) {
        delete socket;
    }
}

void OpenVpn::setUsername(QString username) {
    this->configUser = username;
    this->waitForData = false;
}

void OpenVpn::setPWd(QString pwd) {
    this->configPwd = pwd;
    this->waitForData = false;
}

void OpenVpn::setPrivKey(QString key) {
    this->configPrivKey = key;
    this->waitForData = false;
}

void OpenVpn::setChallengeKey(QString key) {
    this->challengeKey = key;
    this->waitForData = false;
}
