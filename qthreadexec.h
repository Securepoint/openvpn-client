#ifndef QTHREADEXEC_H
#define QTHREADEXEC_H

#include <QtCore>

class QThreadExec : public QThread
{
public:
    static void qSleep(uint t) { QThreadExec::Sleep(t);}
    static void Sleep(unsigned long msecs) { QThread::msleep(msecs); }

protected:
    void run () {exec();}

};

#endif // QTHREADEXEC_H
