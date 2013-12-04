#ifndef CONFIGS_H
#define CONFIGS_H

#include <QtGui>
#include <QDir>
#include <QDialog>
#include "openvpn.h"

class Database;

class Configs
{

 public:
    static Configs *getInstance ();
    void setDatabase (Database *db);
    void refreshConfigs ();
    QList<QPair<int, OpenVpn*> > getConfigsObjects ();
    int configCount () const;

  private:
     Configs();
     static Configs *mInst;
     QList<QPair<int, OpenVpn*> > myList;
     bool isConfigInList (int id);
     void searchConfigs (const QString &sDir);
     void clearConfigs ();
     Database *db;
};


#endif // CONFIGS_H
