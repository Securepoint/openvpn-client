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
                    myObj->configName = fileInfo.fileName().replace(QString(".ovpn"), QString(""));
                    myObj->configPath = sDir;
                    this->myList.append(myObj);
                }
            }
        }
    }
}
