#include "service.h"
#include "debug.h"
#include "srvcli.h"
#include "settings.h"

#include <QtCore\qsettings.h>

Service * Service::_sInstance = nullptr;

Service::Service(int argc, char **argv)
    : QtService<QCoreApplication>(argc, argv, "Securepoint VPN"),
      server (0)
{

    _sInstance = this;

    setServiceDescription("OpenVPN wrapper to connect to an OpenVPN server without administrator permission");
    setServiceFlags(QtServiceBase::CanBeSuspended);
}

void Service::start()
{
    // Den Port einlesen und den SSL-Server starten

    // Ini-Datei öffenen
    QSettings serviceSettings (QCoreApplication::applicationDirPath() + QLatin1String("/service.ini"), QSettings::IniFormat);

    //
    int tapDeviceCountTimeout (serviceSettings.value(QLatin1String("tap/count"), 20000).toInt());
    int tapInstallTimeout (serviceSettings.value(QLatin1String("tap/install"), 180000).toInt());
    int tapRemoveTimeout (serviceSettings.value(QLatin1String("tap/remove"), 60000).toInt());
    int tapCheckTimeout (serviceSettings.value(QLatin1String("tap/check"), 30000).toInt());

    Settings::instance()->setTapDeviceTimeout(tapDeviceCountTimeout);
    Settings::instance()->setTapInstallTimeout(tapInstallTimeout);
    Settings::instance()->setTapRemoveTimeout(tapRemoveTimeout);
    Settings::instance()->setTapCheckTimeout(tapCheckTimeout);

    //
    quint16 myPort (serviceSettings.value(QLatin1String("ports/service"), 3656).toInt());

    // Debug an/ ausschalten
    Debug::setDebugPath(QCoreApplication::applicationDirPath());

#if _DEBUG
    Debug::enableDebugging(serviceSettings.value(QLatin1String("debug/state"), 1).toBool());
    int debLev (serviceSettings.value(QLatin1String("debug/level"), 9).toInt());
#else
        Debug::enableDebugging(serviceSettings.value(QLatin1String("debug/state"), 0).toBool());
        int debLev (serviceSettings.value(QLatin1String("debug/level"), 6).toInt());
#endif
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

    Debug::log(QString("Starting service on port %1").arg(myPort));

    // Print tap timout settings to debug log
    Debug::log(QString("Device count timeout: %1").arg(Settings::instance()->tapDeviceTimeout()));
    Debug::log(QString("Device install timeout: %1").arg(Settings::instance()->tapInstallTimeout()));
    Debug::log(QString("Device remove timeout: %1").arg(Settings::instance()->tapRemoveTimeout()));
    Debug::log(QString("Device check timeout: %1").arg(Settings::instance()->tapCheckTimeout()));

    // SSL Server starten
    this->server = new SslServer(myPort, this);
}

void Service::stop()
{
    this->server->close();
}

void Service::pause()
{

}

void Service::resume()
{

}

