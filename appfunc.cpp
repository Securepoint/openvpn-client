#include "appfunc.h"
#include "settings.h"

AppFunc::AppFunc()
{
}

bool AppFunc::isAppPortable() {
    // Die Dienstanwendung darf nur als Setupversion sein.
    // Wegen dem Dienst, könnte eine Portabel Verwirrung
    // beim dem Benutzer stiften ;)
    return Settings::getInstance()->getIsPortableClient();
}

QString AppFunc::getOS() {
#ifdef Q_OS_WIN32
    return QLatin1String("WIN");
#elif Q_OS_Unix
       return "UNIX";
    #elif Q_OS_Mac
       return "MAC";
#endif
}

QString AppFunc::getAppSavePath() {
    // Gibt das Configverzeichnis zurück
    // Dieses ist standardmäßig in User/Appdata/
    QString retVal (QLatin1String(getenv("APPDATA")) + QLatin1String("/Securepoint SSL VPN"));
#ifdef Q_OS_WIN32
        if (isAppPortable())
            retVal = QApplication::applicationDirPath();
#elif Q_OS_Unix
         // Unix Code
    #elif Q_OS_Mac
       // Mac code
#endif

    return retVal;
}
