#ifndef QTHREADEXEC_H
#define QTHREADEXEC_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMovie>
#include <QtCore/qstack.h>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/qthread.h>

class QThreadExec : public QThread
{
public:
    static void qSleep(uint t) { QThreadExec::Sleep(t);}
    static void Sleep(unsigned long msecs) { QThread::msleep(msecs); }

protected:
    void run () {exec();}
};

#endif // QTHREADEXEC_H
