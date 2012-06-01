#include <QtCore>
#include "service.h"

int main(int argc, char **argv)
{
    // Kurz und knapp :)
    Service vpnservice (argc, argv);
    return vpnservice.exec();
}
