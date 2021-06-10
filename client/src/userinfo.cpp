#include "userinfo.h"

#include <Windows.h>
#include <Windowsx.h>
#include <Lmcons.h>
#include <WtsApi32.h>
#include <sddl.h>
#pragma comment(lib,"wtsapi32.lib")

UserInfo* UserInfo::thisInstance = nullptr;
UserInfo* UserInfo::instance() {
    if (!thisInstance) {
        thisInstance = new UserInfo();
    }

    return thisInstance;
}

UserInfo::UserInfo() {
    // Get the current thread user
    TCHAR username[32767];
    DWORD username_len = 32767;
    //
    GetUserName(username, &username_len);
    this->threadUsername = QString::fromWCharArray(username);

    // Now the user of the login session
    LPTSTR sessionLogonUser = nullptr;
    DWORD sessionId = -1;
    DWORD sessionNameLength = 0;
    //
    if (ProcessIdToSessionId(GetCurrentProcessId(), &sessionId)) {
        if (WTSQuerySessionInformation(WTS_CURRENT_SERVER, sessionId, WTSUserName, &sessionLogonUser, &sessionNameLength)) {
            this->logonUsername = QString::fromWCharArray(sessionLogonUser);
        }
    }

    // SSID threat
    BYTE bThreadSID[SECURITY_MAX_SID_SIZE];
    DWORD dwThreadSidSize = SECURITY_MAX_SID_SIZE;
    LPTSTR szThreadSID;

    TCHAR szThreadDomainName[256];
    DWORD dwThreadDomainSize = 256;

    SID_NAME_USE snuThread = SidTypeInvalid;

    if (LookupAccountName(
        NULL, //use this system
        username, //the user to look up
        bThreadSID, //the returned SID
        &dwThreadSidSize, //the size of the SID returned
        szThreadDomainName, //the returned domain name
        &dwThreadDomainSize, //the size of the domain name
        &snuThread //the type of sid
        )){
        ConvertSidToStringSid(bThreadSID, &szThreadSID);
        this->threadSID = QString::fromWCharArray(szThreadSID);
        this->threadDomain = QString::fromWCharArray(szThreadDomainName);
    }


    // SSID threat
    BYTE bLogonSID[SECURITY_MAX_SID_SIZE];
    DWORD dwLogonSidSize = SECURITY_MAX_SID_SIZE;
    LPTSTR szLogonSID;

    TCHAR szLogonDomainName[256];
    DWORD dwLogonDomainSize = 256;

    SID_NAME_USE snuLogon = SidTypeInvalid;

    if (LookupAccountName(
        NULL, //use this system
        sessionLogonUser, //the user to look up
        bLogonSID, //the returned SID
        &dwLogonSidSize, //the size of the SID returned
        szLogonDomainName, //the returned domain name
        &dwLogonDomainSize, //the size of the domain name
        &snuLogon //the type of sid
        )){
        ConvertSidToStringSid(bLogonSID, &szLogonSID);
        this->logonSID = QString::fromWCharArray(szLogonSID);
        this->logonDomain = QString::fromWCharArray(szLogonDomainName);
    }
    //
    LocalFree(szThreadSID);
    szThreadSID = NULL;
    LocalFree(szLogonSID);
    szLogonSID = NULL;
    //
    WTSFreeMemory(sessionLogonUser);
}
