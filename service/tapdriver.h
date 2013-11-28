#ifndef TAPDRIVER_H
#define TAPDRIVER_H

#include <QtCore>

class TapDriver: public QObject
{
    Q_OBJECT

public:
    TapDriver();
    bool isTapDriverInstalled ();
    bool installTapDriver ();
    bool removeTapDriver ();

private:
    bool tapDriverAvailable;
    bool tapDriverInstalledSuccess;
    bool tapDriverRemovedSuccess;

    QProcess drvProc;
    QProcess drvInstallProc;
    QProcess drvRemoveProc;

    void checkTapDriver ();

private slots:
    void readDriverData ();
};

#endif // TAPDRIVER_H
