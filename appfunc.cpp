#include "appfunc.h"

AppFunc::AppFunc()
{
}

bool AppFunc::isAppPortable() {
    #ifdef Q_OS_WIN32
       // Portabel oder nicht?
       QSettings appSettings;
       if (appSettings.value("HKEY_LOCAL_MASCHINE\\SOFTWARE\\Securepoint\\OpenVPN").toString() != "") {
           // Installation
           return false;
       } else {
           // Portable
           return true;
       }
    #elif Q_OS_Unix
         // Unix Code
    #elif Q_OS_Mac
       // Mac code
    #endif
}

QString AppFunc::getOS() {
    #ifdef Q_OS_WIN32
       return "WIN";
    #elif Q_OS_Unix
       return "UNIX";
    #elif Q_OS_Mac
       return "MAC";
    #endif
}
