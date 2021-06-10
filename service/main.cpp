#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMovie>
#include <QtCore/qstack.h>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include "service.h"

bool g_bPortable = false;

int main(int argc, char **argv)
{
    for (int x = 0; x < argc; x++) {
        if(!strcmp(argv[x], "-e")) {
            g_bPortable = true;
        }
    }    

    // Kurz und knapp :)
    Service vpnservice (argc, argv);
    return vpnservice.exec();
}
