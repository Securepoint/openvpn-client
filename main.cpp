
#include <QtGui>
#include <QTimer>
#include <QSplashScreen>
#include <QProcess>
#include <QTranslator>

#include "preferences.h"
#include "single_application.h"

// Settings Singleton
#include "settings.h"

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
    // config.txt Parameter


    // Commandline Parameter übergeben
    for (int x = 0; x < argc; x++) {
        if (0 == strcmp (argv[x], "-start") && x+1 < argc) {
            // Config starten
            Settings::getInstance()->setIsStartCommandConfig(true);
            Settings::getInstance()->setCommandConfigPath(argv[x+1]);
        } else if (0 == strcmp(argv[x], "-noService")) {
            Settings::getInstance()->setIsRunAsService(false);
        } else if (0 == strcmp(argv[x], "-reconnect")) {
            Settings::getInstance()->setIsAutoReconnect(true);
        } else if (0 == strcmp(argv[x], "-noUserAuth")) {
            Settings::getInstance()->setIsShowNoUserAuth(true);
        } else if (0 == strcmp(argv[x], "-noBallon")) {
            Settings::getInstance()->setIsShowNoBallonMessage(true);
        } else if (0 == strcmp(argv[x], "-user") && x+1 < argc) {
            Settings::getInstance()->setIsStartWithUser (true);
            Settings::getInstance()->setStartWithUserName (argv[x+1]);
        } else if (0 == strcmp(argv[x], "-pwd") && x+1 < argc) {
            Settings::getInstance()->setIsStartWithPwd(true);
            Settings::getInstance()->setStartWithPwdPwd(argv[x+1]);
        } else if (0 == strcmp(argv[x], "-port") && x+1 < argc) {
            Settings::getInstance()->setListenPort((quint16) QString(argv[x+1]).toInt());
        } else if (0 == strcmp(argv[x], "-servicePort") && x+1 < argc) {
            Settings::getInstance()->setServerPort((quint16) QString(argv[x+1]).toInt());
        } else if (0 == strcmp(argv[x], "-searchDir") && x+1 < argc) {
            Settings::getInstance()->setIsStartConfigDir(true);
            Settings::getInstance()->setStartConfigDirPath(argv[x+1]);
        } else if (0 == strcmp(argv[x], "-noStartPopUp")) {
            Settings::getInstance()->setIsNoPopUp(true);
        } else if (0 == strcmp(argv[x], "-useCryptedData")) {
            Settings::getInstance()->setUseCryptedData(true);
        } else if (0 == strcmp(argv[x], "-useEnglish")) {
            Settings::getInstance()->setIsLanguageGerman(false);
        } else if (0 == strcmp(argv[x], "-useNoInteract")) {
            Settings::getInstance()->setUseNoInteract(true);
        } else if (0 == strcmp(argv[x], "-portable")) {
            Settings::getInstance()->setIsPortableClient(true);
        } else if (0 == strcmp(argv[x], "-manage")) {
            Settings::getInstance()->setIsManageClient(true);
        }
    }

    // Wenn Portable ist, immer mit lokalen VPN verbinden
    if (Settings::getInstance()->getIsPortableClient())
        Settings::getInstance()->setIsRunAsService(false);

    QString trans_file = "SpSSLVpn_eng.qm";
    QString trans_dialog_file = "qt_eng.qm";
    if (Settings::getInstance()->getIsLanguageGerman()) {
        trans_file = "SpSSLVpn_ger.qm";
        trans_dialog_file = "qt_de.qm";
    }
    QTranslator translator;
    translator.load(trans_dialog_file, QApplication::applicationDirPath());
    translator.load(trans_file, QApplication::applicationDirPath());
    app.installTranslator(&translator);
    // Prüfen, ob der Systray vorhanden ist,
    // dies muss bei !Windows nicht immer gegeben sein
    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("Securepoint SSL VPN"),
                              QObject::tr("No systray found on this system."));
        return 1;
    }
    // Die Anwendung soll nicht geschlossen werden, weil das Hauptfenster geschlossen wird
    // Appende ist nicht mit close sondern mit exit zu machen.
    QApplication::setQuitOnLastWindowClosed(false);

    // Splashscreen öffnen
    QPixmap pixmap(":/images/startscreen.jpg");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();
    // Splash nach 3 Sekunden schließen
    QTimer::singleShot(3000, &splash, SLOT(close()));
    
    // Main Window
    Preferences window;            
    // Die Verbindung soll über den Dienst hergestellt werden
    // Es muss der Daemon gestartet werden, der versucht eine Verbindung zum
    // Dienst herzustellen. Bei einem Fehler erscheint eine Ballonmeldung
    if (Settings::getInstance()->getIsRunAsSevice())
        window.startDaemon();

    window.searchStartConfigDir();

    // In der RefreshDialog befinden sich die Aktualisierung der Oberfläche
    window.refreshDialog();

    app.prefParent = &window;
    QObject::connect(&app, SIGNAL(messageAvailable(QString)), &app, SLOT(receiveMessage(QString)));

    if (Settings::getInstance()->getIsStartCommandConfig()) {
        // Wenn eine Config angeben ist, immer das Fenster öffenen
        window.openDialog(Settings::getInstance()->getIsStartCommandConfig(), Settings::getInstance()->getCommandConfigPath());
    }

    // Anwendung ohne Dialog starten
    return app.exec();
}
