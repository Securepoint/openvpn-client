#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>

class Settings
{
public:
    static Settings *getInstance ();


    void refresh ();
    // Getter
    bool getIsAutoReconnect () const;
    bool getIsShowNoBallonMessage () const;
    bool getIsRunAsSevice () const;
    bool getIsLanguageGerman () const;
    bool getUseNoInteract () const;
    bool getIsPortableClient () const;
    bool getIsManageClient () const;

    QString getStartWithPwdPwd () const;
    QString getStartWithUser () const;

    quint16 getListenPort () const;
    quint16 getServerPort () const;

    QString getCryptKey () const;
    QString getProxyIniPath ();

    bool popUpDialog () const;
    bool showSplashScreen () const;

    bool autoStartOnWindowsStartup ();
    bool checkWindowsShutdown () const;

    bool isAutoUpdate () const;
    QString updateSource () const;
    bool updateUseProxy () const;
    QString updateProxyIp () const;
    QString updateProxyPort () const;
    QString updateProxyUser () const;
    QString updateProxyPassword () const;
    bool disableSaveData () const;
    bool useSourceForge () const;

    QPoint windowTop () const;

    // Setter
    void setIsAutoReconnect (const bool &flag);
    void setIsShowNoBallonMessage (const bool &flag);

    void setIsRunAsService (const bool &flag);

    void setIsNoPopUp (const bool &flag);
    void setIsLanguageGerman (const bool &flag);
    void setUseNoInteract (const bool &flag);
    void setIsPortableClient (const bool &flag);
    void setIsManageClient (const bool &flag);

    void setStartWithPwdPwd (const QString &pwd);
    void setStartWithUserName (const QString &user);

    void setListenPort (quint16 port);
    void setServerPort (quint16 port);

    void setCryptKey (const QString &key);

    void setPopUpDialog (const bool &flag);
    void setShowSplashScreen (const bool &flag);

    void setAutoStartOnWindowsStartup (const bool &flag);
    void setCheckWindowsShutdown (bool flag);
    void setAutoUpdate (bool flag);
    void setUpdateSource (const QString source);

    void setUpdateUseProxy (bool flag);
    void setUpdateProxyPort (const QString &port);
    void setUpdateProxyIp (const QString &ip);
    void setUpdateProxyUser (const QString &user);
    void setUpdateProxyPassword (const QString &password);
    void setDisableSaveData (bool flag);
    void setUseSourceForge (bool flag);
    void setWindowTop (const QPoint &top);

private:
    static Settings *mInst;
    Settings();
    // Member

    bool autoReconnect;
    bool noBallonMessage;

    bool runAsService;

    bool isLangGerman;
    bool useNoInteract;
    bool isPortableClient;
    bool manageClient;

    bool popUpDialogValue;
    bool showSplashScreenValue;

    bool checkWindowsShutdownValue;
    bool autoUpdateValue;
    QString updateSourceValue;
    bool updateUseProxyValue;
    QString updateProxyIpValue;
    QString updateProxyPortValue;
    QString updateProxyUserValue;
    QString updateProxyPasswordValue;

    // Client und Serverport
    quint16 listenPort;
    quint16 servicePort;
    QString iniPath;

    QString cryptKey;
    bool disableSaveDataValue;

    bool useSourceforgeValue;

    QPoint windowTopValue;
};

#endif // SETTINGS_H
