#ifndef TAPDRIVER_H
#define TAPDRIVER_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMovie>
#include <QtCore/qstack.h>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore>
#include <QProcess>

class TapDriver: public QObject
{
    Q_OBJECT

public:
    TapDriver();
    bool isTapDriverInstalled ();
    bool installTapDriver ();
    bool removeTapDriver ();
    int deviceCount();

private:
    bool tapDriverAvailable;
    bool tapDriverInstalledSuccess;
    bool tapDriverRemovedSuccess;

    QProcess drvProc;
    QProcess drvInstallProc;
    QProcess drvRemoveProc;
    QProcess drvCountProc;
    QString arch;

    int tapCount;
    bool wait;

    void checkTapDriver ();

private slots:

    void readTapCount ();
    void readDriverData ();
};

#endif // TAPDRIVER_H
