#ifndef CHECKSYSTEM_H
#define CHECKSYSTEM_H
#ifdef Q_OS_WIN32
#include "c:/mingw/include/windows.h"
#endif


class CheckSystem
{
public:
    CheckSystem();
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    bool is64Bit ();
};

#endif // CHECKSYSTEM_H
