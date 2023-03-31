#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>

// We need to inherit from QObject beacuse we need the Q_PROPERTY macro
class Settings : public QObject
{
    Q_OBJECT
    // Without define the setting with the Q_Property macro it is not possible to set the value
    Q_PROPERTY (bool useInteract READ useInteract WRITE setUseInteract)
    Q_PROPERTY (bool startOnWindows READ startOnWindows WRITE setStartOnWindows)
    Q_PROPERTY (bool blockShutdown READ blockShutdown WRITE setBlockShutdown)

    Q_PROPERTY (bool showBallon READ showBallon WRITE setShowBallon)
    Q_PROPERTY (bool autoRestart READ autoRestart WRITE setAutoRestart)
    Q_PROPERTY (bool alwaysPopup READ alwaysPopup WRITE setAlwaysPopup)
    Q_PROPERTY (bool showSplash READ showSplash WRITE setShowSplash)

    Q_PROPERTY (bool enableUpdate READ enableUpdate WRITE setEnableUpdate)
    Q_PROPERTY (bool useSecurepoint READ useSecurepoint WRITE setUseSecurepoint)
    Q_PROPERTY (QString updateUrl READ updateUrl WRITE setUpdateUrl)

    Q_PROPERTY (QString maschineConfigDirectory READ maschineConfigDirectory WRITE setMaschineConfigDirectory)

    Q_PROPERTY (bool dhShowSmallKeyInformation READ dhShowSmallKeyInformation WRITE setDhShowSmallKeyInformation)

    Q_PROPERTY (bool sendToTrayAfterConnect READ sendToTrayAfterConnect WRITE setSendToTrayAfterConnect)
public:
    static Settings *instance ();
    virtual ~Settings(){}
    void loadOrRefresh ();

    void setUseInteract (bool flag);
    bool useInteract () const;

    void setStartOnWindows (bool flag) ;
    bool startOnWindows () const;
    void setRegEntryStartOnWindows (bool flag);

    bool blockShutdown() const;
    void setBlockShutdown (bool flag);

    bool showBallon () const;
    void setShowBallon (bool flag);

    void setAutoRestart (bool flag);
    bool autoRestart () const;

    void setAlwaysPopup (bool flag);
    bool alwaysPopup () const;

    void setShowSplash (bool flag);
    bool showSplash () const;

    void setEnableUpdate (bool flag);
    bool enableUpdate () const;

    void setDhShowSmallKeyInformation (bool flag);
    bool dhShowSmallKeyInformation () const;

    void setUseSecurepoint (bool flag);
    bool useSecurepoint () const;

    void setManaged(bool flag);
    bool managed() const;

    void setUpdateUrl (const QString &value);
    QString updateUrl () const;

    void setMaschineConfigDirectory (const QString &value);
    QString maschineConfigDirectory () const;

    QString startConfig() const;
    void setStartConfig(const QString &value);

    QString startUser() const;
    void setStartUser(const QString &value);

    QString startPassword() const;
    void setStartPassword(const QString &value);

    void setUseGerman () {
        this->useGermanValue = true;
    }

    void setNoSave(){
        this->noSave = true;
    }

    void setVpnLog(bool);
    bool VpnLog();

    void setSendToTrayAfterConnect (bool flag);
    bool sendToTrayAfterConnect () const;

private:
    Settings();
    static Settings *mInst;

    volatile bool isOnRefresh;

    bool isUseInteract;
    bool isStartOnWindows;
    bool isBlockShutdown;
    //
    bool isShowBallon;

    bool isAutoRestart;
    bool isAlwaysPopup;

    bool isShowSplash;
    bool isShowSmallDhKeyHint;

    bool isEnableUpdate;
    QString updateUrlValue;
    QString maschineDirectoryValue;
    bool isUseSecurepoint;

    bool isManaged;
    bool isSendToTrayAfterConnect;

    QString _startConfigPath;
    QString _startUser;
    QString _startPassword;

    bool _bShowLog;

    bool useGermanValue;
    bool noSave;

    void save (const QString &name, const QString &value);
};

#endif // SETTINGS_H
