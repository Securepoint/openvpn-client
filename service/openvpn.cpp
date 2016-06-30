#include "openvpn.h"
#include <windows.h>

#include "debug.h"
#include "qthreadexec.h"
#include "srvcli.h"

extern bool g_bPortable;

OpenVpn::OpenVpn (SrvCLI * srvCLI)
    : onDisconnect (false),
      useNoInteract (false),
      connectionStable (false),
      connecting (false),
      _id(-1),
      lastAction (-1),
      configName (""),
      configPath (""),
      configPrivKey (""),
      challengeKey (""),
      connectionIP (""),
      connectionLineBak (""),
      procScripts (0),
      proxyString (""),
      _srvCLI(srvCLI),
      retryMultiHostCount(0)
{

}

OpenVpn::~OpenVpn()
{
    Debug::log(QLatin1String("Destroy openvpn"), DebugLevel::Destruktor);
}

void OpenVpn::connectToVpn()
{
    //
    // Die OpenVpn.exe mit der Config starten, um die Verbindung aufzubauen
    //

    // Haben wir eine ID?
    if (this->id() == -1) {
        // Das hier dürfte nie passieren!
        Debug::error(QLatin1String("OpenVpn: No valid id to connect."));
        return;
    }

    this->connecting = true;
    this->lastAction = -1;

    // Pfad für die Config bauen
    QString cFile (this->configPath + QLatin1String("/") + this->configName + QLatin1String(".ovpn"));
    Debug::log(QString("File %1").arg(cFile));

    {
        QFile cf (cFile);

        if (!cf.open(QIODevice::ReadOnly | QIODevice::Text)) {
            this->connecting = false;
             this->connectionStable = false;
            _srvCLI->send(QString("%1;%2")
                             .arg(this->id())
                             .arg("Failed to read " + cFile), QLatin1String("ERROR"));
            return;
        }

        QTextStream in(&cf);

        // Read the host count
        while (!in.atEnd()) {
            QString line = in.readLine();
            if(line.trimmed().toLower().left(6) == "remote")
                ++this->retryMultiHostCount;
        }
    }

    // Die Parameter für OpenVpn bauen
    QStringList arguments;
    arguments << QString ("--service");
    arguments << QString ("openvpngui_exit_%1").arg(this->id());
    arguments << QString ("0");
    arguments << QString ("--config");
    arguments << cFile;

    // Wurde ein Proxy übergeben
    if (!this->proxyString.trimmed().isEmpty()) {
        // Proxy wurde übergeben
        arguments << this->proxyString.split(" ");
    }

    // Interact setzen -> wegen OTP
    if (!this->useNoInteract) {
        arguments << QString ("--auth-retry");
        arguments << QString ("interact");
    }

    arguments << QString("--tls-cipher");
    arguments << QString("TLS-DHE-RSA-WITH-AES-256-CBC-SHA:TLS-DHE-RSA-WITH-CAMELLIA-256-CBC-SHA:TLS-DHE-RSA-WITH-3DES-EDE-CBC-SHA:TLS-DHE-RSA-WITH-AES-128-CBC-SHA:TLS-DHE-RSA-WITH-SEED-CBC-SHA:TLS-DHE-RSA-WITH-CAMELLIA-128-CBC-SHA:TLS-DHE-RSA-WITH-DES-CBC-SHA:TLS-DHE-RSA-WITH-AES-256-GCM-SHA384:TLS-DHE-RSA-WITH-AES-256-CBC-SHA256:TLS-DHE-RSA-WITH-AES-128-GCM-SHA256:TLS-DHE-RSA-WITH-AES-128-CBC-SHA256");

    // Prozesssignale zur Überwachung an die Slots binden
    if (!QObject::connect(&this->proc, SIGNAL(error ( QProcess::ProcessError)), this, SLOT(showProcessError (QProcess::ProcessError)))) {
        Debug::error(QLatin1String("OpenVPN: Can't connect process error signal"));
    }

    if(!QObject::connect(&this->proc, SIGNAL(readyReadStandardOutput()), this, SLOT(readProcessData()))) {
        Debug::error(QLatin1String("OpenVPN: Can't connect read std signal"));
    }

    if(!QObject::connect(&this->proc, SIGNAL(readyReadStandardError()), this, SLOT(readProcessData()))) {
        Debug::error(QLatin1String("OpenVPN: Can't connect read err signal"));
    }

    if (!QObject::connect(&this->proc, SIGNAL(finished (int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)))) {
        Debug::error(QLatin1String("OpenVPN: Can't connect finished signal"));
    }

    // Programm starten im Config Verzeichnis, sonst findet OpenVPN keine Zertifikate
    this->proc.setWorkingDirectory(this->configPath + QLatin1String("/"));

    auto Isx64 = []() -> bool {
        typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

        LPFN_ISWOW64PROCESS fnIsWow64Process;

        BOOL bIsWow64 = FALSE;

        //IsWow64Process is not available on all supported versions of Windows.
        //Use GetModuleHandle to get a handle to the DLL that contains the function
        //and GetProcAddress to get a pointer to the function if available.

        fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
            GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

        if(NULL != fnIsWow64Process) {
            if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)) {
                // handle error
            }
        }
        return (bIsWow64 == TRUE);
    };

    QString arch ("x32");
    if (Isx64()) {
        arch = QLatin1String("x64");
    }

    QString prog = QLatin1String("/bin/openvpn.exe");

    if(g_bPortable)
    {
        prog = QString("/bin/%1/openvpn.exe").arg(arch);
    }

    QString program (QCoreApplication::applicationDirPath() + prog);

    Debug::log(QLatin1String("OpenVpn path: ") + program);
    Debug::log(QLatin1String("Start OpenVpn with: ") + arguments.join(QLatin1String(" ")));

    // Ist die exe da
    if (!QFile::exists(program)) {
        // Leider nicht
        Debug::error(QLatin1String("OpenVPN.exe is missing"));
        emit removeItemFromList(this->id());
        return;
    }

    // Programm starten
    this->proc.start(program, arguments);
    //
    Debug::log(QLatin1String("wait for started"));
    if (!this->proc.waitForStarted()) {
        Debug::error(QLatin1String("Start failed"));
        Debug::log(this->proc.errorString());
        emit removeItemFromList(this->id());
    }

    Debug::log(QLatin1String("End of connect vpn"));
}

void OpenVpn::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode)
    Q_UNUSED(exitStatus)

    // OpenVpn beenden
    QObject::disconnect(&this->proc, 0, 0, 0);

    emit removeItemFromList(this->id());

    Debug::log(QLatin1String("OpenVpn: Process finished"));

    this->connectionStable = false;
    this->connecting = false;
    this->lastAction = -1;
}

bool OpenVpn::isConnectionStable () const
{
    return this->connectionStable;
}

bool OpenVpn::isConnecting() const
{
    return this->connecting;
}

void OpenVpn::setProxyString(const QString &proxy)
{
    this->proxyString = proxy;
}

void OpenVpn::sendStatus()
{
    Debug::log(QLatin1String("Send status"));
    _srvCLI->send(QString("%1;%2;%3;%4;%5")
                             .arg(this->id())
                             .arg((this->isConnectionStable() ? QLatin1String("1") : QLatin1String("0")))
                             .arg((this->isConnecting() ? QLatin1String("1") : QLatin1String("0")))
                             .arg(this->lastAction)
                             .arg(this->connectionIP), QLatin1String("STATUS"));
}

void OpenVpn::showProcessError(QProcess::ProcessError error)
{
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

    Debug::error(QLatin1String("OpenVpn: ") + errMessage);
    if (!this->onDisconnect) {
        _srvCLI->send(QString::number(this->id()) + QLatin1String(";") + errMessage, QLatin1String("ERROR"));
        // Wenn der Process nicht gestartet werden kann, gibt es auch kein finished Signal
        if (error == QProcess::FailedToStart) {
            // Element aus der Liste löschen
            emit removeItemFromList(this->id());
        }
    }
}

void OpenVpn::disconnectVpn(bool silent)
{

    Debug::log(QLatin1String("Disconnect OpenVPN.  Id: ") + QString::number(this->id()));

    // OpenVpn beenden
    QObject::disconnect(&this->proc, 0, 0, 0);

    this->onDisconnect = true;

    this->configName = "";
    this->configPath = "";
    this->connectionStable = false;
    this->connecting = false;
    // Per Winapi killen
    HANDLE exitEvent;
    QString eventName (QLatin1String("openvpngui_exit_") + QString::number(this->id()));
    exitEvent = CreateEvent(NULL, TRUE, FALSE, (LPCTSTR)eventName.utf16());
    if (!exitEvent) {
        return;
    }
    SetEvent(exitEvent);
    CloseHandle (exitEvent);

    if(!this->proc.waitForFinished(5000)) {
        Debug::log(QLatin1String("OpenVpn do kill"));
        this->proc.kill();
        this->proc.waitForFinished(5000);
    }

    if(!silent)
    {
        //
        emit removeItemFromList(this->id());
        _srvCLI->send(QString::number(this->id()), QLatin1String("DISCONNECTED"));
    }

}

void OpenVpn::readProcessData()
{
    if (this->onDisconnect) {
        return;
    }

    Debug::log (QLatin1String("Read process data"));

    QByteArray line;
    bool showLine = true;
    line = this->proc.readAllStandardError();
    if (line == "")
        line = this->proc.readAllStandardOutput();
    if (line != "") {
        Debug::log ("=============================");
        Debug::log (line);
        //Enter Private Key Password:
        // 0 - Username
        // 1 - Pwd
        // 2 - OTP
        // 3 - PKCS12
        // 4 - Private Key für Crypted User Data
        QString pkkey (line);
        if (pkkey.contains("Enter Private Key Password:", Qt::CaseInsensitive)) {
            Debug::log(QLatin1String("Wait for private key"));
            this->lastAction = 3;
            _srvCLI->send(QString("%1;3").arg(this->id()), QLatin1String("INPUT"));
            showLine = false;
        } else if (line == "Enter Auth Username:" || line == "Enter Challenge Username:" ) { // Enter Username?
            Debug::log(QLatin1String("Wait for User"));
            if (line == "Enter Challenge Username:" && !this->challangeUserName.isEmpty()) {
                this->setUsername(this->challangeUserName);
            } else {
                this->lastAction = 0;
                _srvCLI->send(QString("%1;0").arg(this->id()), QLatin1String("INPUT"));
            }
            showLine = false;
        } else if (line == "Enter Auth Password:") { // Enter Password
            Debug::log(QLatin1String("Wait for pass"));
            this->lastAction = 1;
            _srvCLI->send(QString("%1;1").arg(this->id()), QLatin1String("INPUT"));
            showLine = false;
        } else if (line == "Enter Challenge Password:" || line.contains("CHALLENGE: Please enter token PIN")) { // Challenge Key
            Debug::log(QLatin1String("Wait for challenge"));
            this->lastAction = 2;
            _srvCLI->send(QString("%1;2").arg(this->id()), QLatin1String("INPUT"));
            showLine = false;
        } else if (line.contains("Enter HTTP Proxy Username:")) {
            Debug::log(QLatin1String("Wait for http user"));
            this->lastAction = 5;
            _srvCLI->send(QString("%1;5").arg(this->id()), QLatin1String("INPUT"));
            showLine = false;
        } else if (line.contains("Enter HTTP Proxy Password:")) {
            Debug::log(QLatin1String("Wait for http pass"));
            this->lastAction = 6;
            _srvCLI->send(QString("%1;6").arg(this->id()), QLatin1String("INPUT"));
            showLine = false;
        }
        // QByteArray in QString
        QString lineOut(line);
        QStringList singleLines (lineOut.split("\n"));
        foreach(QString singleLine, singleLines) {
            Debug::log(QLatin1String("SL: ") + singleLine);
            // Verbindung da?
            if (singleLine.indexOf("Notified TAP-Windows driver") > -1) {
                Debug::log(QLatin1String("Found DHCP"));
                // IP Suchen und speichern
                QString connIP = singleLine.mid(singleLine.indexOf("Notified TAP-Windows driver") + 56, 15);
                Debug::log(QLatin1String("connIP: ") + connIP);
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

                this->lastAction = -1;
                // Meldung zeigen connected
                this->connectionIP = connIP;
                Debug::log(QLatin1String("connIP Clean: ") + connectionIP);
                // Status speichern und Tray Icon setzen
                this->connectionStable = true;
                // Line speichern
                this->connectionLineBak = lineOut;
            }
        }

        // Fehler abfangen
        bool errorOcurred = false;
        bool _tlsHandshakeFailed (false);
        QString errorMessage;
        //"All TAP-Win32 adapters on this system are currently in use"
        if (lineOut.contains("All TAP-Windows adapters on this system are currently in use", Qt::CaseInsensitive)) {
            _srvCLI->send(QLatin1String("NEEDED"), QLatin1String("TAPINSTALL"));
            errorMessage = QLatin1String ("All TAP-Windows adapters on this system are currently in use");
            errorOcurred = true;
        } else if (lineOut.contains("There are no TAP-Windows adapters on this system", Qt::CaseInsensitive)) {
            _srvCLI->send(QLatin1String("NEEDED"), QLatin1String("TAPINSTALL"));
            errorMessage = QLatin1String ("There are no TAP-Windows adapters on this system");
            errorOcurred = true;
        } else if (lineOut.contains("TLS Error: Need PEM pass phrase for private key", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("TLS Error: Need PEM pass phrase for private key");
            errorOcurred = true;
        } else if (lineOut.contains("TLS Error: TLS handshake failed", Qt::CaseInsensitive)) {

            errorMessage = QLatin1String ("TLS error! See log for details");
            errorOcurred = true;

            if(this->retryMultiHostCount <= 0) {

                _tlsHandshakeFailed = true;
            }
            else {
                --this->retryMultiHostCount;
            }

        } else if (lineOut.contains("RESOLVE: Cannot resolve host address:", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("Connection error! See log for details");
            errorOcurred = true;
             if(this->retryMultiHostCount <= 0) {
                _tlsHandshakeFailed = true;
            }
            else  {
                --this->retryMultiHostCount;
            }
        } else if (lineOut.contains("EVP_DecryptFinal:bad decrypt", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("EVP_DecryptFinal:bad decrypt");
            errorOcurred = true;
        } else if (lineOut.contains("PKCS12_parse:mac verify failure", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("PKCS12_parse:mac verify failure");
            errorOcurred = true;
        } else if (lineOut.contains("Received control message: AUTH_FAILED", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("Received AUTH_FAILED control message");
            errorOcurred = true;
        } else if (lineOut.contains("Auth username is empty", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("Auth username is empty");
            errorOcurred = true;
        } else if (lineOut.contains("error=certificate has expired", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("error=certificate has expired");
            errorOcurred = true;
        } else if (lineOut.contains("error=certificate is not yet valid", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("error=certificate is not yet valid");
            errorOcurred = true;
        } else if (lineOut.contains("Proxy Authentication Required", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("Proxy Authentication Required");
            errorOcurred = true;
        } else if (lineOut.contains("Cannot load certificate file", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("Cannot load certificate file");
            errorOcurred = true;
        } else if (lineOut.contains("Exiting", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("Application Exiting!");
            errorOcurred = true;
        } else if (lineOut.contains("Use --help for more information.", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("OpenVPN parameter error!\nSee log for details");
            errorOcurred = true;
        } else if (lineOut.contains("will try again in 5 seconds", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("OpenVPN connection error!\nSee log for details");
        } else if (lineOut.contains("No Route to Host", Qt::CaseInsensitive)) {
            errorMessage = QLatin1String ("No Route to Host!\nSee log for details");
            _srvCLI->send(QString("%1;%2")
                             .arg(this->id())
                             .arg(errorMessage), QLatin1String("ERROR"));

            if(this->retryMultiHostCount <= 0)
            {
                this->onDisconnect = true;
                this->connectionStable = false;
                this->proc.kill();
            }
            else
            {
                --this->retryMultiHostCount;
            }
       }

        if (lineOut.contains("Proxy requires authentication") && !lineOut.contains("process exiting")) {
            // Kein Fehler
            errorOcurred = false;
            errorMessage = "";
        }

        // Fehler durch normalen Disconnect verhindern
        if (lineOut.contains("SIGTERM[hard,] received, process exiting", Qt::CaseInsensitive)) {
            errorOcurred = false;
        }

        if (errorOcurred) {
            this->connectionStable = false;
            _srvCLI->send(QString("%1;%2")
                             .arg(this->id())
                             .arg(errorMessage), QLatin1String("ERROR"));


        }


        if(lineOut.contains("Received control message: AUTH_FAILED", Qt::CaseInsensitive))
        {
            _srvCLI->send(QString ("%1")
                .arg(this->id()), QLatin1String("REMOVE_USER_DATA"));
        }

        // Die Ausgabe von OpenVPN immer senden, sofern es keine Useraufforderungen sind
        if (showLine) {
            _srvCLI->send(QString("%1;%2")
                             .arg(this->id())
                             .arg(QString(line)), QLatin1String("LOG"));
        }


        if (_tlsHandshakeFailed) {
            this->disconnectVpn();
        }

        if (lineOut.contains("Restart pause", Qt::CaseInsensitive)) {
            // Bei Restart Pause befinden wir uns immer noch im Connect auch wenn vorher ein Fehler aufgetreten ist!
            // Status speichern und Tray Icon setzen
            if (!_tlsHandshakeFailed) {
                _srvCLI->send(QString("%1").arg(this->id()), QLatin1String("RESTART"));
            } else {
                _srvCLI->send(QString ("%1;%2")
                                 .arg(this->id())
                                 .arg(QLatin1String("Timeout[Maybe your cetificates are not valid. Please check if it is revoked], restart pause will be ignored! Shuting down OpenVPN ...")), QLatin1String("LOG"));
            }
        }

        //Initialization Sequence Completed
        if (lineOut.contains("Initialization Sequence Completed", Qt::CaseInsensitive)) {
            this->connectionStable = true;
            this->connecting = false;
            _srvCLI->send(QString ("%1;%2")
                             .arg(this->id())
                             .arg(connectionLineBak), QLatin1String("LOG"));

            // Nun die Ip senden, damit kann der Client auf grün gehen
            Debug::log(QString("RECEIVEDIP: %1;%2").arg(this->id()).arg(this->connectionIP));
            _srvCLI->send(QString("%1;%2").arg(this->id()).arg(this->connectionIP), QLatin1String("RECEIVEDIP"));
        }

    }
}

void OpenVpn::errorSocket(QAbstractSocket::SocketError err)
{
    Q_UNUSED(err)
}

void OpenVpn::setUsername(const QString &username)
{
    Debug::log(QLatin1String("Set username"));
    if (this->proc.isWritable()) {
        if (this->challangeUserName.isEmpty()) {
            this->challangeUserName = username;
        }
        QByteArray ba;
        ba.append(username + "\n");
        this->proc.write(ba);
        this->lastAction = -1;
    } else {
        Debug::error(QLatin1String("Process is not writable"));
    }
}


void OpenVpn::setPassword(const QString &pwd)
{
    Debug::log(QLatin1String("Set password"));
    if (this->proc.isWritable()) {
        QByteArray ba;
        ba.append(pwd + "\n");
        this->proc.write(ba);
        this->lastAction = -1;
    } else {
        Debug::error(QLatin1String("Process is not writable"));
    }
}

void OpenVpn::setPrivateKey(const QString &key)
{
    Debug::log(QLatin1String("Set private key"));
    if (this->proc.isWritable()) {
        QByteArray ba;
        ba.append(key + "\n");
        this->proc.write(ba);
        this->lastAction = -1;
    } else {
        Debug::error(QLatin1String("Process is not writable"));
    }
}

void OpenVpn::setChallengeKey(const QString &key)
{
    Debug::log(QLatin1String("Set challenge key"));
    if (this->proc.isWritable()) {
        QByteArray ba;
        ba.append(key + "\n");
        this->proc.write(ba);
        this->lastAction = -1;
    } else {
        Debug::error(QLatin1String("Process is not writable"));
    }
}

int OpenVpn::id() const
{
    return this->_id;
}

void OpenVpn::setId(const int &ident)
{
    this->_id = ident;
}

void OpenVpn::setConfigName(const QString &name)
{
    this->configName = name;
}

void OpenVpn::setConfigPath(const QString &path)
{
    this->configPath = path;
}

void OpenVpn::setUseInteract(const QString &interact)
{
    this->useNoInteract = (interact == QLatin1String("1") ? true : false);
}
