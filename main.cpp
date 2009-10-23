
#include <QtGui>
#include <QTimer>
#include <QSplashScreen>

#include "window.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(systray);

    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("OpenVPN Client"),
                              QObject::tr("Es wurde kein Systray auf diesen "
                                          "System gefunden."));
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
    Window window;

    // Anwendung ohne Dialog starten
    return app.exec();
}
