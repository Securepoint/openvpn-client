#include "configs.h"
#include "appfunc.h"

Configs *Configs::mInst = NULL;

Configs *Configs::getInstance()
{
    if (!mInst) {
        mInst = new Configs ();
    }

    return mInst;
}

Configs::Configs ()
    : refreshConnectionPath (""),
      id (1)
{

}

QList<QPair<int, OpenVpn*> > Configs::getConfigsObjects ()
{
    return this->myList;
}

void Configs::clearConfigs()
{    
    this->refreshConnectionPath.clear();

    // OD: Mutable List statt foreach
    QMutableListIterator<QPair<int, OpenVpn*> > i(this->myList);

    while (i.hasNext()) {
        QPair <int, OpenVpn*> entry = i.next();
        OpenVpn *obj = entry.second;

        if (!obj->isConnectionStable() && !obj->isConnecting() && !obj->isError() && obj->getAdvName().isEmpty() && !obj->isDelayed()) {
            // Objekt löschen
            delete obj;
            obj = 0;
            // Nun aus der Liste entfernen
            i.remove();
        } else {
            // Pfad merken
            this->refreshConnectionPath = obj->getConfigFullPath();
        }        
    }
}

bool Configs::isConfigInList(const QString &path)
{
    //
    // Ist die Konfiguration bereits in der Liste
    //

    if (path.isEmpty()) {
        return false;
    }

    QListIterator<QPair<int, OpenVpn*> > i(this->myList);

    while (i.hasNext()) {
        QPair <int, OpenVpn*> entry = i.next();
        OpenVpn *obj = entry.second;

        // Den Pfad kontrollieren
        if (path == obj->getConfigFullPath()) {
            return true;
        }
    }

    return false;
}

void Configs::searchLinkedConfigs()
{
    QFile linkedOvpn (AppFunc::getAppSavePath() + QLatin1String ("/configs.txt"));

    if (linkedOvpn.exists()) {
        if (!linkedOvpn.open(QIODevice::ReadOnly | QIODevice::Text)) {
            return;
        }
        // Offen einlesen
        QTextStream in (&linkedOvpn);
        while (!in.atEnd()) {
            QString line = in.readLine();            
            if (!this->isConfigInList(line)) {
                // Add to list
                QFile linkFileExists (line.trimmed());
                if (linkFileExists.exists()) {
                    int _id (this->id++);
                    QString configName = line.trimmed().right(line.trimmed().size() - line.trimmed().lastIndexOf("/") - 1);
                    configName = configName.left(configName.size()-5);
                    OpenVpn* myObj = new OpenVpn();
                    myObj->setConfigStable(false);
                    myObj->setConfigLinked(true);
                    myObj->setConfigName(configName);
                    myObj->setConfigPath(line.trimmed().left(line.lastIndexOf("/")));
                    myObj->setId(_id);
                    // An die Liste anfügen
                    this->myList.append(qMakePair(_id, myObj));
                }
            }
        }
        linkedOvpn.close();
    }
}

void Configs::changeConfigNameInLinkedList (const QString &oldName, const QString &newName)
{
    if (this->isConfigInList(newName)) {
        // ist bereits drin
        return;
    }
    this->removeConfigFromList(oldName);
    QFile linkedWriteOvpn (AppFunc::getAppSavePath()  + QLatin1String ("/configs.txt"));
    if (!linkedWriteOvpn.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        return;
    }

    QTextStream out (&linkedWriteOvpn);
    out << newName + QLatin1String ("\n");
    linkedWriteOvpn.close();
}

bool Configs::isConfigLinked(const QString &config)
{
    bool retVal = false;
    QFile linkedOvpn (AppFunc::getAppSavePath()  + QLatin1String ("/configs.txt"));
    if (linkedOvpn.exists()) {
        if (!linkedOvpn.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;
        // Offen einlesen
        QTextStream in (&linkedOvpn);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed() == config) {
                retVal = true;
                break;
            }
        }
        linkedOvpn.close();
    }
    return retVal;
}

void Configs::removeConfigFromList(const QString &config)
{
    QString configClean (config);
    configClean = configClean.replace("\\", "/");

    QFile linkedOvpn (AppFunc::getAppSavePath()  + QLatin1String ("/configs.txt"));
    if (linkedOvpn.exists()) {
        if (!linkedOvpn.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        // Offen einlesen
        QStringList fileContent;
        QTextStream in (&linkedOvpn);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed() != configClean) {
                fileContent.append(line.trimmed());
            }
        }
        linkedOvpn.close();
        // Datei schreiben
        QFile linkedWriteOvpn (AppFunc::getAppSavePath()  + QLatin1String ("/configs.txt"));
        if (!linkedWriteOvpn.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
            return;
        QTextStream out (&linkedWriteOvpn);
        foreach (QString str, fileContent)
            out << str + QLatin1String ("\n");
        linkedWriteOvpn.close();
    }
}

void Configs::searchConfigs (QString sDir)
{
    // Dateien im aktuellen Verzeichnis suchen

    QDir dir (sDir);

    dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isDir()) {
            // Verzeichnis auf Configs durchsuchen
            if (fileInfo.fileName() != "." && fileInfo.fileName() != QLatin1String("..")) {
                this->searchConfigs(sDir + QLatin1String ("/") + fileInfo.fileName());
            }
        } else {
            // Ist eine Datei, auch eine VPN Config?
            if (fileInfo.suffix() == QLatin1String("ovpn")){                
                if (!this->isConfigInList(sDir.replace("\\", "/") + QLatin1String ("/") + fileInfo.fileName())) {
                    int _id (this->id++);
                    OpenVpn* myObj = new OpenVpn();
                    myObj->setConfigStable(false);
                    myObj->setConfigLinked(false);
                    myObj->setConfigName(fileInfo.fileName().replace(QLatin1String(".ovpn"), QLatin1String("")));
                    myObj->setConfigPath(sDir);
                    myObj->setId(_id);

                    this->myList.append(qMakePair(_id, myObj));
                }
            }
        }
    }
}

void Configs::appendConfigToList(OpenVpn *obj)
{    
    int _id (this->id++);
    obj->setId(_id);
    this->myList.append(qMakePair(_id, obj));
}
