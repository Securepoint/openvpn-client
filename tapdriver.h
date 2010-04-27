#ifndef TAPDRIVER_H
#define TAPDRIVER_H

#include <QtGui>
#include "check64.h"

class TapDriver: public QObject
{
    Q_OBJECT

public:
    static TapDriver *getInstance ();
    bool isTapDriverInstalled ();
    bool installTapDriver ();
    bool removeTapDriver ();

private:
    static TapDriver *mInst;
    TapDriver();
    QProcess *drvProc;
    QProcess *drvInstallProc;
    QProcess *drvRemoveProc;
    void checkTapDriver ();
    bool tapDriverAvailable;
    bool tapDriverInstalledSuccess;
    bool tapDriverRemovedSuccess;

private slots:
    void readDriverData ();
    void readDriverInstallData ();
    void readDriverRemoveData ();
};

#endif // TAPDRIVER_H
