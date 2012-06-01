#ifndef CHECK64_H
#define CHECK64_H
#include <windows.h>

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

class Check64
{
public:    
    static bool isRunning64Bit ();

private:
    static LPFN_ISWOW64PROCESS fnIsWow64Process;
    static BOOL IsWow64();
    Check64();
};

#endif // CHECK64_H
