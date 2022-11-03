#ifndef Configs_h
#define Configs_h

#include <QtCore>
#include "../widgets/mainview/connectiondata.h"

struct ConfigData
{
    QString configFile;
    QString configName;
    bool isAutoStart;
    QString vpnUser;
    QString vpnPassword;
    QString vpnPkcs12;
    QString httpUser;
    QString httpPassword;
    bool removeOnStart;
};

class Configs
{
private:
    static Configs * m_Inst;

    QList<QPair<int, ConnectionData*> > myList;

public:
    static Configs *instance ()
    {
        if(!m_Inst)
            m_Inst = new Configs();

        return m_Inst;
    }
    int configCount () const;

    void refreshConfigs();
    void findConfigsInDir(const QString &sDir);
    bool isConfigInList(int id);
    ConnectionData* configWithId(int id);
    bool isConnectionAvailable();

    // This will remove all configs, marked as remove on start from the database
    void cleanupConfigs();


    void addConfigToDatabase(const ConfigData &data);

    void removeFromDatabase(int id);

    decltype(myList) & getList()
    {
        return myList;
    }

    bool ConfigExists(const QString &name);
};


#endif // Configs_h
