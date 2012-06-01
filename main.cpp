
#include <QtCore>
#include <QtGui>

#include "preferences.h"
#include "single_application.h"

// Settings Singleton
#include "settings.h"
#include "message.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(systray);

    // Die Anwenung ist eine Single Application,
    // wenn eine Instanz schon offen ist,
    // die neue geschlossen und von der bereits
    // vorhandenen das Statusfenster geöffnet
    SingleApplication app(argc, argv, "1x4z37");

    if (app.isRunning()){
        app.sendMessage("Someone is out there");
        return 0;
    }
    // Config aus Textdatei auslesen
    // Commandline Parameter haben eine höhere Prio als


    // Plugins laden
    app.addLibraryPath(app.applicationDirPath() + QDir::separator() + QLatin1String("plugins"));

    // RC 4: Einstellungen werden aus einer config gelesen
    //
    Settings::getInstance();
    Settings::getInstance()->refresh();

    // Commandline Parameter übergeben
    for (int x = 0; x < argc; x++) {
        if (0 == strcmp (argv[x], "-start") && x + 1 < argc) {
            // Config starten
            Settings::getInstance()->setIsStartCommandConfig(true);
            Settings::getInstance()->setCommandConfigPath(argv[x + 1]);
        } else if (0 == strcmp(argv[x], "-noService")) {
            // Es soll kein Dienst verwendet werden
            Settings::getInstance()->setIsRunAsService(false);
        } else if (0 == strcmp(argv[x], "-reconnect")) {
            // Reconnect beim Trennen
            Settings::getInstance()->setIsAutoReconnect(true);
        } else if (0 == strcmp(argv[x], "-noBallon")) {
            // Keine Ballon Messages im Systray anzeigen
            Settings::getInstance()->setIsShowNoBallonMessage(true);
        } else if (0 == strcmp(argv[x], "-noPopUp")) {
            // Kein Start Popup
            Settings::getInstance()->setPopUpDialog(false);
        } else if (0 == strcmp(argv[x], "-noSplash")) {
            // Kein Start Splashscreen
            Settings::getInstance()->setShowSplashScreen(false);
        } else if (0 == strcmp(argv[x], "-port") && x + 1 < argc) {
            // Der POrt für den Client
            Settings::getInstance()->setListenPort(static_cast<quint16>(QString(argv[x + 1]).toInt()));
        } else if (0 == strcmp(argv[x], "-servicePort") && x + 1 < argc) {
            // Der Port für den Service
            Settings::getInstance()->setServerPort(static_cast<quint16>(QString(argv[x + 1]).toInt()));
        } else if (0 == strcmp(argv[x], "-searchDir") && x + 1 < argc) {
            // Ein Verzeichnis welches beim Starten durchsucht werden soll
            Settings::getInstance()->setIsStartConfigDir(true);
            Settings::getInstance()->setStartConfigDirPath(argv[x + 1]);
        } else if (0 == strcmp(argv[x], "-useEnglish")) {
            // Client st in Englisch
            Settings::getInstance()->setIsLanguageGerman(false);
        } else if (0 == strcmp(argv[x], "-useNoInteract")) {
            // Für OTP
            Settings::getInstance()->setUseNoInteract(true);
        } else if (0 == strcmp(argv[x], "-portable")) {
            // CLient installiert TAP
            Settings::getInstance()->setIsPortableClient(true);
        } else if (0 == strcmp(argv[x], "-manage")) {
            // Benutzer kann Einstellungen ändern
            Settings::getInstance()->setIsManageClient(true);
        }
    }
    // Portabel bauen
    //Settings::getInstance()->setIsManageClient(true);
    //Settings::getInstance()->setIsPortableClient(true);
    // Wenn Portable ist, immer mit lokalen VPN verbinden
    if (Settings::getInstance()->getIsPortableClient()) {
        Settings::getInstance()->setIsRunAsService(false);
    }

    // Die Übersetzungsdatei laden
    QString trans_file ("SpSSLVpn_eng.qm");
    QString trans_dialog_file ("qt_eng.qm");
    if (Settings::getInstance()->getIsLanguageGerman()) {
        trans_file = QLatin1String("SpSSLVpn_ger.qm");
        trans_dialog_file = QLatin1String("qt_de.qm");
    }

    QTranslator translator;
    translator.load(trans_dialog_file, QApplication::applicationDirPath());
    translator.load(trans_file, QApplication::applicationDirPath());
    app.installTranslator(&translator);

    // Prüfen, ob der Systray vorhanden ist,
    // dies muss bei !Windows nicht immer gegeben sein
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        Message::error(QObject::tr("No systray found on this system."), QObject::tr("Securepoint SSL VPN"));
        return 1;
    }

    // Die Anwendung soll nicht geschlossen werden, weil das Hauptfenster geschlossen wird
    // Appende ist nicht mit close sondern mit exit zu machen.
    QApplication::setQuitOnLastWindowClosed(false);    

    // Splashscreen öffnen
    QPixmap pixmap(":/images/startscreen.jpg");
    QSplashScreen splash(pixmap);
    if (Settings::getInstance()->showSplashScreen()) {
        splash.show();
        app.processEvents();
        // Splash nach 3 Sekunden schließen
        QTimer::singleShot(3000, &splash, SLOT(close()));
    }
    
    // Main Window    
    // Die Verbindung soll über den Dienst hergestellt werden
    // Es muss der Daemon gestartet werden, der versucht eine Verbindung zum
    // Dienst herzustellen. Bei einem Fehler erscheint eine Ballonmeldung
    if (Settings::getInstance()->getIsRunAsSevice()) {
        if (!Preferences::instance()->startDaemon()) {
            Message::error(QObject::tr("Can't connect to the ssl vpn service. Application is shuting down!"));
            return 1;
        }
    }   

    // Konfigurationen suchen
    Preferences::instance()->searchStartConfigDir();

    // In der RefreshDialog befinden sich die Aktualisierung der Oberfläche
    Preferences::instance()->refreshDialog();

    QObject::connect(&app, SIGNAL(messageAvailable(QString)), &app, SLOT(receiveMessage(QString)));

    if (Settings::getInstance()->getIsStartCommandConfig()) {
        // Wenn eine Config angeben ist, immer das Fenster öffenen
        Preferences::instance()->openDialog(Settings::getInstance()->getIsStartCommandConfig(), Settings::getInstance()->getCommandConfigPath().replace("\\", "/"));
    }

    // Anwendung ohne Dialog starten
    return app.exec();
}
