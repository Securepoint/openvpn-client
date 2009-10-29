
#include <QtGui>
#include <QTimer>
#include <QSplashScreen>
#include <QProcess>

#include "preferences.h"
#include "single_application.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(systray);

    SingleApplication app(argc, argv, "1x4z37");

    if (app.isRunning()){
        QMessageBox::critical(0, QObject::tr("OpenVPN Client"),
                              QObject::tr("Another instance of Securepoint OpenVPN client is already running!"));
        return 0;
    }


    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("OpenVPN Client"),
                              QObject::tr("No systray found on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);
    //QApplication::setStyle(new QPlastiqueStyle());


    // Splashscreen öffnen
    QPixmap pixmap(":/images/startscreen.jpg");
    QSplashScreen splash(pixmap);
    splash.show();
    app.processEvents();
    // Splash nach 3 Sekunden schließen
    QTimer::singleShot(3000, &splash, SLOT(close()));
    
    // Main Window
    Preferences window;
    //QObject::connect(&app, SIGNAL(messageAvailable(QString)), Preferences, SLOT(receiveMessage(QString)));

    // Anwendung ohne Dialog starten
    return app.exec();
}
