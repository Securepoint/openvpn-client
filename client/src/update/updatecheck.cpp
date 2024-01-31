#include "updatecheck.h"

#include <QMessageBox>
#include <QtCore>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QAuthenticator>
#include <QMessageBox>
#include <ui_sslcertsdialog.h>
#include <ui_authenticationdialog.h>
#include <utils.h>
#include <crypt.h>
#include <frmmain.h>
#include <message.h>
#include <proxysettings.h>
#include <sslcertsdialog.h>
#include <debug/debug.h>
#include <iostream>

#include <tchar.h>
#include <wininet.h>
#pragma comment (lib, "Wininet.lib")

UpdateCheck::UpdateCheck()
    : currentMajor(2),
      currentMinor(0),
      currentPatch(41)
{


}

UpdateCheck::~UpdateCheck()
{

}


void UpdateCheck::finished(QString response)
{
        Debug::log(QLatin1String("UpdateCheck::finished aufgerufen"));

        response.remove("\n");
        response = response.trimmed();
        // String needs 3 dot delimitered values major.minor.patch
        if (response.isEmpty()) {
            //
            qDebug() << "invalid respone(empty)";
            Debug::log(QLatin1String("invalid respone(empty)"));
            //
            return;
        }

        QStringList valueList (response.split("."));
        // Check if its the right format
        if (valueList.size() != 3) {
            //
            qDebug() << "invalid value list size";
            Debug::log(QLatin1String("invalid value list size"));
            //
            return;
        }

        bool majorOkay;
        int major (valueList.value(0).toInt(&majorOkay));
        //
        if (!majorOkay) {
            //
            qDebug() << "invalid major";
                    Debug::log(QLatin1String("invalid major"));
            //
            return;
        }

        bool minorOkay;
        int minor (valueList.value(1).toInt(&minorOkay));
        //
        if (!minorOkay) {
            //
            qDebug() << "invalid minor";
            Debug::log(QLatin1String("invalid minor"));
            //
            return;
        }

        bool patchOkay;
        int patch (valueList.value(2).toInt(&patchOkay));
        //
        if (!patchOkay) {
            //
            qDebug() << "invalid patch";
            Debug::log(QLatin1String("invalid patch"));
            //
            return;
        }

        // Is a newer version available?
        Debug::log(QLatin1String("Is a newer version available?"));
        bool newerVersionAvailable(false);
        //
        if (major > this->currentMajor) {
            newerVersionAvailable = true;
        }

        //
        if (minor > this->currentMinor) {
            newerVersionAvailable = true;
        }

        //
        if (patch > this->currentPatch) {
            newerVersionAvailable = true;
        }

        //
        if (newerVersionAvailable) {
            //
            Debug::log(QLatin1String("newerVersionAvailable"));
            emit updateAvailable((QString::number(major) + "." + QString::number(minor) + "." + QString::number(patch)));

        }
}

void UpdateCheck::onError(QNetworkReply::NetworkError code)
{
    qDebug() << "onError: " << code;
}

QString* UpdateCheck::proxyAuthenticationRequired()
{
    Debug::log(QLatin1String("UpdateCheck::proxyAuthenticationRequired aufgerufen"));

    QString* loginData = new QString[2];

    // Ask the user for the proxy credentials
    QDialog credentialsDialog;
    Ui::Dialog credentialsUi;
    // Assign the ui to the raw dialog
    credentialsUi.setupUi(&credentialsDialog);
    // Resize the dialog
    credentialsDialog.adjustSize();
    // Init dialog
    Debug::log(QLatin1String("init dialog"));
    credentialsUi.passwordEdit->setEchoMode(QLineEdit::Password);
    credentialsUi.siteDescription->setText(QObject::tr("<qt>Connect to proxy \"%1\":</qt>").arg(QString(proxy.hostName()).toHtmlEscaped()));

    // Set the style
    Debug::log(QLatin1String("set Style"));
    credentialsUi.buttonBox->setStyleSheet(QLatin1String("color: #ffffff;"));
    credentialsUi.siteDescription->setStyleSheet(QLatin1String("color: #ffffff;"));
    credentialsUi.label->setStyleSheet(QLatin1String("color: #ffffff;"));
    credentialsUi.label_2->setStyleSheet(QLatin1String("color: #ffffff;"));
    credentialsUi.label_3->setStyleSheet(QLatin1String("color: #ffffff;"));
    credentialsUi.label_4->setStyleSheet(QLatin1String("color: #ffffff;"));

    // Do the user clicked okay
    Debug::log(QLatin1String("did user click ok?"));
    if (credentialsDialog.exec() != QDialog::Accepted) {
        // No
        Debug::log(QLatin1String("no"));
        return loginData;
    }

    loginData[0] = credentialsUi.userEdit->text();
    loginData[1] = credentialsUi.passwordEdit->text();

    return loginData;
}

void UpdateCheck::run()
{
    HINTERNET hIntSession = NULL;
    DWORD dwErr = (static_cast<long long>(1234));
    QString* loginData = NULL;
    bool https = false;

    if (!ProxySettings::instance()->settingsAvailable() || ProxySettings::instance()->proxyTypeInUse() == "CONFIG") {     // No Proxy

        //INTERNET_OPEN_TYPE_DIRECT Resolves all host names locally.
        hIntSession = ::InternetOpen(_T("MyApp"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

    }
    else if (ProxySettings::instance()->proxyTypeInUse() == "IE") {     // IE OR MANUAL

        // IE Settings
        // INTERNET_OPEN_TYPE_PRECONFIG Retrieves the proxy or direct configuration from the registry
        hIntSession = ::InternetOpen(_T("MyApp"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

    }
    else {     // Manual

        // Proxy
        // INTERNET_OPEN_TYPE_PROXY 	Passes requests to the proxy

        QString proxyString = "";
        if( ProxySettings::instance()->isHttpProxy() )
        {
                proxyString = "http=" + ProxySettings::instance()->proxyHost() + ":" + ProxySettings::instance()->proxyPort();
                Debug::log(QLatin1String("proxy: ") + proxyString);
        }
        else
        {
                proxyString = "SOCKS=" + ProxySettings::instance()->proxyHost() + ":" + ProxySettings::instance()->proxyPort();
                Debug::log(QLatin1String("proxy: ") + proxyString);
        }


        std::wstring proxy = proxyString.toStdWString();
        const wchar_t* szProxy= proxy.c_str();
        hIntSession = ::InternetOpen(_T("MyApp"), INTERNET_OPEN_TYPE_PROXY, szProxy, NULL, 0);

    }
    int retry = 3;

    HINTERNET hHttpSession =
      InternetConnect(hIntSession, _T("updatevpn.securepoint.de"), 80, 0, 0, INTERNET_SERVICE_HTTP , 0, NULL);

        // header vorbereiten
        QString version = QString(FrmMain::instance()->version.toLatin1());//.toStdWString();
        QString windows = QString(Utils::windowsVersion().toLatin1());
        QString bitness = QString(Utils::isX64Platform() ? QByteArray("x64") : QByteArray("x86"));

        // umwandeln in wchar
        QString qStr = "Content-Type: text/html\r\nde.securepoint.product: sslvpnv2\r\nde.securepoint.product.version: " + version + "\r\nde.securepoint.product.os: " + windows + "\r\nde.securepoint.product.arch: " + bitness + "\r\n\r\n";
        std::wstring headers = qStr.toStdWString();
        const wchar_t* szHeaders = headers.c_str();

        CHAR szReq[1024] = "";
        HINTERNET hResourceHandle;
        DWORD dwStatus;
        DWORD dwStatusSize = sizeof(dwStatus);

        // HttpOpenRequest
        hResourceHandle = HttpOpenRequestW(
          hHttpSession,
          _T("GET"),
          _T("/sslvpn-client/LATEST"),
          0, 0, 0,INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS , 0);

        // HttpSendRequest
        if( !HttpSendRequest(hResourceHandle, szHeaders, _tcslen(szHeaders), szReq, strlen(szReq))) {

            // abort if cert error
            dwErr = GetLastError();
            // stop if cert error
            if (dwErr == ERROR_INTERNET_SEC_CERT_DATE_INVALID || //12037
                dwErr == ERROR_INTERNET_SEC_CERT_ERRORS || // 12055
                dwErr == ERROR_INTERNET_INVALID_CA){ // 12045

                ::InternetCloseHandle(hResourceHandle);
                ::InternetCloseHandle(hHttpSession);
                ::InternetCloseHandle(hIntSession);

                emit updateAvailable("error");
                return;
                }
            } // end if

        dwErr = GetLastError();
        Debug::log(QLatin1String("ssl error2: ") + QString::number(dwErr));

        // HttpQueryInfo
        HttpQueryInfo(hResourceHandle, HTTP_QUERY_FLAG_NUMBER |
                      HTTP_QUERY_STATUS_CODE, &dwStatus, &dwStatusSize, NULL);

        // Switch to SSL
        if (dwStatus == HTTP_STATUS_BAD_REQUEST ) { // 400

            // close all handles so we can start fresh with INTERNET_FLAG_SECURE
            ::InternetCloseHandle(hResourceHandle);
            ::InternetCloseHandle(hHttpSession);
            ::InternetCloseHandle(hIntSession);

            if (!ProxySettings::instance()->settingsAvailable() || ProxySettings::instance()->proxyTypeInUse() == "CONFIG") {     // No Proxy

                //INTERNET_OPEN_TYPE_DIRECT Resolves all host names locally.
                hIntSession = ::InternetOpen(_T("MyApp"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

            }
            else if (ProxySettings::instance()->proxyTypeInUse() == "IE") {     // IE OR MANUAL

                // IE Settings
                // INTERNET_OPEN_TYPE_PRECONFIG Retrieves the proxy or direct configuration from the registry
                hIntSession = ::InternetOpen(_T("MyApp"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

            }
            else {     // Manual

                // Proxy
                // INTERNET_OPEN_TYPE_PROXY 	Passes requests to the proxy

                QString proxyString = "";
                if( ProxySettings::instance()->isHttpProxy() )
                {
                        proxyString = "http=" + ProxySettings::instance()->proxyHost() + ":" + ProxySettings::instance()->proxyPort();
                        Debug::log(QLatin1String("proxy: ") + proxyString);
                }
                else
                {
                        proxyString = "SOCKS=" + ProxySettings::instance()->proxyHost() + ":" + ProxySettings::instance()->proxyPort();
                        Debug::log(QLatin1String("proxy: ") + proxyString);
                }


                std::wstring proxy = proxyString.toStdWString();
                const wchar_t* szProxy= proxy.c_str();
                hIntSession = ::InternetOpen(_T("MyApp"), INTERNET_OPEN_TYPE_PROXY, szProxy, NULL, 0);

            }

            HINTERNET hHttpSession =
              InternetConnect(hIntSession, _T("updatevpn.securepoint.de"), 80, 0, 0, INTERNET_SERVICE_HTTP , 0, NULL);

            // HttpOpenRequest
            hResourceHandle = HttpOpenRequestW(
              hHttpSession,
              _T("GET"),
              _T("/sslvpn-client/LATEST"),
              0, 0, 0,INTERNET_FLAG_SECURE , 0);

            HttpSendRequest(hResourceHandle, szHeaders, _tcslen(szHeaders), szReq, strlen(szReq));
            dwStatus = NULL;

            HttpQueryInfo(hResourceHandle, HTTP_QUERY_FLAG_NUMBER |
                          HTTP_QUERY_STATUS_CODE, &dwStatus, &dwStatusSize, NULL);

                        // abort if cert error
                        dwErr = GetLastError();
                        if (dwErr == ERROR_INTERNET_SEC_CERT_DATE_INVALID || //12037
                            dwErr == ERROR_INTERNET_SEC_CERT_ERRORS || // 12055
                            dwErr == ERROR_INTERNET_INVALID_CA){ // 12045

                            ::InternetCloseHandle(hResourceHandle);
                            ::InternetCloseHandle(hHttpSession);
                            ::InternetCloseHandle(hIntSession);

                            emit updateAvailable("error");
                            return;
                        }
                        https = true;
        }


                do{

                        if (dwStatus == HTTP_STATUS_PROXY_AUTH_REQ || dwStatus == HTTP_STATUS_DENIED ) // 407 or 401
                        {

                            // close all handles so we can start fresh with the new login data
                            ::InternetCloseHandle(hResourceHandle);
                            ::InternetCloseHandle(hHttpSession);
                            ::InternetCloseHandle(hIntSession);

                            //get name and pw
                            loginData = proxyAuthenticationRequired();

                            // change to LPCTSTR
                            std::wstring proxyUserName = loginData[0].toStdWString();
                            const wchar_t* szProxyUserName = proxyUserName.c_str();

                            std::wstring proxyUserPassword = loginData[1].toStdWString();
                            const wchar_t* szProxyUserPassword = proxyUserPassword.c_str();

                            // InternetOpen
                            QString proxyString = "http=" + ProxySettings::instance()->proxyHost() + ":" + ProxySettings::instance()->proxyPort();
                            std::wstring proxy = proxyString.toStdWString();
                            const wchar_t* szProxy= proxy.c_str();
                            hIntSession = ::InternetOpen(_T("MyApp"), INTERNET_OPEN_TYPE_PROXY, szProxy, NULL, 0);

                            // InternetConnect
                            hHttpSession =
                              InternetConnect(hIntSession, _T("updatevpn.securepoint.de"), 80, szProxyUserName, szProxyUserPassword, INTERNET_SERVICE_HTTP, 0, NULL);

                            if(https){ // if we had HTTP_STATUS_BAD_REQUEST before
                                // HttpOpenRequest
                                hResourceHandle = HttpOpenRequestW(
                                  hHttpSession,
                                  _T("GET"),
                                  _T("/sslvpn-client/LATEST"),
                                  0, 0, 0, INTERNET_FLAG_SECURE, 0);

                            }
                            else{
                                // HttpOpenRequest
                                hResourceHandle = HttpOpenRequestW(
                                  hHttpSession,
                                  _T("GET"),
                                  _T("/sslvpn-client/LATEST"),
                                  0, 0, 0, INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS, 0);
                            }


                            // HttpSendRequest
                            if( !HttpSendRequest(hResourceHandle, szHeaders, _tcslen(szHeaders), szReq, strlen(szReq))) {
                              dwErr = GetLastError();

                            }

                            // HttpQueryInfo
                            HttpQueryInfo(hResourceHandle, HTTP_QUERY_FLAG_NUMBER |
                                          HTTP_QUERY_STATUS_CODE, &dwStatus, &dwStatusSize, NULL);



                            retry --;

                        }// if end
                        else{
                            break;
                        }

                        }
                        while(retry != 0);




        // read http request
        CHAR szBuffer[1025] = {};
        DWORD dwRead=0;
        Debug::log(QLatin1String("read"));

        while(::InternetReadFile(hResourceHandle, szBuffer, sizeof(szBuffer)-1, &dwRead) && dwRead) {

            Debug::log(QLatin1String("Version: "));
            Debug::log(QLatin1String(szBuffer));


          szBuffer[dwRead] = 0;
          OutputDebugStringA(szBuffer);
          dwRead=0;
        }


         // close all handles
         ::InternetCloseHandle(hResourceHandle);
         ::InternetCloseHandle(hHttpSession);
         ::InternetCloseHandle(hIntSession);

        // finish
        finished(szBuffer);
}
