#include "configs.h"

#include <database.h>
#include <QtCore>
#include <FrmMain.h>
#include <WidgetFactory.h>
#include <crypt.h>
#include <SrvCLI.h>
#include <checksum.h>

extern bool g_bPortable;

Configs * Configs::m_Inst = nullptr;

// On windows this is faster than QFile::exists
inline bool FileExists (const char* name)
{
    if(INVALID_FILE_ATTRIBUTES == GetFileAttributesA(name) && GetLastError() == ERROR_FILE_NOT_FOUND) {
        //File not found
        return false;
    }
    return true;
}

void Configs::refreshConfigs()
{
    QLatin1String configSql("SELECT \"vpn-id\", \"vpn-name\", \"vpn-config\", \"vpn-autostart\", \"vpn-state\", \"vpn-last-used\", \"vpn-last-connected\", \"vpn-last-error\", coalesce(\"vpn-user\",\"vpn-password\", 0) AS CREDENTIAL_STATE FROM vpn;");
    QScopedPointer<QSqlQuery> configQuery (Database::instance()->openQuery(configSql));
        //
    QString vpnName = "";
    QString vpnConfig = "";
    std::vector<int> vpnIds;

    while (configQuery->next()) {
        // OpenVpn Objekt bauen und an die Liste anfügen
        int vpnId (configQuery->value(0).toInt());

        auto n = (Crypt::decodeToPlaintext(configQuery->value(1).toString()));
        vpnName = QString::fromUtf8(n.data(), n.size());

        n = (Crypt::decodeToPlaintext(configQuery->value(2).toString()));
        vpnConfig = QString::fromUtf8(n.data(), n.size());

        bool vpnAutoStart ((configQuery->value(3).toString() == QLatin1String("1") ? true : false));
        //
        if (g_bPortable) {
            // We are running in portable mode, so we need to check if the dir is a relative path or an absolute!
            if (!vpnConfig.contains(":")) {
                // Relative path, build the absolute
                QDir currentApplicationDirectory (qApp->applicationDirPath());
                vpnConfig = currentApplicationDirectory.absoluteFilePath(vpnConfig);
            }
        }

        if(!QFile::exists(vpnConfig)){
            printf("Remove2 Config %s\n", vpnConfig.toLatin1().data());
            removeFromDatabase(vpnId);
            continue;
        }

        vpnIds.push_back(vpnId);
        if (this->isConfigInList(vpnId)) {
            continue;
        }

        ConnectionState state = (ConnectionState)configQuery->value(4).toInt();

        // Hack to get correct state in status, this will only show the corrected states when a window is created
        // Not a good solution, but it works and its easy
        if(FrmMain::instanceCheck()) {
            if(state == ConnectionState::Connected || state == ConnectionState::Connecting || state == ConnectionState::Disconnecting) {
                state = ConnectionState::Disconnected;
            }
        }
        ConnectionData* obj = new ConnectionData();


        obj->SetName(vpnName);
        obj->SetId(vpnId);
        obj->SetAutoStart(vpnAutoStart);

        // dont write the status to the database, because this state could be a corrected state
        obj->SetState(state, false /* dont write to database */);


        obj->SetConfigPath(vpnConfig);
        obj->SetLastUsed(configQuery->value(5).toUInt(), false /* dont write to database */);
        obj->SetLastConnected(configQuery->value(6).toUInt(), false /* dont write to database */);
        obj->SetError(configQuery->value(7).toString());

        // Handle saved credential state, coalesce must be not equal to 0
        const QString credentialsAreSet = configQuery->value(8).toString();
        // Now check the value
        obj->SetUserConfig(credentialsAreSet == "0" ? false : true);

        // Append config to list
        this->myList.append(qMakePair(vpnId, obj));
    }

    std::list<int> removedIds;
    for(auto item : this->myList) {
        removedIds.push_back(item.first);
    }

    for(auto vpnId : vpnIds) {
        removedIds.remove(vpnId);
    }


    if(removedIds.size() > 0) {
        QMutableListIterator<QPair<int, ConnectionData*> > i(this->myList);

        while (i.hasNext()) {
            QPair <int, ConnectionData*> entry = i.next();
            ConnectionData *obj = entry.second;

            if(removedIds.front() == entry.first) {
                // Objekt löschen
                delete obj;
                obj = 0;
                // Nun aus der Liste entfernen
                i.remove();
                removedIds.pop_front();
                if(removedIds.size() == 0)
                    break;
            }
        }
    }

    // sort the list by name
    std::sort(this->myList.begin(), this->myList.end(), [](QPair<int, ConnectionData*> a, QPair<int, ConnectionData*> b) -> bool {return a.second->GetName().toLower() < b.second->GetName().toLower();});
}

void Configs::removeFromDatabase(int id)
{
    QString sql (QString("DELETE FROM vpn WHERE \"vpn-id\" = %1")
        .arg(id));
    Database::instance()->execute(sql);
}

void Configs::addConfigToDatabase(const ConfigData &data)
{
    if(ConfigExists(data.configName))
        return;

     QString sql (QString("INSERT INTO vpn  (\"vpn-name\", \"vpn-config\", \"vpn-autostart\", \"vpn-user\", \"vpn-password\", \"vpn-pkcs12\", \"vpn-http-user\", \"vpn-http-password\", \"vpn-remove-on-start\") VALUES ('%1', '%2', '%3', '%4', '%5', '%6', '%7', '%8', '%9');")
         .arg(Crypt::encodePlaintext(data.configName))
         .arg(Crypt::encodePlaintext(data.configFile))
         .arg(data.isAutoStart)
         .arg(data.vpnUser.isEmpty() ? "" : Crypt::encodePlaintext(data.vpnUser))
         .arg(data.vpnPassword.isEmpty() ? "" : Crypt::encodePlaintext(data.vpnPassword))
         .arg(data.vpnPkcs12.isEmpty() ? "" : Crypt::encodePlaintext(data.vpnPkcs12))
         .arg(data.httpUser.isEmpty() ? "" : Crypt::encodePlaintext(data.httpUser))
         .arg(data.httpPassword.isEmpty() ? "" : Crypt::encodePlaintext(data.httpPassword))
         .arg(data.removeOnStart));

     Database::instance()->execute(sql);
}

void Configs::cleanupConfigs()
{
    QLatin1String configSql("SELECT \"vpn-id\", \"vpn-remove-on-start\" FROM vpn;");
    QScopedPointer<QSqlQuery> configQuery (Database::instance()->openQuery(configSql));
        //
    std::vector<int> vpnIds;


    printf("%s\n", configQuery->lastError().text().toLatin1().data());

    while (configQuery->next()) {
        // OpenVpn Objekt bauen und an die Liste anfügen
        int vpnId (configQuery->value(0).toInt());
        bool removeOnStart = configQuery->value(1).toBool();

        if(removeOnStart)
        {
            removeFromDatabase(vpnId);
            continue;
        }

        vpnIds.push_back(vpnId);
    }

    std::list<int> removedIds;
    for(auto item : this->myList)
    {
        removedIds.push_back(item.first);
    }

    for(auto vpnId : vpnIds)
    {
        removedIds.remove(vpnId);
    }


    if(removedIds.size() > 0)
    {
        QMutableListIterator<QPair<int, ConnectionData*> > i(this->myList);

        while (i.hasNext()) {
            QPair <int, ConnectionData*> entry = i.next();
            ConnectionData *obj = entry.second;

            if(removedIds.front() == entry.first) {
                // Objekt löschen
                delete obj;
                obj = 0;
                // Nun aus der Liste entfernen
                i.remove();
                removedIds.pop_front();
                if(removedIds.size() == 0)
                    break;
            }
        }
    }
}

bool Configs::isConfigInList(int id)
{
    //
    // Ist die Konfiguration bereits in der Liste
    //

    QListIterator<QPair<int, ConnectionData*> > i(this->myList);
    while (i.hasNext()) {
        auto entry = i.next();
        // Den Pfad kontrollieren
        if (entry.first == id) {
            return true;
        }
    }

    return false;
}

ConnectionData *Configs::configWithId(int id)
{
    QListIterator<QPair<int, ConnectionData*> > i(this->myList);

    while (i.hasNext()) {
        QPair <int, ConnectionData*> entry = i.next();
        ConnectionData *obj = entry.second;

        if(id == entry.first) {
            return obj;
        }
    }

    return nullptr;
}

bool Configs::ConfigExists(const QString &name)
{
    refreshConfigs();
    for(auto &config : this->myList)
    {
        if(config.second->GetName() == name)
        {
            return true;
        }
    }

    return false;
}

void Configs::findConfigsInDir(const QString &sDir)
{
    QDir dir (sDir);
    QString tempDir (sDir);

    dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isDir()) {
            // Unterverzeichnis auf Configs durchsuchen
            if (fileInfo.fileName() != QLatin1String(".") && fileInfo.fileName() != QLatin1String("..")) {
                this->findConfigsInDir(sDir + QLatin1String ("/") + fileInfo.fileName());
            }
        } else {
            // Ist eine Datei, auch eine VPN Config?
            if (fileInfo.suffix() == QLatin1String("ovpn")) {


                auto configName = fileInfo.fileName().replace(QLatin1String(".ovpn"), QLatin1String(""));

                // Konfiguration der Datenbank hinzufügen
                QString configDirectory (tempDir.replace("\\", "/") + QLatin1String("/") + fileInfo.fileName());
                //
                // On portable mode we need to save only a relative path
                if (g_bPortable) {
                    QDir applicationDirectory (qApp->applicationDirPath());
                    //
                    configDirectory = applicationDirectory.relativeFilePath(configDirectory);
                }

                // Check if config already in list
                QString checkSql(QString("SELECT \"vpn-id\" FROM vpn WHERE \"vpn-name\" = '%1'")
                    .arg(Crypt::encodePlaintext(configName)));

                QScopedPointer<QSqlQuery> checkQuery (Database::instance()->openQuery(checkSql));
                //
                if (checkQuery->first()) {
                    continue;
                }

                if(ConfigExists(configName))
                    continue;

                QString sql (QString("INSERT INTO vpn  (\"vpn-name\", \"vpn-config\", \"vpn-autostart\") VALUES ('%1', '%2', 0);")
                    .arg(Crypt::encodePlaintext(configName))
                    .arg(Crypt::encodePlaintext(configDirectory)));

                Database::instance()->execute(sql);

                // Calculate the checksum
                if (!g_bPortable) {
                    QString idOfInsertSql(QString("SELECT \"vpn-id\" FROM vpn WHERE \"vpn-name\" = '%1'")
                        .arg(Crypt::encodePlaintext(configName)));

                    QScopedPointer<QSqlQuery> idOfInsertQuery (Database::instance()->openQuery(idOfInsertSql));
                    //
                    if (!idOfInsertQuery->first()) {
                        continue;
                    }

                    int configId = idOfInsertQuery->value(0).toInt();
                    Checksum::instance()->createNewChecksumFromFile(configId, configDirectory);
                }
            }
        }
    }
}
