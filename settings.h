#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>

class Settings
{
public:
    static Settings *getInstance ();

    // Getter
    bool getIsAutoReconnect ();
    bool getIsShowNoBallonMessage ();
    bool getIsShowNoUserAuth ();
    bool getIsRunAsSevice ();
    bool getIsStartCommandConfig ();
    bool getIsStartConfigDir ();
    bool getIsStartWithPwd ();
    bool getIsStartWithUser ();
    bool getIsNoPopUp ();
    bool getUseCryptedData ();
    bool getIsForcePrivateKey ();
    bool getIsLanguageGerman ();
    bool getUseNoInteract ();
    bool getIsPortableClient ();
    bool getIsManageClient ();

    QString getCommandConfigPath ();
    QString getStartConfigDirPath ();
    QString getStartWithPwdPwd ();
    QString getStartWithUser ();

    quint16 getListenPort ();
    quint16 getServerPort ();

    // Setter
    void setIsAutoReconnect (bool flag);
    void setIsShowNoBallonMessage (bool flag);
    void setIsShowNoUserAuth (bool flag);
    void setIsRunAsService (bool flag);
    void setIsStartCommandConfig (bool flag);
    void setIsStartConfigDir (bool flag);
    void setIsStartWithPwd (bool flag);
    void setIsStartWithUser (bool flag);
    void setIsNoPopUp (bool flag);
    void setUseCryptedData (bool flag);
    void setForcePrivateKey (bool flag);
    void setIsLanguageGerman (bool flag);
    void setUseNoInteract (bool flag);
    void setIsPortableClient (bool flag);
    void setIsManageClient (bool flag);

    void setCommandConfigPath (QString path);
    void setStartConfigDirPath (QString path);
    void setStartWithPwdPwd (QString pwd);
    void setStartWithUserName (QString user);

    void setListenPort (quint16 port);
    void setServerPort (quint16 port);

    // Normale Methoden

private:
    static Settings *mInst;
    Settings();
    // Member

    bool autoReconnect;
    bool noBallonMessage;
    bool noUserAuth;
    bool runAsService;
    bool startCommandConfig;
    bool startConfigDir;
    bool startWithPwd;
    bool startWithUser;
    bool startNoPopUp;
    bool useCryptedData;
    bool forcePrivateKey;
    bool isLangGerman;
    bool useNoInteract;
    bool isPortableClient;
    bool manageClient;

    QString commandConfigPath;
    QString startConfigDirPath;
    QString startWithPwdPwd;
    QString startWithUserName;

    // Client und Serverport
    quint16 listenPort;
    quint16 servicePort;
};

#endif // SETTINGS_H
