#include "frmmain.h"

#include <Windows.h>
#include <Windowsx.h>
#include <Shlobj.h>
#include <string>
#include <cstdio>

#include <ShellScalingApi.h>

#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
//
#include <database/database.h>
#include <widgets/settings/client/settings.h>
#include <utils.h>
#include <database\crypt.h>
#include <fstream>
#include <Windows.h>
#include <service\srvcli.h>

#include <future>

#ifdef QT_OPENGL
#error "OpenGL"
#endif

#ifndef QT_OPENGL_ES_2_ANGLE
#error "No Angle"
#endif

#include <single_application.h>

#include <psapi.h>
#include <tchar.h>
#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <tapdriver.h>
#include <config\Configs.h>
#include <widgets\settings\client\settings.h>
#include <message.h>

// Just a simple socket used in the cli
class ClientCom : public QObject
 {
     Q_OBJECT

 private:
      qint64 blockSize;
 public:
        QTcpSocket socket;

       ClientCom()
           : blockSize(0)
       {
            QObject::connect(&socket, SIGNAL(readyRead()), this, SLOT(slotRead()));
       }

       void sendCommand(const QString& command, const QString &params)
       {
           // Neuen Block zum Senden erstellen
           QByteArray block;
           // Datasteam an den Block binden
           QDataStream out (&block, QIODevice::WriteOnly);
           // DataStream version setzen, hier aktuelle 4.6 = DataStream version 10
           out.setVersion(QDataStream::Qt_4_6);
           // Gr��e des Blockes erstmal mit 0 initieren und Aktion angeben
           out << quint64(0);
           out << command;
           out << params;
           // Wieder an die erste Stelle des Blockes springen und die Gr��e neu setzen
           out.device()->seek(0);
           out << quint64(block.size() - sizeof(quint64));
           // Block an das Socket schicken und senden
           this->socket.write(block);
           this->socket.waitForBytesWritten();
           this->socket.flush();
       }

       void start()
       {

       }

public slots:
     void slotRead()
     {
         QDataStream in(&socket);
         in.setVersion(QDataStream::Qt_4_6);

         if (blockSize == 0) {
             if ((quint32) socket.bytesAvailable() < sizeof(qint64)) {
                 return;
             }
             in >> blockSize;
         }

         if (socket.bytesAvailable() < blockSize) {
             return;
         }

         this->blockSize = 0;

         QString command("");
         QString params("");

         in >> command;
         in >> params;

         if(command == "VPN_LOG")
         {
             printf(params.toLatin1().data());
             OutputDebugStringA(params.toLatin1().data());
             OutputDebugStringA("\n");
             printf("\n");
         }
         else if(command == "VPN_ERROR")
         {
             printf(params.toLatin1().data());
             printf("\n");
         }
         else if(command == "GET_CLIENTS")
         {
             printf(params.toLatin1().data());
             printf("\n");
         }

         // Always close the application after handling the command

         this->socket.disconnectFromHost();

         Sleep(100);

         QApplication::exit();
     }
 };

#include "release/main.moc"

// Use a global variable as save user data flag
// Default is true, set it to false is only possible with
// a command line argument -noSave
bool globalSaveUserData (true);
// Wheather show user informations or not
bool g_bSilent = false;
#ifdef _PORTABLE
bool g_bPortable = true;
#pragma message("Compiling Portable")
#else
bool g_bPortable = false;
#pragma message("Compiling Normal")
#endif
bool g_bDebug = false;

static const WORD MAX_CONSOLE_LINES = 500;

void myMessageOutput(QtMsgType type, const QMessageLogContext & context, const QString & msg)
 {
     //in this function, you can write the message to any stream!
     switch (type) {
     case QtDebugMsg:
         if(g_bDebug)
            printf("Debug: %s at [%s:%d]\n", msg.toLatin1().data(), context.file, context.line);
         break;
     case QtWarningMsg:
         if(g_bDebug)
             printf( "Warning: %s\n", msg.toLatin1().data());
         break;
     case QtCriticalMsg:
         if(g_bDebug)
             printf("Critical: %s\n", msg.toLatin1().data());
         break;
     case QtFatalMsg:
         if(g_bDebug)
             printf("Fatal: %s\n", msg.toLatin1().data());
         abort();
     }
 }





// Just parse a string into proper argv and output the argc
// this is required because we DONT'T use Console mode for this application
// So we dont get the argv and argc
 PCHAR*
    CommandLineToArgvA(
        PCHAR CmdLine,
        int* _argc
        )
    {
        PCHAR* argv;
        PCHAR  _argv;
        ULONG   len;
        ULONG   argc;
        CHAR   a;
        ULONG   i, j;

        bool  in_QM;
        bool  in_TEXT;
        bool  in_SPACE;

        len = strlen(CmdLine);
        i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

        argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
            i + (len+2)*sizeof(CHAR));

        _argv = (PCHAR)(((PUCHAR)argv)+i);

        argc = 0;
        argv[argc] = _argv;
        in_QM = false;
        in_TEXT = false;
        in_SPACE = true;
        i = 0;
        j = 0;

        while( a = CmdLine[i] ) {
            if(in_QM) {
                if(a == '\"') {
                    in_QM = false;
                } else {
                    _argv[j] = a;
                    j++;
                }
            } else {
                switch(a) {
                case '\"':
                    in_QM = true;
                    in_TEXT = true;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    in_SPACE = false;
                    break;
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    if(in_TEXT) {
                        _argv[j] = '\0';
                        j++;
                    }
                    in_TEXT = false;
                    in_SPACE = true;
                    break;
                default:
                    in_TEXT = true;
                    if(in_SPACE) {
                        argv[argc] = _argv+j;
                        argc++;
                    }
                    _argv[j] = a;
                    j++;
                    in_SPACE = false;
                    break;
                }
            }
            i++;
        }
        _argv[j] = '\0';
        argv[argc] = NULL;

        (*_argc) = argc;
        return argv;
    }

 QString g_strClientName;

 static const char* g_szVersion = "2.0.19";

 void PrintHelp()
 {
     printf("Version %s\n", g_szVersion);
     printf("\nUsage: SSLVpnClient.com [options]\n");
     printf("Options:\n");
     printf("\t-help: Display this information\n");
     printf("\t-manage: Allow the user to import, edit, remove and create configs\n");
     printf("\t-german: Change the language to german\n");
     printf("\t-silent: Suppress all user interaction (Confirmation dialogs)\n");
     printf("\t-start configPath user password \n");
     printf("\t-user username used for all connections\n");
     printf("\t-noSave prevent storage of the user credentials\n");
     printf("\t-pwd password used for all connections \n");
     printf("\t-portable: Start the client in portable mode (configs are saved in the current directory)\n");
     printf("\t-vpnlog: Print the VPN Log in the command line\n");
     printf("\t-status: shows the status of the connections in the currently running VPN Client instance of the current folder [ ID NAME STATUS ]\n");
     printf("\t-removeTap: Remove TAP Devices on shutdown\n");
     printf("\t-stop: Close the VPN Client\n");
     printf("\t-log clientid configid: Prints the VPN Log for the given configuration\n");
     printf("\t-error id: Prints the last error for the given configuration\n");
     printf("\t-clients: Show a list of running clients with their id [NAME ID]\n");
     printf("\t-version: Shows the current version of the client\n");
 }


#include <cmath>


 float windowsDpiScale();

int CALLBACK WinMain (_In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow)
{
    Q_UNUSED(hInstance)
    Q_UNUSED(hPrevInstance)
    Q_UNUSED(lpCmdLine)
    Q_UNUSED(nCmdShow)

    // Enable high dpi support, this settings needed
    // at least Qt 5.6!
    //qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "1");



    // We have to disable buffering, otherwise the output will not show in the console
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    LPCSTR *argv;
    int argc;

    // Get the command line and split the command line arguments in argv so we can use it in the code below
    argv = (LPCSTR*)CommandLineToArgvA(GetCommandLineA(), &argc);

    // TODO: Remove qt 5.6 high dpi support until all custom crap is removed
    // QApplication::setDesktopSettingsAware(false);
    SingleApplication a(argc, (char**)argv);

    // Set fusion style on high dpi displays
    if(a.devicePixelRatio() > 1) {
        // We are running on a high dpi display
        // Use the fusion style for better look and feel
        //QApplication::setStyle(QStringLiteral("fusion"));
    }

    std::vector<ConfigData> vecStartConfigs;

    bool removeTap = false;

    QString startConfig = "";

    bool cmdFound = true;

    // Portable is always managed
    if(g_bPortable) {
        Settings::instance()->setManaged(true);
    }

    bool  loadGermanTranslation(false);
    // Handle CLI
    for (int x = 1; x < argc; x++) {
        if (!strcmp(argv[x], "-manage")) {
            // Benutzer kann Einstellungen �ndern
            Settings::instance()->setManaged(true);
        } else if(!strcmp(argv[x], "-german")) {
            loadGermanTranslation = true;
        } else if(!strcmp(argv[x], "-silent")) {
            g_bSilent = true;
        } else if(!strcmp(argv[x], "-portable")) {
            g_bPortable = true;
        } else if(!strcmp(argv[x], "-debug")) {
            g_bDebug = true;
        } else if(!strcmp(argv[x], "-start") && x + 3 < argc) {
            QString startConfigPath = argv[x + 1];
            QString startUsername = argv[x + 2];
            QString startPassword = argv[x + 3];
            startConfig = startConfigPath;

            ConfigData configData;
            startConfig.replace("\\", "/");
            QString configName = startConfig;
            configName.replace(".ovpn", "");
            configName = configName.right(configName.size() - configName.lastIndexOf("/") - 1);

            configData.configName = configName;
            configData.isAutoStart = true;
            configData.configFile = startConfig;
            configData.vpnUser = startUsername;
            configData.vpnPassword = startPassword;

            vecStartConfigs.push_back(configData);

            x+=3;
        } else if(!strcmp(argv[x], "-user" ) && x + 1 < argc) {
            QString startUser = argv[x + 1];
            Settings::instance()->setStartUser(startUser);
            x++;
        } else if(!strcmp(argv[x], "-pwd") && x + 1 < argc) {
            QString startPassword = argv[x + 1];
            Settings::instance()->setStartPassword(startPassword);
            x++;
        } else if(!strcmp(argv[x], "-removeTap")) {
            removeTap = true;
        } else if(!strcmp(argv[x], "-noSave")) {
            // Denial storage of the user crendentials etc.
            globalSaveUserData = false;
        } else if(!strcmp(argv[x], "-status")) {

            // Can I make that with  the , well, Service?!

            Configs::instance()->refreshConfigs();

            for(auto &config : Configs::instance()->getList()) {
                std::string state = "UNKNOWN";

                if(config.second->GetState() == ConnectionState::Connected)
                    state = "CONNECTED";
                else if(config.second->GetState() == ConnectionState::Connecting)
                    state = "CONNECTING";
                else if(config.second->GetState() == ConnectionState::Disconnected)
                    state = "DISCONNECTED";
                else if(config.second->GetState() == ConnectionState::Disconnecting)
                    state = "DISCONNECTING";
                else if(config.second->GetState() == ConnectionState::Error)
                    state = "ERROR";


                state += " [" + std::string(config.second->GetError().toLatin1().data()) + "]";

                printf("%d %s %s\n", config.second->GetId(), config.second->GetName().toLatin1().data(), state.c_str());
            }

            return 0;
        } else if(!strcmp(argv[x], "-log")  && x + 1 < argc) {

            QString clientIndex  = argv[x + 1];
            QString sconfigIndex  = argv[x + 2];


            ClientCom com;

            // Connect to the runnin client instance
            com.socket.connectToHost(QLatin1String("127.0.0.1"), 3656);
            if(!com.socket.waitForConnected()) {
                //
                return 0;
            }


            com.sendCommand("GETLOG", clientIndex + ";" + sconfigIndex);

            QApplication::exec();

            return 0;
        } else if(!strcmp(argv[x], "-error") && x +2 < argc) {

            QString clientIndex  = argv[x + 1];
            QString sconfigIndex  = argv[x + 2];
            //int configIndex = sconfigIndex.toInt();


            ClientCom com;

            // Connect to the runnin client instance
            com.socket.connectToHost(QLatin1String("127.0.0.1"), 3656);
            if(!com.socket.waitForConnected()) {
                //
                return 0;
            }


            com.sendCommand("GET_ERROR", clientIndex + ";" + sconfigIndex);

            QApplication::exec();

            return 0;
        } else if(!strcmp(argv[x], "-vpnlog")) {
            Settings::instance()->setVpnLog(true);
        } else if(!strcmp(argv[x], "-stop")) {
            //#define CLOSE_VPN_CLIENT 11055
            //HWND hwnd = FindWindowA(NULL, "HalloSPSSLVPN");
            //if(hwnd)
            //	SendMessage(hwnd, CLOSE_VPN_CLIENT, NULL, NULL);
            //printf("Could not find the vpn client\n");
            ClientCom com;

            // Connect to the runnin client instance
            com.socket.connectToHost(QLatin1String("127.0.0.1"), 3656);
            if(!com.socket.waitForConnected()) {
                //
                return 0;
            }


            com.sendCommand("STOP_CLIENTS", "");

            QApplication::exec();

            return 0;
        } else if(!strcmp(argv[x], "-clients"))  {
                        ClientCom com;

            // Connect to the runnin client instance
            com.socket.connectToHost(QLatin1String("127.0.0.1"), 3656);
            if(!com.socket.waitForConnected()) {
                //
                return 0;
            }


            com.sendCommand("GET_CLIENTS", "");

            QApplication::exec();

            return 0;

        } else if(!strcmp(argv[x], "-help")) {

            PrintHelp();

            ExitProcess(0);
            return 0;
        } else if(!strcmp(argv[x], "-version")) {
            printf("SSLVpnClient Version %s\n", g_szVersion);
            return 0;
        } else {
            cmdFound = false;
        }
    }

    // If the was an invalid command we just inform the user and exit
    if(!cmdFound) {
        printf("Unknown command\nExiting...\n");
        return 0;
    }

    // If there was just one argument [path to self] print the help
    if(argc == 1) {
        PrintHelp();
    }

    // Use different key so that the portable client does not prevent the installed client from starting
    QString key = "1x4z37";
    if(g_bPortable) {
        //
        key = "1x4z37Portable";
        //
        // Check for the visual c++ 2013 x86 restributable
        // This check must be adjusted for each new version
        QString keyPath ("HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\DevDiv\\vc\\Servicing\\12.0\\RuntimeMinimum");
        QSettings windowsRegistry (keyPath, QSettings::NativeFormat);
        // Check if its installed
        QString installedValue (windowsRegistry.value(QLatin1String("Install")).toString());
        //
        if (installedValue != QLatin1String("1")) {
            // No vcrest 2013 found
            Message::warning(QObject::tr("No Visual C++ Redistributable Package for Visual Studio 2013 x86 found. Please download and install it. Application is now exiting."), QObject::tr("Visual C++ Redistributable Packages for Visual Studio 2013"));

            // Exit
            return 1;
        }
    }

    a.setSharedKey(key);

    // Only check if its installed client instance
    // Check if SSL Vpn client already running
    if (!g_bPortable && a.isRunning()) {
        a.sendMessage("Someone is out there");
        return 0;
    }

    // Check if the service is already running, if not start it with -e
    if(g_bPortable)
    {
        // Start/check if netman is running, this is required, for windows 8 with hte portable client
        // The installer installs the service with a dependency to netman so it not required for a installed client
        if(!Utils::StartNetman()) {
            MessageBoxA(NULL, "Failed to start netman\nEither the program was not started as administrator or there was an error while starting netman", "Error", 0);
            return 0;
        }

        if(!Utils::IsVPNServiceRunning()) {
            QProcess *vpnService = new QProcess();
            vpnService->setWorkingDirectory(qApp->applicationDirPath());
            vpnService->start(QCoreApplication::applicationDirPath() + QLatin1String("/SPSSLVpnService.exe"), QStringList() << QLatin1String("-e"));
            if (!vpnService->waitForStarted(3000)) {
                qDebug() << "service not startet";
            }
        } else {
            std::cerr << "Already running" << std::endl;
        }
    }

    QSettings sett (Utils::userApplicationDataDirectory() + "/sslpnv2.ini", QSettings::IniFormat);

    QString cryptKey = sett.value(QLatin1String("self/key"), QLatin1String("")).toString();
    try {
        if (!cryptKey.isEmpty()) {
            cryptKey = QString(Crypt::decodeToPlaintext(cryptKey.toLatin1()));
        } else {
            // Neuen Key erzeugen
            qsrand(QDateTime::currentDateTime().toTime_t());
            QString _t1 (QString::number((qrand() % 1500) + 1));
            QString _t2 (QString::number((qrand() % 2500) + 1));
            QString key (QLatin1String("S3m!BHF") + _t1 + QLatin1String("83$%�kd") + _t2 + QString::number(QDateTime::currentDateTime().toTime_t()) + _t1);

            key = QString(Crypt::encodePlaintext(key));
            sett.setValue(QLatin1String("self/key"), key);
            cryptKey = key;

            cryptKey = Crypt::decodeToPlaintext(cryptKey.toLatin1());
        }
    } catch (std::runtime_error &e) {
        qDebug () << e.what();
        exit(1);
    }
    catch (...) {
        exit(1);
    }


    auto keks = qApp->libraryPaths();
    QApplication::setQuitOnLastWindowClosed(false);

    // Load the stylesheets
    QStringList styleSheetFiles;
    styleSheetFiles << QLatin1String("global")
        << QLatin1String("tabs")
        << QLatin1String("checkradio")
        << QLatin1String("scrollbar");

    // Loading the style files into one single string
    QString applicationStyleSheet;
    foreach (QString styleFileName, styleSheetFiles) {
        QFile styleFile (Utils::dataDirectory() + QLatin1String("/styles/") + styleFileName + QLatin1String(".qss"));
        //
        if (styleFile.open(QIODevice::ReadOnly)) {
            // Read the styles definition
            applicationStyleSheet += QLatin1String(styleFile.readAll());
        }
        //
        styleFile.close();
    }

    applicationStyleSheet += "QCheckBox::indicator {";
    applicationStyleSheet += "width: " + QString::number(floor(13 * windowsDpiScale())) + "px;";
    applicationStyleSheet += "height: " + QString::number(floor(13 * windowsDpiScale())) + "px;";
    applicationStyleSheet += "}";

    applicationStyleSheet += "QRadioButton::indicator {";
    applicationStyleSheet += "width: " + QString::number(floor(13 * windowsDpiScale())) + "px;";
    applicationStyleSheet += "height: " + QString::number(floor(13 * windowsDpiScale())) + "px;";
    applicationStyleSheet += "}";

    // Set the styles to the application
    a.setStyleSheet(applicationStyleSheet);

    QTranslator translator;
    QTranslator translator_default;

    // check if we have to load the german translation
    if(loadGermanTranslation) {
        // Load the german translation

        QString trans_file = QLatin1String("sslvpnclient_de.qm");
        QString trans_dialog_file = QLatin1String("qt_de.qm");


        translator_default.load(trans_dialog_file, QApplication::applicationDirPath());
        translator.load(trans_file, QApplication::applicationDirPath());
        a.installTranslator(&translator);
        a.installTranslator(&translator_default);
    }

    // Init database
    Database::instance();

    // Init settings
    Settings::instance()->loadOrRefresh();
    //
    if (loadGermanTranslation) {
        // Client should use german language
        Settings::instance()->setUseGerman();
    }

    startConfig.replace("\\", "/");

    // Remove the config
    Configs::instance()->cleanupConfigs();

     // Create main window
    FrmMain::instance();

    // Add the auto start configs (-start) to the database
    if(vecStartConfigs.size()) {
        for(auto &configData : vecStartConfigs) {
            Configs::instance()->addConfigToDatabase(configData);
        }
    }

    // Initialize the service connection stuff
    FrmMain::instance()->initDaemon();

    // Start async connect to service
    auto func = std::bind(&FrmMain::startDaemon, FrmMain::instance());
    auto handle = std::thread(func);
    handle.detach();

    // Speed up first double click
    FrmMain::instance()->showNormal();
    FrmMain::instance()->close();
    // Fix that menu mess
    FrmMain::instance()->setMenuToShowWindow();

    // Show the tray icon
    if(!g_bSilent) {
        FrmMain::instance()->showTrayIcon();
    }

    // If its always popup the show the window on startup
    if(Settings::instance()->alwaysPopup() && !g_bSilent) {
        FrmMain::instance()->showDiffieHellmanWarning();
        //
        FrmMain::instance()->showNormal();
    }

    auto r = QApplication::exec();

    // Cleanup on shutdown

    // If (-removeTap), then remove all tap devices on shutdown
    if(removeTap) {
        TapDriver::instance()->removeTapDriver();
    }

    // Remove the -start configs from the database
    if(vecStartConfigs.size()) {
        for(auto &configData : vecStartConfigs) {
            for(auto &config : Configs::instance()->getList()) {
                if(config.second->GetName() == configData.configName && config.second->GetConfigPath() == configData.configFile) {
                    Configs::instance()->removeFromDatabase(config.first);
                }
            }
        }
    }

    // A little hack to make the com wrapper a bit more reliable when exiting.
    printf("\n");
    fflush(stdout);

    Database::instance()->close();

    // Hack to fix crash at shutdown, this will prevent the call to ExitProcess
    TerminateProcess (GetCurrentProcess(), 0);

    return r;
}

// Define some manifest settings
#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

#pragma comment(lib, "ComCtl32.lib")
