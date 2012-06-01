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
    QList<QPair<int, OpenVpn*> > getConfigsObjects ();
    void searchLinkedConfigs ();
    bool isConfigLinked (const QString &config);
    void removeConfigFromList (const QString &config);
    void changeConfigNameInLinkedList (const QString &oldName, const QString &newName);
    void appendConfigToList (OpenVpn *obj);        

  private:
     Configs();
     static Configs *mInst;
     QList<QPair<int, OpenVpn*> > myList;
     QString refreshConnectionPath;
     int id;
     bool isConfigInList (const QString &path);
};


#endif // CONFIGS_H
