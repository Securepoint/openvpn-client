#ifndef UTILS_H
#define UTILS_H

#include <QtCore>

//#include <WinSock2.h>
#include <Windows.h>

class Utils
{
public:
    static QString dataDirectory ();
    static QString userApplicationDataDirectory();
    static QString enviromentValue (const QString &name);
    static bool isUserAdmin ();
    static bool isX64Platform ();

    static bool IsProcessRunning(const char* processName);
    static std::string GetProcessNameFromPID(DWORD processID);
    static bool IsVPNServiceRunning();
    static bool StartNetman();
private:
    Utils();
};

#endif // UTILS_H
