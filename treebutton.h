#ifndef TREEBUTTON_H
#define TREEBUTTON_H

#include <QtGui>
#include <QtCore>
#include <QDate>
#include <QTime>
#include <QObject>
#include <QTimer>

#include "treeconitem.h"
#include "mainwindowcontroll.h"
#include "Configs.h"


class TreeButton : public  QPushButton
{
    Q_OBJECT
public:
    TreeButton();
    void setParentItem (QTreeWidgetItem *item);
    QTreeWidgetItem *getParentItem ();
    TreeConItem *getParentItemCast ();
    void setReadyToConnect ();
    void setConnecting ();
    void setConnected (QString ip);
    void setError (QString error);

private:
    QTreeWidgetItem *parentItem;
    QTimer *timer;

public slots:
    void receivedError (QString errMessage);
    void receivedConnctionIsStable (QString ip);
    void receivedDisconnect ();
    void receivedConnecting ();

private slots:
    void doReconnect ();
    void refreshConnectedSince ();

signals:
    void sendEnableTree ();
};

#endif // TREEBUTTON_H
