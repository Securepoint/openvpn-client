#include "configs.h"
#include "appfunc.h"

#include "database.h"
#include "debug.h"
#include "preferences.h"

Configs *Configs::mInst = NULL;

Configs *Configs::getInstance()
{
    if (!mInst) {
        mInst = new Configs ();
    }

    return mInst;
}

Configs::Configs ()
{

}

void Configs::setDatabase(Database *db)
{
    this->db = db;
}

QList<QPair<int, OpenVpn*> > Configs::getConfigsObjects ()
{
    return this->myList;
}

int Configs::configCount() const
{
    return this->myList.size();
}

void Configs::clearConfigs()
{
    //
    // Löscht die Liste der Verbindungen, bis auf Konfiguration, die sich gerade verbinden, verbunden sind oder einen Fehler haben
    //

    // OD: Mutable List statt foreach
    QMutableListIterator<QPair<int, OpenVpn*> > i(this->myList);

    while (i.hasNext()) {
        QPair <int, OpenVpn*> entry = i.next();
        OpenVpn *obj = entry.second;

        if (!obj->isConnectionStable() && !obj->isConnecting() && !obj->isError()) {
            // Objekt löschen
            delete obj;
            obj = 0;
            // Nun aus der Liste entfernen
            i.remove();
        }
    }
}

bool Configs::isConfigInList(int id)
{
    //
    // Ist die Konfiguration bereits in der Liste
    //

    QListIterator<QPair<int, OpenVpn*> > i(this->myList);
    while (i.hasNext()) {
        QPair <int, OpenVpn*> entry = i.next();
        // Den Pfad kontrollieren
        if (entry.first == id) {
            return true;
        }
    }

    return false;
}

void Configs::refreshConfigs()
{
    //
    // Query DB
    //

	{
		QLatin1String configSql("SELECT [vpn-id], [vpn-name], [vpn-config], [vpn-autostart] FROM vpn ORDER BY [vpn-name] COLLATE NOCASE ASC;");
		QScopedPointer<QSqlQuery> configQuery (this->db->openQuery(configSql));

		while (configQuery->next()) 
		{
			int vpnId (configQuery->value(0).toInt());
			QString vpnName (configQuery->value(1).toString());
			QString vpnConfig (configQuery->value(2).toString());


			QString sql (QString("UPDATE vpn SET [vpn-config] = '%1' WHERE [vpn-id] = %2")
				.arg(this->db->makeCleanValue(vpnConfig.replace("\\", "/")))
				.arg(vpnId));

			this->db->execute(sql);
		}
	};


    this->clearConfigs();
    // Ist das der erste Refresh ist,
    // dann werden die Verzeichnisse mit durchsucht und
    // die gefundenen Konfigurationen in die Datenbank geschrieben
    if (Settings::getInstance()->getIsPortableClient()) {
        this->searchConfigs(qApp->applicationDirPath());
    } else {
        this->searchConfigs(AppFunc::getAppSavePath() + QLatin1String("/config"));
    }


    //
    // Nun die Liste aus der Datenbank füllen
    QLatin1String configSql("SELECT [vpn-id], [vpn-name], [vpn-config], [vpn-autostart] FROM vpn ORDER BY [vpn-name] COLLATE NOCASE ASC;");
    QScopedPointer<QSqlQuery> configQuery (this->db->openQuery(configSql));
    //
    while (configQuery->next()) {
        // OpenVpn Objekt bauen und an die Liste anfügen
        int vpnId (configQuery->value(0).toInt());
        QString vpnName (configQuery->value(1).toString());
        QString vpnConfig (configQuery->value(2).toString());
        Debug::log(QString("Configs: Config path: %1").arg(vpnConfig));
        bool vpnAutoStart ((configQuery->value(3).toString() == QLatin1String("1") ? true : false));
        //
        if (this->isConfigInList(vpnId)) {
            continue;
        }

        // Im portabel modus werden relative pfade gespeichert,
        // diese müssen nun wieder in absolute geändert werden
        if (Settings::getInstance()->getIsPortableClient()) {
            Debug::log(QLatin1String("Configs: Portabel mode"));
            vpnConfig = QDir::cleanPath(QDir().absoluteFilePath(vpnConfig));
        }

        Debug::log(QString("Configs: New config path: %1").arg(vpnConfig));

        OpenVpn* obj = new OpenVpn(this->db);
        obj->setConfigName(vpnName);
        obj->setId(vpnId);
        obj->setConfigPath(vpnConfig);
        obj->setStartConfig(vpnAutoStart);

        this->myList.append(qMakePair(vpnId, obj));
    }
}

void Configs::searchConfigs (const QString &sDir)
{
    // Dateien im übergebenen Verzeichnis suchen

    QDir dir (sDir);
    QString tempDir (sDir);

    dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isDir()) {
            // Unterverzeichnis auf Configs durchsuchen
            if (fileInfo.fileName() != QLatin1String(".") && fileInfo.fileName() != QLatin1String("..")) {
                this->searchConfigs(sDir + QLatin1String ("/") + fileInfo.fileName());
            }
        } else {
            // Ist eine Datei, auch eine VPN Config?
            if (fileInfo.suffix() == QLatin1String("ovpn")){
                // Konfiguration der Datenbank hinzufügen
                QString configDirectory (tempDir.replace("\\", "/") + QLatin1String("/") + fileInfo.fileName());
                if (Settings::getInstance()->getIsPortableClient()) {
                    QDir td (QCoreApplication::applicationDirPath());
                    configDirectory = td.relativeFilePath(configDirectory);
                }
                QString configName (fileInfo.fileName().replace(QLatin1String(".ovpn"), QLatin1String("")));
                // Check if config already in list
                QString checkSql(QString("SELECT [vpn-id] FROM vpn WHERE [vpn-name] = '%1' AND [vpn-config] = '%2'")
                                 .arg(configName)
                                 .arg(configDirectory));
                QScopedPointer<QSqlQuery> checkQuery (this->db->openQuery(checkSql));
                //
                if (checkQuery->first()) {
                    // A config with this name and path is already in list
                    // Skip it
                    continue;
                }

                // Config is new, append it to the database
                QString sql (QString("INSERT INTO vpn  ([vpn-name], [vpn-config], [vpn-autostart]) VALUES ('%1', '%2', 0);")
                             .arg(configName)
                             .arg(configDirectory));
                this->db->execute(sql);
            }
        }
    }
}
