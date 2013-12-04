#include "service.h"
#include "debug.h"
#include "srvcli.h"

Service::Service(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "Securepoint VPN"),
      server (0)
{
    setServiceDescription("OpenVPN wrapper to connect to an OpenVPN server without administrator permission");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

void Service::start()
{
    // Den Port einlesen und den SSL-Server starten

    // Ini-Datei öffenen
    QSettings serviceSettings (QCoreApplication::applicationDirPath() + QLatin1String("/service.ini"), QSettings::IniFormat);
    //
    quint16 myPort (serviceSettings.value(QLatin1String("ports/service"), 3656).toInt());
    // Client port
    quint16 clientPort (serviceSettings.value(QLatin1String("ports/client"), 3655).toInt());

    // Debug an/ ausschalten
    Debug::setDebugPath(QCoreApplication::applicationDirPath());
    Debug::enableDebugging(serviceSettings.value(QLatin1String("debug/state"), 0).toBool());
    int debLev (serviceSettings.value(QLatin1String("debug/level"), 9).toInt());
    switch (debLev) {
        case 1:
            Debug::setDebugLevel(DebugLevel::Error);
            break;
        case 2:
            Debug::setDebugLevel(DebugLevel::Category);
            break;
        case 3:
            Debug::setDebugLevel(DebugLevel::Function);
            break;
        case 4:
            Debug::setDebugLevel(DebugLevel::Comment);
            break;
        case 5:
            Debug::setDebugLevel(DebugLevel::Database);
            break;
        case 6:
            Debug::setDebugLevel(DebugLevel::Debug);
            break;
        case 7:
            Debug::setDebugLevel(DebugLevel::Construktor);
            break;
        case 8:
            Debug::setDebugLevel(DebugLevel::Destruktor);
            break;
        case 9:
            Debug::setDebugLevel(DebugLevel::All);
            break;
        case 0:
        default:
            Debug::setDebugLevel(DebugLevel::None);
            break;
    }

    Debug::enableDateTime(serviceSettings.value(QLatin1String("debug/date"), 1).toBool());
    Debug::enableMSecs(serviceSettings.value(QLatin1String("debug/msecs"), 1).toBool());



    // Den Client Port in der SrvCli setzen
    SrvCLI::instance()->setPort(clientPort);

    Debug::log(QString("Starting service on port %1").arg(myPort));
    Debug::log(QString("Client is listing on port %1").arg(clientPort));

    // SSL Server starten
    this->server = new SslServer(myPort, this);
}

void Service::stop()
{

}

void Service::pause()
{

}

void Service::resume()
{

}

