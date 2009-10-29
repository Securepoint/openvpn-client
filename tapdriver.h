#ifndef TAPDRIVER_H
#define TAPDRIVER_H

#include <QtGui>

class TapDriver: public QObject
{
    Q_OBJECT

public:
    TapDriver();
    virtual ~TapDriver(){}
    bool isTapDriverInstalled ();
    bool installTapDriver ();
    bool removeTapDriver ();

private:
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
