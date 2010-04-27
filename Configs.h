#ifndef CONFIGS_H
#define CONFIGS_H

#include <QtGui>
#include <QDir>
#include <QDialog>
#include "openvpn.h"

class Configs
{
  public:
    static Configs *getInstance ();
    void searchConfigs (QString sDir);
    void clearConfigs ();
    QList<OpenVpn*> getConfigsObjects ();
    void searchLinkedConfigs ();
    bool isConfigLinked (QString config);
    void removeConfigFromList (QString config);
    void changeConfigNameInLinkedList (QString oldName, QString newName);
    void appendConfigToList (OpenVpn *obj);    
    void swap (int i, int j);

  private:
     Configs(){
          this->refreshConnectionPath = QString("");
     }
     static Configs *mInst;
     QList<OpenVpn*> myList;
     QString refreshConnectionPath;
};


#endif // CONFIGS_H
