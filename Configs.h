#ifndef CONFIGS_H
#define CONFIGS_H

#include <QtGui>
#include <QDir>
#include <QDialog>
#include "openvpn.h"

class Configs
{
  public:
    Configs(){
        this->refreshConnectionPath = QString("");
    }
    ~Configs(){}

  void searchConfigs (QString sDir);
  void clearConfigs ();
  QList<OpenVpn*> getConfigsObjects ();
  void searchLinkedConfigs ();
  bool isConfigLinked (QString config);
  void removeConfigFromList (QString config);
  void changeConfigNameInLinkedList (QString oldName, QString newName);


  private:
     QList<OpenVpn*> myList;
     QString refreshConnectionPath;
};


#endif // CONFIGS_H
