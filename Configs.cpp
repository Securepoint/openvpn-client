#include "Configs.h"

QList<OpenVpn*> Configs::getConfigsObjects (){
    return this->myList;
}

void Configs::clearConfigs() {
    this->refreshConnectionPath = QString("");
    foreach (OpenVpn* obj, this->myList) {
        if (!obj->isConnectionStable()) {
            this->myList.removeOne(obj);
        } else {
            // Pfad merken
            this->refreshConnectionPath = obj->configPath
                                        + QString ("/")
                                        + obj->configName
                                        + QString (".ovpn");
        }
    }
}

void Configs::searchLinkedConfigs() {
    QFile linkedOvpn (QApplication::applicationDirPath() + QString ("/configs.txt"));
    if (linkedOvpn.exists()) {
        if (!linkedOvpn.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        // Offen einlesen
        QTextStream in (&linkedOvpn);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed() != this->refreshConnectionPath) {
                // Add to list
                QFile linkFileExists (line.trimmed());
                if (linkFileExists.exists()) {
                    QString configName = line.trimmed().right(line.trimmed().size() - line.trimmed().lastIndexOf("/") -1);
                    configName = configName.left(configName.size()-5);
                    OpenVpn* myObj = new OpenVpn();
                    myObj->connectionStable = false;
                    myObj->isLinked = true;
                    myObj->configName = configName;
                    myObj->configPath = line.trimmed().left(line.lastIndexOf("/"));
                    this->myList.append(myObj);
                }
            }
        }
        linkedOvpn.close();
    }
}
void Configs::changeConfigNameInLinkedList (QString oldName, QString newName) {
    this->removeConfigFromList(oldName);
    QFile linkedWriteOvpn (QApplication::applicationDirPath() + QString ("/configs.txt"));
    if (!linkedWriteOvpn.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        return;
    QTextStream out (&linkedWriteOvpn);
    out << newName + QString ("\n");
    linkedWriteOvpn.close();
}

bool Configs::isConfigLinked(QString config) {
    bool retVal = false;
    QFile linkedOvpn (QApplication::applicationDirPath() + QString ("/configs.txt"));
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

void Configs::removeConfigFromList(QString config) {
    QFile linkedOvpn (QApplication::applicationDirPath() + QString ("/configs.txt"));
    if (linkedOvpn.exists()) {
        if (!linkedOvpn.open(QIODevice::ReadOnly | QIODevice::Text))
            return;
        // Offen einlesen
        QStringList fileContent;
        QTextStream in (&linkedOvpn);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed() != config) {
                fileContent.append(line.trimmed());
            }
        }
        linkedOvpn.close();
        // Datei schreiben
        QFile linkedWriteOvpn (QApplication::applicationDirPath() + QString ("/configs.txt"));
        if (!linkedWriteOvpn.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
            return;
        QTextStream out (&linkedWriteOvpn);
        foreach (QString str, fileContent)
            out << str + QString ("\n");
        linkedWriteOvpn.close();
    }
}

void Configs::searchConfigs (QString sDir) {
    // Dateien im aktuellen Verzeichnis suchen

    QDir dir (sDir);

    dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.isDir()) {
            // Verzeichnis auf Configs durchsuchen
            if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
                this->searchConfigs(sDir + QString ("/") + fileInfo.fileName());
        } else {
            // Ist eine Datei, VPN Config?
            if (fileInfo.suffix() == "ovpn"){
                if (this->refreshConnectionPath != sDir + QString ("/") + fileInfo.fileName()) {
                    OpenVpn* myObj = new OpenVpn();
                    myObj->connectionStable = false;
                    myObj->isLinked = false;
                    myObj->configName = fileInfo.fileName().replace(QString(".ovpn"), QString(""));
                    myObj->configPath = sDir;
                    this->myList.append(myObj);
                }
            }
        }
    }
}
