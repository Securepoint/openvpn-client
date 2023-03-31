#include "utils.h"
//#include <WinSock2.h>
#include <Windows.h>
#include <psapi.h>
#include <Shlobj.h>

#pragma comment (lib, "Advapi32.lib")

extern bool g_bPortable;

QString Utils::dataDirectory()
{
    //
    // Return the path to the data directory
    //

    //
    QDir dir (QCoreApplication::applicationDirPath());

    return dir.absolutePath() + QLatin1String("/data");
}

bool Utils::isLegalFileName(QString nameToCheck)
{

    if (nameToCheck.isEmpty()) {
        return false;
    }

    QMap<QString, int> invalidNames;
    //
    invalidNames.insert(QLatin1String("CON"), 1);
    invalidNames.insert(QLatin1String("PRN"), 2);
    invalidNames.insert(QLatin1String("AUX"), 3);
    invalidNames.insert(QLatin1String("NUL"), 4);
    //
    invalidNames.insert(QLatin1String("COM1"), 5);
    invalidNames.insert(QLatin1String("COM2"), 6);
    invalidNames.insert(QLatin1String("COM3"), 7);
    invalidNames.insert(QLatin1String("COM4"), 8);
    invalidNames.insert(QLatin1String("COM5"), 9);
    invalidNames.insert(QLatin1String("COM6"), 10);
    invalidNames.insert(QLatin1String("COM7"), 11);
    invalidNames.insert(QLatin1String("COM8"), 12);
    invalidNames.insert(QLatin1String("COM9"), 13);
    //
    invalidNames.insert(QLatin1String("LPT1"), 14);
    invalidNames.insert(QLatin1String("LPT2"), 15);
    invalidNames.insert(QLatin1String("LPT3"), 16);
    invalidNames.insert(QLatin1String("LPT4"), 17);
    invalidNames.insert(QLatin1String("LPT5"), 18);
    invalidNames.insert(QLatin1String("LPT6"), 19);
    invalidNames.insert(QLatin1String("LPT7"), 20);
    invalidNames.insert(QLatin1String("LPT8"), 21);
    invalidNames.insert(QLatin1String("LPT9"), 22);

    //
    if (invalidNames.contains(nameToCheck)) {
        return false;
    }

    // Normal char check
    LPCWSTR filename = (const wchar_t*) nameToCheck.utf16();
    WCHAR valid_invalid[MAX_PATH];
    wcscpy_s(valid_invalid, filename);

    int result = PathCleanupSpec(nullptr, valid_invalid);

    // If return value is non-zero, or if 'valid_invalid'
    // is modified, file-name is assumed invalid
    return result == 0 && wcsicmp(valid_invalid, filename)==0;
}

QString Utils::userApplicationDataDirectory()
{
    //
    // Returns the users appdata directory
    //

    if(g_bPortable)
    {
        QDir dir (QCoreApplication::applicationDirPath());
        return dir.absolutePath();
    }


    QString directoryPath (QString("%1")
                           .arg(enviromentValue("APPDATA")));

    directoryPath.append(QLatin1String("/Securepoint SSL VPN"));

    // Replace windows \ with qt /
    directoryPath.replace("\\", "/");

    //
    return directoryPath;
}

QString Utils::enviromentValue(const QString &name)
{
    //
    // Returns the value of the given enviroment variable
    //

    wchar_t *buffer = 0;
    size_t bufferSize = 0;

    QString value;
    // Read the enviroment variable
    if (_wdupenv_s(&buffer, &bufferSize, name.toStdWString().c_str()) == 0) {
        if (bufferSize > 0) {
            // Read and assign value
            // -1 is to remove the ending \0
            value = QString::fromWCharArray(buffer, bufferSize - 1);
        }

        // Cleanup
        delete buffer;
    }

    return value;
}

bool Utils::isUserAdmin ()
{
    struct Data
    {
        PACL   pACL;
        PSID   psidAdmin;
        HANDLE hToken;
        HANDLE hImpersonationToken;
        PSECURITY_DESCRIPTOR     psdAdmin;
        Data() : pACL(NULL), psidAdmin(NULL), hToken(NULL),
            hImpersonationToken(NULL), psdAdmin(NULL)
        {}
        ~Data()
        {
            if (pACL)
                LocalFree(pACL);
            if (psdAdmin)
                LocalFree(psdAdmin);
            if (psidAdmin)
                FreeSid(psidAdmin);
            if (hImpersonationToken)
                CloseHandle (hImpersonationToken);
            if (hToken)
                CloseHandle (hToken);
        }
    } data;

    BOOL   fReturn = FALSE;
    DWORD  dwStatus;
    DWORD  dwAccessMask;
    DWORD  dwAccessDesired;
    DWORD  dwACLSize;
    DWORD  dwStructureSize = sizeof(PRIVILEGE_SET);

    PRIVILEGE_SET   ps;
    GENERIC_MAPPING GenericMapping;
    SID_IDENTIFIER_AUTHORITY SystemSidAuthority = SECURITY_NT_AUTHORITY;

    const DWORD ACCESS_READ  = 1;
    const DWORD ACCESS_WRITE = 2;

    if (!OpenThreadToken (GetCurrentThread(), TOKEN_DUPLICATE|TOKEN_QUERY, TRUE, &data.hToken))
    {
        if (GetLastError() != ERROR_NO_TOKEN) {
            return false;
        }

        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE|TOKEN_QUERY, &data.hToken)) {
            return false;
        }
    }

    if (!DuplicateToken (data.hToken, SecurityImpersonation, &data.hImpersonationToken)) {
        return false;
    }

    if (!AllocateAndInitializeSid(&SystemSidAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &data.psidAdmin)) {
        return false;
    }

    data.psdAdmin = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
    if (data.psdAdmin == NULL) {
        return false;
    }

    if (!InitializeSecurityDescriptor(data.psdAdmin, SECURITY_DESCRIPTOR_REVISION)) {
        return false;
    }

    // Compute size needed for the ACL.
    dwACLSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(data.psidAdmin) - sizeof(DWORD);

    data.pACL = (PACL)LocalAlloc(LPTR, dwACLSize);
    if (data.pACL == NULL) {
        return false;
    }

    if (!InitializeAcl(data.pACL, dwACLSize, ACL_REVISION2)) {
        return false;
    }

    dwAccessMask = ACCESS_READ | ACCESS_WRITE;

    if (!AddAccessAllowedAce(data.pACL, ACL_REVISION2, dwAccessMask, data.psidAdmin)) {
        return false;
    }

    if (!SetSecurityDescriptorDacl(data.psdAdmin, TRUE, data.pACL, FALSE)) {
        return false;
    }

    // AccessCheck validates a security descriptor somewhat; set the group
    // and owner so that enough of the security descriptor is filled out
    // to make AccessCheck happy.

    SetSecurityDescriptorGroup(data.psdAdmin, data.psidAdmin, FALSE);
    SetSecurityDescriptorOwner(data.psdAdmin, data.psidAdmin, FALSE);

    if (!IsValidSecurityDescriptor(data.psdAdmin)) {
        return false;
    }

    dwAccessDesired = ACCESS_READ;

    GenericMapping.GenericRead    = ACCESS_READ;
    GenericMapping.GenericWrite   = ACCESS_WRITE;
    GenericMapping.GenericExecute = 0;
    GenericMapping.GenericAll     = ACCESS_READ | ACCESS_WRITE;

    if (!AccessCheck(data.psdAdmin, data.hImpersonationToken, dwAccessDesired, &GenericMapping, &ps, &dwStructureSize, &dwStatus, &fReturn)){
        return false;
    }

    return fReturn;
}

bool Utils::isX64Platform()
{
    //
    // Returns true if we are running on a x64 platform
    // Needed at leas XP wit SP 2
    //

    auto Isx64 = []() -> bool {
        typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

        LPFN_ISWOW64PROCESS fnIsWow64Process;

        BOOL bIsWow64 = FALSE;

        fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
            GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

        if(NULL != fnIsWow64Process) {
            if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)) {
                // handle error
            }
        }

        return (bIsWow64 == TRUE);
    };

    return Isx64();
}

bool Utils::isWindows10Platform()
{
    NTSTATUS(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
    OSVERSIONINFOEXW osInfo;

    *(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

    if (RtlGetVersion) {
        osInfo.dwOSVersionInfoSize = sizeof(osInfo);
        RtlGetVersion(&osInfo);

        if (osInfo.dwMajorVersion < 10.0) {
            return false;
        }
    }

    return true;
}

QString Utils::windowsVersion()
{
    NTSTATUS(WINAPI *RtlGetVersion)(LPOSVERSIONINFOEXW);
    OSVERSIONINFOEXW osInfo;

    *(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

    if (RtlGetVersion) {
        osInfo.dwOSVersionInfoSize = sizeof(osInfo);
        RtlGetVersion(&osInfo);

        return QString("%1.%2.%3")
                .arg(QString::number(osInfo.dwMajorVersion))
                .arg(QString::number(osInfo.dwMinorVersion))
                .arg(QString::number(osInfo.dwBuildNumber));
    }

    return QString("unknown");
}


Utils::Utils()
{
}

std::string Utils::GetProcessNameFromPID( DWORD processID )
{
    char szProcessName[MAX_PATH] = ("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
             &cbNeeded) )
        {
            GetModuleBaseNameA( hProcess, hMod, szProcessName,
                               sizeof(szProcessName)/sizeof(char) );
        }
    }

    // Release the handle to the process.

    CloseHandle( hProcess );

    return szProcessName;
}


bool Utils::IsProcessRunning(const char* processName)
{
    DWORD processList[1024];
    DWORD bytesofReturningList;
    DWORD processCount;

    if (!EnumProcesses(processList, sizeof(processList), &bytesofReturningList)) {
        //
        return true;
    }

    // Calculate how many process identifiers were returned.
    processCount = bytesofReturningList / sizeof(DWORD);

    // Check the name for each process.
    for (int processIndex = 0; processIndex < processCount; ++processIndex) {
        if(processList[processIndex] == 0) {
            continue;
        }

        // Get the name of the pid
        if (GetProcessNameFromPID(processList[processIndex]) == processName) {
            //
            return true;
        }
    }

    // Default return is false
    return false;
}

bool Utils::IsVPNServiceRunning()
{
    // Get a handle to the service manager
    // Params: local computer,SERVICES_ACTIVE_DATABASE(default) and read only
    SC_HANDLE handleOfServiceManager =  OpenSCManagerA(0, 0, GENERIC_READ);
    // No handle, service is not installed, check the running state of the serive executable.
    // Maybe we are in portable mode or the service manager is still not ready yet
    if(handleOfServiceManager == 0) {
        // Return the state if the process is running
        return IsProcessRunning("SPSSLVpnService.exe");
    }

    // Handle to service manager is valid, try to open the service
    // Params: Handle of service manager, service name and read only access
    SC_HANDLE handleOfService = OpenServiceA(handleOfServiceManager, "Securepoint VPN", GENERIC_READ);

    // Check if we got the handle
    if(handleOfService == 0) {
        // Close service manager
        CloseServiceHandle(handleOfServiceManager);
        //
        return false;
    }

    // Default it is running, otherwise we set the state later
    bool serviceIsRunning (true);

    // Service handle is valid, now get the status
    // Params: Service handle, status type info, target struct, size of target struct, bytes returned
    SERVICE_STATUS_PROCESS serviceInfo;
    DWORD bytesNeeded = 0;
    //
    if(QueryServiceStatusEx(handleOfService, SC_STATUS_PROCESS_INFO, (LPBYTE)&serviceInfo, sizeof(serviceInfo), &bytesNeeded) == 0) {
        serviceIsRunning = false;
    }

    // If the service is not running, check the state of the process
    if(serviceInfo.dwCurrentState != SERVICE_RUNNING) {
        serviceIsRunning = IsProcessRunning("SPSSLVpnService.exe");
    }

    // Cleanup service and manager handle
    CloseServiceHandle(handleOfService);
    CloseServiceHandle(handleOfServiceManager);

    //
    return serviceIsRunning;
}

bool Utils::StartNetman()
{
    SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (schSCManager == NULL) {
        return FALSE;
    }

    SC_HANDLE schService  = OpenService(schSCManager, L"Netman", SERVICE_ALL_ACCESS);
    if (schService == NULL) {
        CloseServiceHandle(schSCManager);
        return FALSE;
    }


    BOOL ret = StartService(schService, 0, NULL);
    DWORD dwError = GetLastError();

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);

    return (ret || dwError == ERROR_SERVICE_ALREADY_RUNNING);
}
