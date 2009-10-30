#include "appfunc.h"

AppFunc::AppFunc()
{
}

bool AppFunc::isAppPortable() {
    #ifdef Q_OS_WIN32
       // Portabel oder nicht?
       QFile appSettings (QApplication::applicationDirPath() + QString("/setup.txt"));
       if (appSettings.exists()) {
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

QString AppFunc::getAppSavePath() {
    #ifdef Q_OS_WIN32
        if (!this->isAppPortable()) {
            return QString(getenv("APPDATA")) + QString("/Securepoint VPN");
        } else {
            return QApplication::applicationDirPath() + QString("/data");
        }
    #elif Q_OS_Unix
         // Unix Code
    #elif Q_OS_Mac
       // Mac code
    #endif
}
