#ifndef TAPDRIVER_H
#define TAPDRIVER_H

#include <QtGui>
#include "check64.h"

class TapDriver: public QObject
{
    Q_OBJECT

public:
    static TapDriver *instance ();
    bool isTapDriverInstalled ();
    bool installTapDriver ();
    bool removeTapDriver ();
    int deviceCount ();

private:
    static TapDriver *mInst;
    TapDriver();

    QProcess drvProc;
    QProcess drvCountProc;
    QProcess drvInstallProc;
    QProcess drvRemoveProc;

    void checkTapDriver ();
    bool tapDriverAvailable;
    bool tapDriverInstalledSuccess;
    bool tapDriverRemovedSuccess;
    int tapCount;
    bool wait;

    QString getTapPath ();

private slots:
    void readDriverData ();
    void readTapCount ();
};

#endif // TAPDRIVER_H
