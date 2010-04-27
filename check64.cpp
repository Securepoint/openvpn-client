#include "check64.h"

Check64 *Check64::mInst = NULL;

Check64 *Check64::getInstance() {
    if (!mInst)
        mInst = new Check64 ();
    return mInst;
}

Check64::Check64()
{
}

BOOL Check64::IsWow64() {
    BOOL bIsWow64 = FALSE;

    //IsWow64Process is not available on all supported versions of Windows.
    //Use GetModuleHandle to get a handle to the DLL that contains the function
    //and GetProcAddress to get a pointer to the function if available.

    fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
        GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

    if(NULL != fnIsWow64Process)
    {
        if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
        {
            //handle error
        }
    }
    return bIsWow64;
}

bool Check64::isRunning64Bit() {
    if (this->IsWow64())
        return true;
    else
        return false;
}
