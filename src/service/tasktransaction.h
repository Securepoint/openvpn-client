#ifndef TASKTRANSACTION_H
#define TASKTRANSACTION_H

#include <QtCore>
#include "transaction.h"

class QWrapperThread : public QThread
{
public:
    static void Sleep(unsigned long msecs) { QThread::msleep(msecs); }
};

class TaskTransaction : public QObject, public Transaction
{
    Q_OBJECT
public:
    TaskTransaction();
    virtual ~TaskTransaction(){}
    virtual void apply();

    void setCommand (const QString &com);
    void setParams (const QString &parm);

private:
    QString command;
    QString params;
    inline void qSleep(uint t) { QWrapperThread::Sleep(t);}

};

#endif // TASKTRANSACTION_H
