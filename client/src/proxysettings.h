#ifndef PROXYSETTINGS_H
#define PROXYSETTINGS_H

#include <QtCore>

class ProxySettings
{
public:
    static ProxySettings *instance();
    QString openVpnProxyString ();
    void checkAndMigrateOldProxySettings();

    bool settingsAvailable();
    bool oldSettingsAvailable();

    QString proxyTypeInUse ();
    void setProxyTypeInUse(const QString &type);

    bool isHttpProxy();
    void setIsHttpProxy(bool flag);

    QString proxyPort ();
    QString systemProxyPort ();
    void setProxyPort(const QString &port);

    QString proxyHost ();
    QString systemProxyHost ();
    void setProxyHost(const QString &host);

private:
    ProxySettings();
    static ProxySettings *thisInstance;

    QString oldProxyIniPath;
    QString proxyIniPath;
    //
    QString proxyTypeInUseValue;
    bool useHttpProxyValue;

    QString proxyHostValue;
    QString proxyPortValue;
};

#endif // PROXYSETTINGS_H
