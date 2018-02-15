#ifndef CONNECTIONDATA_H
#define CONNECTIONDATA_H

#include <QObject>
#include <QtCore>

enum ConnectionState : char
{
    Disconnected,
    Disconnecting,
    Connecting,
    Connected,
    Error,
};

class ConnectionData : public QObject
{
    Q_OBJECT

public:
    ConnectionData(QObject *parent = 0);
    ~ConnectionData();

    void SetName(const QString &name);
    const QString GetName();

    const QString GetIP();
    void SetIP(const QString &ip);

    QString getScript(const QString &type);
    void runScript(const QString &type);

    quint32 GetLastUsed();
    quint32 GetLastConnected();

	/* On Configs::refreshConfigs bDb is set to false to speed it up */
    void SetLastUsed(quint32 lastUsed, bool bDb = true);
    void SetLastConnected(quint32 lastConnected, bool bDb = true);

    ConnectionState GetState();
    void SetState(ConnectionState state, bool bDb = true);

    bool IsAutoStart();
    void SetAutoStart(bool autostart);

    bool IsUserConfig();
    void SetUserConfig(bool user);

    const QString& GetConfigPath();
    void SetConfigPath(const QString &path);

    const QString GetDir();

    void SetId(quint32 id);
    quint32 GetId();

    void SetError(QString errorMsg);
    QString GetError();

    bool Connect();
    bool Disconnect();

    void Delete();

    bool HasCrediantials(int type);

    QString getSavedUserData(int type);

private:
    quint64 lastConnected;
    quint64 lastUsed;
    QString name;
    QString ip;
    QString configPath;
    QString errorMsg;
    quint32 id;
    bool autostart;
    bool user;
    ConnectionState state;

public slots:
        void writeUserData (QString data);
        void saveUserData (int id, int type, QString value, bool save);

private slots:
    void startAfterConnectDelayed ();
    void scriptFinished(int exitCode, QProcess::ExitStatus status);
    void scriptErrorOccurred(QProcess::ProcessError error);
    void scriptStartet();

};

#endif // CONNECTIONDATA_H
