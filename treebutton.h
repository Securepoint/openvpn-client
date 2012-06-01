#ifndef TREEBUTTON_H
#define TREEBUTTON_H

#include <QtGui>
#include <QtCore>
#include <QDate>
#include <QTime>
#include <QObject>
#include <QTimer>

#include "treeconitem.h"
#include "configs.h"


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

    int getState () const;

private:
    QTreeWidgetItem *parentItem;
    QTimer timer;
    int state;

public slots:
    void receivedError (QString errMessage);
    void receivedConnctionIsStable (QString ip);
    void receivedDisconnect ();
    void receivedConnecting ();

private slots:
    void doReconnect ();
    void refreshConnectedSince ();
    void clickMe ();
};

#endif // TREEBUTTON_H
