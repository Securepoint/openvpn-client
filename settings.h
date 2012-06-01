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
    bool getIsStartCommandConfig () const;
    bool getIsStartConfigDir () const;        
    bool getIsLanguageGerman () const;
    bool getUseNoInteract () const;
    bool getIsPortableClient () const;
    bool getIsManageClient () const;

    QString getCommandConfigPath () const;
    QString getStartConfigDirPath () const;
    QString getStartWithPwdPwd () const;
    QString getStartWithUser () const;

    quint16 getListenPort () const;
    quint16 getServerPort () const;

    QString getCryptKey () const;
    QString getProxyIniPath ();

    bool popUpDialog () const;
    bool showSplashScreen () const;

    bool autoStartOnWindowsStartup ();

    QString delayConfig () const;

    // Setter
    void setIsAutoReconnect (const bool &flag);
    void setIsShowNoBallonMessage (const bool &flag);

    void setIsRunAsService (const bool &flag);
    void setIsStartCommandConfig (const bool &flag, bool save = false);
    void setIsStartConfigDir (const bool &flag);

    void setIsNoPopUp (const bool &flag);    
    void setIsLanguageGerman (const bool &flag);
    void setUseNoInteract (const bool &flag);
    void setIsPortableClient (const bool &flag);
    void setIsManageClient (const bool &flag);

    void setCommandConfigPath (const QString &path, bool save = false);
    void setStartConfigDirPath (const QString &path);
    void setStartWithPwdPwd (const QString &pwd);
    void setStartWithUserName (const QString &user);

    void setListenPort (quint16 port);
    void setServerPort (quint16 port);

    void setCryptKey (const QString &key);

    void setPopUpDialog (const bool &flag);
    void setShowSplashScreen (const bool &flag);

    void setAutoStartOnWindowsStartup (const bool &flag);
    void setDelayStartConfig (const QString &value);

private:
    static Settings *mInst;
    Settings();
    // Member

    bool autoReconnect;
    bool noBallonMessage;

    bool runAsService;
    bool startCommandConfig;
    bool startConfigDir;

    bool isLangGerman;
    bool useNoInteract;
    bool isPortableClient;
    bool manageClient;

    bool popUpDialogValue;
    bool showSplashScreenValue;

    QString commandConfigPath;
    QString startConfigDirPath;    

    // Client und Serverport
    quint16 listenPort;
    quint16 servicePort;
    QString iniPath;

    QString cryptKey;
    QString delayConfigVal;
};

#endif // SETTINGS_H
