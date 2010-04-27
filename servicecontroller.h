#ifndef SERVICECONTROLLER_H
#define SERVICECONTROLLER_H

#include "preferences.h"

#include <QtGui>
#include <QTimer>
#include <QSystemTrayIcon>



#ifdef Q_OS_WIN32
    #include "C:\Qt\2009.04\mingw\include\windows.h"
    #include "C:\Qt\2009.04\mingw\include\winsock2.h"
#endif

class ServiceController : public QDialog
{
    Q_OBJECT
public:
    ServiceController(QWidget *parent = 0);
    virtual ~ServiceController (){}
    void stopService ();
    void startService ();
    void restartService ();
    bool isServiceRunning ();

private slots:
    void shotStart ();

public slots:
    void checkService ();

private:
    // Enum für die Icons
    enum Icons {
        Inaktiv,
        Connected,
        Error,
        Connecting
    };
    bool serviceRunning;
    QTimer *servTimer;
};

#endif // SERVICECONTROLLER_H
