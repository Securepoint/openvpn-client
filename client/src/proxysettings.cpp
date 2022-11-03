#include "proxysettings.h"
#include <utils.h>
#include <crypt.h>

ProxySettings *ProxySettings::thisInstance = 0;

ProxySettings *ProxySettings::instance() {
    if (!ProxySettings::thisInstance) {
        ProxySettings::thisInstance = new ProxySettings;
    }

    return ProxySettings::thisInstance;
}

QString ProxySettings::openVpnProxyString()
{
    // We got manual or system proxy
    QStringList proxyArguments;

    if (this->proxyTypeInUseValue == QLatin1String("IE")) {
        // Read the system settings
        QSettings registrySystemSettings (QLatin1String("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), QSettings::NativeFormat);
        QString proxyServerValue = registrySystemSettings.value(QLatin1String("ProxyServer"), QLatin1String("")).toString();
        // Value is host:port
        QStringList proxyServerSettings = proxyServerValue.split(":");
        //
        if (proxyServerSettings.size() == 2) {
            proxyArguments << QLatin1String ("--http-proxy")
                           << proxyServerSettings[0]
                           << proxyServerSettings[1];
        }
    } else if (this->proxyTypeInUseValue == QLatin1String("MANUAL")) {
        // Read the client settings
        //
        if (!this->proxyHostValue.isEmpty() && !this->proxyPortValue.isEmpty()) {
            if (this->isHttpProxy()) {
                // HTTP PROXY
                proxyArguments << QLatin1String ("--http-proxy");
            } else {
                // SOCKS PROXY
                proxyArguments << QLatin1String ("--socks-proxy");
            }

            proxyArguments << this->proxyHostValue;
            proxyArguments << this->proxyPortValue;
        }


    } else if (this->proxyTypeInUseValue == QLatin1String("CONFIG")) {
        // Nothing to do, all data of this connections is defined in config file
    }

    return proxyArguments.join(" ");
}

void ProxySettings::checkAndMigrateOldProxySettings()
{


    // Check if already a proxy.ini is in appdata
    // If yes, we migrating the data already
    if (this->settingsAvailable()) {
        //
        return;
    }

    // No proxy.ini available, read the old one
    // write the new one and then remove the old one
    {
        QSettings oldProxyIni (this->oldProxyIniPath, QSettings::IniFormat);
        //
        const QString useType = oldProxyIni.value("proxy-use", QLatin1String("")).toString();
        const QString http = oldProxyIni.value("proxy-type", QLatin1String("")).toString();
        const QString host = oldProxyIni.value("proxy-ip", QLatin1String("")).toString();
        const QString port = oldProxyIni.value("proxy-port", QLatin1String("")).toString();

        // Write the new ini
        this->setProxyTypeInUse(useType);
        //
        if (!http.isEmpty()) {
            this->setIsHttpProxy(http == "HTTP");
        }
        //
        if (!host.isEmpty()) {
            this->setProxyHost(host);
        }
        //
        if (!port.isEmpty()) {
            this->setProxyPort(port);
        }
    }

    // Unlink the file
    QFile oldProxySettingsFile (this->oldProxyIniPath);
    oldProxySettingsFile.remove();
}

bool ProxySettings::settingsAvailable()
{
    QFile proxyIniFile (this->proxyIniPath);
    //
    return proxyIniFile.exists(this->proxyIniPath);
}

bool ProxySettings::oldSettingsAvailable()
{
    QFile proxyIniFile (this->oldProxyIniPath);
    //
    return proxyIniFile.exists(this->oldProxyIniPath);
}

QString ProxySettings::proxyTypeInUse()
{
    return this->proxyTypeInUseValue;
}

void ProxySettings::setProxyTypeInUse(const QString &type)
{
    if (this->proxyTypeInUseValue != type) {
        //
        this->proxyTypeInUseValue = type;
        //
        QSettings proxyIni (this->proxyIniPath, QSettings::IniFormat);
        proxyIni.setValue("proxy-use", Crypt::encodePlaintext(type));
    }
}

bool ProxySettings::isHttpProxy()
{
    return this->useHttpProxyValue;
}

void ProxySettings::setIsHttpProxy(bool flag)
{
    if (this->useHttpProxyValue != flag) {
        //
        this->useHttpProxyValue = flag;
        //
        QSettings proxyIni (this->proxyIniPath, QSettings::IniFormat);
        proxyIni.setValue("proxy-type", Crypt::encodePlaintext((flag ? "HTTP" : "SOCKS")));
    }

}

QString ProxySettings::proxyPort()
{
    return this->proxyPortValue;
}

QString ProxySettings::systemProxyPort()
{
    QSettings registrySystemSettings (QLatin1String("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), QSettings::NativeFormat);
    QString proxyServerValue = registrySystemSettings.value(QLatin1String("ProxyServer"), QLatin1String("")).toString();
    // Value is host:port
    QStringList proxyServerSettings = proxyServerValue.split(":");
    //
    if (proxyServerSettings.size() == 2) {
        return proxyServerSettings[1];
    }

    return QString("");
}

QString ProxySettings::systemProxyHost()
{
    QSettings registrySystemSettings (QLatin1String("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"), QSettings::NativeFormat);
    QString proxyServerValue = registrySystemSettings.value(QLatin1String("ProxyServer"), QLatin1String("")).toString();
    // Value is host:port
    QStringList proxyServerSettings = proxyServerValue.split(":");
    //
    if (proxyServerSettings.size() == 2) {
        return proxyServerSettings[0];
    }

    return QString("");
}

void ProxySettings::setProxyPort(const QString &port)
{
    if (this->proxyPortValue != port) {
        //
        this->proxyPortValue = port;
        //
        QSettings proxyIni (this->proxyIniPath, QSettings::IniFormat);
        proxyIni.setValue("proxy-port", Crypt::encodePlaintext(port));
    }
}

QString ProxySettings::proxyHost()
{
    return this->proxyHostValue;
}

void ProxySettings::setProxyHost(const QString &host)
{
    if (this->proxyHostValue != host) {
        //
        this->proxyHostValue = host;
        //
        QSettings proxyIni (this->proxyIniPath, QSettings::IniFormat);
        proxyIni.setValue("proxy-ip", Crypt::encodePlaintext(host));
    }
}

ProxySettings::ProxySettings()
 : oldProxyIniPath(Utils::dataDirectory() + QLatin1String("/proxy.ini")),
   proxyIniPath (Utils::userApplicationDataDirectory() + QLatin1String("/proxy.ini")),
   useHttpProxyValue(false)
{
    if (this->oldSettingsAvailable()) {
        //
        this->checkAndMigrateOldProxySettings();
    }

    // Now check for the current handling
    if (this->settingsAvailable()) {
        QSettings proxyIni (this->proxyIniPath, QSettings::IniFormat);
        this->proxyTypeInUseValue = QString(Crypt::decodeToPlaintext(proxyIni.value("proxy-use", QLatin1String("")).toString()));
        //
        this->useHttpProxyValue = QString(Crypt::decodeToPlaintext(proxyIni.value("proxy-type", QLatin1String("")).toString())) == QLatin1String("SOCKS") ? false : true;
        //
        this->proxyHostValue = QString(Crypt::decodeToPlaintext(proxyIni.value("proxy-ip", QLatin1String("")).toString()));
        this->proxyPortValue = QString(Crypt::decodeToPlaintext(proxyIni.value("proxy-port", QLatin1String("")).toString()));
    }
}

