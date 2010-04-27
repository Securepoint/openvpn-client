#ifndef CHECK64_H
#define CHECK64_H
#include <windows.h>

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

class Check64
{
public:
    static Check64 *getInstance ();
    bool isRunning64Bit ();
private:
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    static Check64 *mInst;
    Check64();
    BOOL IsWow64();
};

#endif // CHECK64_H
