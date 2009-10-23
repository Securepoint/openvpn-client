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

    }
    ~Configs(){}

  void searchConfigs (QString sDir);
  QList<OpenVpn*> getConfigsObjects ();


  private:
     QList<OpenVpn*> myList;
};


#endif // CONFIGS_H
