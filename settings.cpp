#include "settings.h"
#include "appfunc.h"

#include "crypt.h"

Settings *Settings::mInst = NULL;

Settings *Settings::getInstance() {
    if (!mInst)
        mInst = new Settings ();
    return mInst;
}

Settings::Settings()
    : autoReconnect (false),
      noBallonMessage (false),
      runAsService (true),
      isLangGerman (true),
      useNoInteract (true),
      isPortableClient (false),
      manageClient (false),
      popUpDialogValue (true),
      showSplashScreenValue (true),
      checkWindowsShutdownValue (true),
      autoUpdateValue(true),
      updateSourceValue ("http://updatevpnc.securepoint.de/vpn-changelog.xml"),
      updateUseProxyValue(false),
      updateProxyIpValue(""),
      updateProxyPortValue("8080"),
      updateProxyUserValue(""),
      updateProxyPasswordValue(""),
      listenPort (3655),
      servicePort (3656),
      cryptKey (""),
      disableSaveDataValue(false),
      useSourceforgeValue(false)
{
}

QString Settings::getProxyIniPath()
{
    if (this->isPortableClient) {
        return QCoreApplication::applicationDirPath() + QLatin1String("/proxy.ini");
    }

    return AppFunc::getAppSavePath() + QLatin1String("/proxy.ini");
}

void Settings::refresh()
{
    // Read ini file
    this->iniPath = AppFunc::getAppSavePath() + QLatin1String("/vpn.ini");

    QSettings sett (this->iniPath, QSettings::IniFormat);

    // Die Settings einlesen
    this->noBallonMessage = (sett.value(QLatin1String("connect/noballon"), QLatin1String("0")).toString() == QLatin1String("1") ? true : false);
    this->popUpDialogValue = (sett.value(QLatin1String("connect/popup"), QLatin1String("1")).toString() == QLatin1String("1") ? true : false);
    this->showSplashScreenValue = (sett.value(QLatin1String("connect/splash"), QLatin1String("1")).toString() == QLatin1String("1") ? true : false);
    this->autoReconnect = (sett.value(QLatin1String("connect/autoreconnect"), QLatin1String("0")).toString() == QLatin1String("1") ? true : false);
    this->useNoInteract = (sett.value(QLatin1String("connect/nointeract"), QLatin1String("0")).toString() == QLatin1String("1") ? true : false);
    this->autoUpdateValue = (sett.value(QLatin1String("update/auto"), QLatin1String("1")).toString() == QLatin1String("1") ? true : false);

    this->updateSourceValue = sett.value(QLatin1String("update/source"), QLatin1String("http://updatevpnc.securepoint.de/vpn-changelog.xml")).toString();

    this->updateUseProxyValue = (sett.value(QLatin1String("update/useproxy"), QLatin1String("0")).toString() == QLatin1String("1") ? true : false);
    this->updateProxyIpValue = sett.value(QLatin1String("update/proxyip"), QLatin1String("")).toString();
    this->updateProxyPortValue = sett.value(QLatin1String("update/proxyport"), QLatin1String("8080")).toString();


    this->useSourceforgeValue = (sett.value(QLatin1String("update/useSF"), QLatin1String("0")).toString() == QLatin1String("1") ? true : false);

    this->checkWindowsShutdownValue = (sett.value(QLatin1String("exit/checkWinEvent"), QLatin1String("1")).toString() == QLatin1String("1") ? true : false);
    this->cryptKey = sett.value(QLatin1String("self/key"), QLatin1String("")).toString();
    if (!this->cryptKey.isEmpty()) {
        Crypt crypt;
        this->cryptKey = QString(crypt.cryptToPlainTextExt(this->cryptKey.toAscii()));
    } else {
        // Neuen Key erzeugen
        qsrand(QDateTime::currentDateTime().toTime_t());
        QString _t1 (QString::number((qrand() % 1500) + 1));
        QString _t2 (QString::number((qrand() % 2500) + 1));
        QString key (QLatin1String("S3m!BHF") + _t1 + QLatin1String("83$%§kd") + _t2 + QString::number(QDateTime::currentDateTime().toTime_t()) + _t1);

        Crypt crypt;
        key = QString(crypt.cryptPlainTextExt(key.toAscii()));

        sett.setValue(QLatin1String("self/key"), key);
        this->cryptKey = key;
    }

    //
    this->updateProxyUserValue = sett.value(QLatin1String("update/proxyuser"), QLatin1String("")).toString();
    this->updateProxyPasswordValue = sett.value(QLatin1String("update/proxypassword"), QLatin1String("")).toString();
    if (!this->updateProxyPasswordValue.isEmpty()) {
        Crypt crypt;
        this->updateProxyPasswordValue = QString(crypt.cryptToPlainTextExt(this->updateProxyPasswordValue.toAscii()));
    }

    this->windowTopValue = sett.value(QLatin1String("window/top")).toPoint();
}

// GETTER

bool Settings::getIsAutoReconnect() const
{
    return this->autoReconnect;
}

bool Settings::getIsShowNoBallonMessage() const
{
    return this->noBallonMessage;
}

bool Settings::getIsRunAsSevice() const
{
    return this->runAsService;
}

bool Settings::getIsLanguageGerman() const
{
    return this->isLangGerman;
}

bool Settings::getUseNoInteract() const
{
    return this->useNoInteract;
}

bool Settings::getIsPortableClient() const
{
    return this->isPortableClient;
}

bool Settings::getIsManageClient() const
{
    return this->manageClient;
}

quint16 Settings::getListenPort() const
{
    return this->listenPort;
}

quint16 Settings::getServerPort() const
{
    return this->servicePort;
}

QString Settings::getCryptKey() const
{
    return this->cryptKey;
}

bool Settings::popUpDialog() const
{
    return this->popUpDialogValue;
}

bool Settings::showSplashScreen() const
{
    return this->showSplashScreenValue;
}

bool Settings::autoStartOnWindowsStartup()
{
    QSettings regRun (QLatin1String("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
    QString regVal = regRun.value(QLatin1String("SpSSLVPN"), QLatin1String("0")).toString();
    // Ist der Eintrag da und gefüllt
    if (regVal != QLatin1String("0")) {
        return true;
    }

    return false;
}

// SETTER
void Settings::setAutoStartOnWindowsStartup(const bool &flag)
{
    QSettings regRun (QLatin1String("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), QSettings::NativeFormat);
    if (flag) {
        QString path;
        if (this->isPortableClient) {
            path = QCoreApplication::applicationDirPath() + QLatin1String("/SecurepointVPNClient.exe");
        } else {
            path = QCoreApplication::applicationDirPath() + QLatin1String("/Spvpncl.exe");
        }

        if (!this->isLangGerman) {

            path += QLatin1String(" -useEnglish");
        }

        if (this->manageClient) {
            // Add manage param
            path += QLatin1String(" -manage");
        }

        if (this->disableSaveDataValue) {
            path += QLatin1String(" -disableSaveData");
        }

        path = path.replace("/", "\\");
        regRun.setValue(QLatin1String("SpSSLVPN"), path);
    } else {
        if (this->autoStartOnWindowsStartup()) {
            regRun.remove(QLatin1String("SpSSLVPN"));
        }
    }
}

void Settings::setIsAutoReconnect(const bool &flag)
{
    this->autoReconnect = flag;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("connect/autoreconnect"), flag ? QLatin1String("1") : QLatin1String("0"));
}

void Settings::setIsShowNoBallonMessage(const bool &flag)
{
    this->noBallonMessage = flag;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("connect/noballon"), flag ? QLatin1String("1") : QLatin1String("0"));
}

void Settings::setIsRunAsService(const bool &flag)
{
    this->runAsService = flag;
}

void Settings::setIsLanguageGerman(const bool &flag)
{
    this->isLangGerman = flag;
}

void Settings::setUseNoInteract(const bool &flag)
{
    this->useNoInteract = flag;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("connect/nointeract"),flag ? QLatin1String("1") : QLatin1String("0"));
}

void Settings::setPopUpDialog(const bool &flag)
{
    this->popUpDialogValue = flag;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("connect/popup"), flag ? QLatin1String("1") : QLatin1String("0"));
}

void Settings::setShowSplashScreen(const bool &flag)
{
    this->showSplashScreenValue = flag;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("connect/splash"), flag ? QLatin1String("1") : QLatin1String("0"));
}

void Settings::setIsPortableClient(const bool &flag)
{
    this->isPortableClient = flag;
}

void Settings::setIsManageClient(const bool &flag)
{
    this->manageClient = flag;
}

void Settings::setServerPort(quint16 port)
{
    this->servicePort = port;
}

void Settings::setListenPort(quint16 port)
{
    this->listenPort = port;
}

void Settings::setCryptKey(const QString &key)
{
    this->cryptKey = key;
}

bool Settings::checkWindowsShutdown() const
{
    return this->checkWindowsShutdownValue;
}

void Settings::setCheckWindowsShutdown(bool flag)
{
    this->checkWindowsShutdownValue = flag;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("exit/checkWinEvent"), (this->checkWindowsShutdownValue ? QLatin1String("1") : QLatin1String("0")));
}

bool Settings::isAutoUpdate() const
{
    return this->autoUpdateValue;
}

void Settings::setAutoUpdate(bool flag)
{
    this->autoUpdateValue = flag;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("update/auto"), (this->autoUpdateValue ? QLatin1String("1") : QLatin1String("0")));
}

QString Settings::updateSource() const
{
    return this->updateSourceValue;
}

void Settings::setUpdateSource(const QString source)
{
    this->updateSourceValue = source;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("update/source"), this->updateSourceValue);
}

bool Settings::updateUseProxy() const
{
    return this->updateUseProxyValue;
}

QString Settings::updateProxyIp() const
{
    return this->updateProxyIpValue;
}

QString Settings::updateProxyPort() const
{
    return this->updateProxyPortValue;
}

void Settings::setUpdateUseProxy(bool flag)
{
    this->updateUseProxyValue = flag;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("update/useproxy"), (this->updateUseProxyValue ? QLatin1String("1") : QLatin1String("0")));
}

void Settings::setUpdateProxyPort(const QString &port)
{
    this->updateProxyPortValue = port;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("update/proxyport"), this->updateProxyPortValue);
}

void Settings::setUpdateProxyIp(const QString &ip)
{
    this->updateProxyIpValue = ip;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("update/proxyip"), this->updateProxyIpValue);
}

QString Settings::updateProxyUser() const
{
    return this->updateProxyUserValue;
}

QString Settings::updateProxyPassword() const
{
    return this->updateProxyPasswordValue;
}

bool Settings::disableSaveData() const
{
    return this->disableSaveDataValue;
}

bool Settings::useSourceForge() const
{
    return this->useSourceforgeValue;
}

QPoint Settings::windowTop() const
{
    return this->windowTopValue;
}

void Settings::setUpdateProxyUser(const QString &user)
{
    this->updateProxyUserValue = user;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("update/proxyuser"), this->updateProxyUserValue);
}

void Settings::setUpdateProxyPassword(const QString &password)
{
    this->updateProxyPasswordValue = password;
    QString data (password);
    if (!password.isEmpty()) {
        Crypt crypt;
        data = QString(crypt.cryptPlainTextExt(data.toAscii()));
    }
    //
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("update/proxypassword"), data);
}

void Settings::setDisableSaveData(bool flag)
{
    this->disableSaveDataValue = flag;
}

void Settings::setUseSourceForge(bool flag)
{
    this->useSourceforgeValue = flag;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("update/useSF"), (this->useSourceforgeValue ? QLatin1String("1") : QLatin1String("0")));
}

void Settings::setWindowTop(const QPoint &top)
{
    this->windowTopValue = top;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("window/top"), this->windowTopValue);
}
