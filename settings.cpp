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
      startCommandConfig (false),
      startConfigDir (false),                
      isLangGerman (true),
      useNoInteract (true),
      isPortableClient (false),
      manageClient (false),
      popUpDialogValue (true),
      showSplashScreenValue (true),
      commandConfigPath (""),
      startConfigDirPath (""),
      listenPort (3655),
      servicePort (3656),
      cryptKey (""),
      delayConfigVal ("0")
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

    // Start config
    this->startCommandConfig = (sett.value(QLatin1String("start/auto"), QLatin1String("0")).toString() == QLatin1String("1") ? true : false);
    this->commandConfigPath = sett.value(QLatin1String("start/path"), QLatin1String("")).toString();

    this->delayConfigVal = sett.value(QLatin1String("start/delay"), QLatin1String("0")).toString();

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

bool Settings::getIsStartCommandConfig() const
{
    return this->startCommandConfig;
}

bool Settings::getIsStartConfigDir() const
{
    return this->startConfigDir;
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

QString Settings::getCommandConfigPath() const
{
    return this->commandConfigPath;
}

QString Settings::getStartConfigDirPath() const
{
    return this->startConfigDirPath;
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

QString Settings::delayConfig() const
{
    return this->delayConfigVal;
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

void Settings::setIsStartCommandConfig(const bool &flag, bool save)
{
    this->startCommandConfig = flag;

    if (save) {
        QSettings sett (this->iniPath, QSettings::IniFormat);
        sett.setValue(QLatin1String("start/auto"), flag ? QLatin1String("1") : QLatin1String("0"));
    }
}

void Settings::setIsStartConfigDir(const bool &flag)
{
    this->startConfigDir = flag;
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

void Settings::setCommandConfigPath(const QString &path, bool save)
{
    this->commandConfigPath = path;
    this->commandConfigPath = this->commandConfigPath.replace("\\", "/").trimmed();

    if (save) {
        QSettings sett (this->iniPath, QSettings::IniFormat);
        sett.setValue(QLatin1String("start/path"), this->commandConfigPath);
    }
}

void Settings::setStartConfigDirPath(const QString &path)
{
    this->startConfigDirPath = path;
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

void Settings::setDelayStartConfig(const QString &value)
{
    this->delayConfigVal = value;
    QSettings sett (this->iniPath, QSettings::IniFormat);
    sett.setValue(QLatin1String("start/delay"), this->delayConfigVal);
}
