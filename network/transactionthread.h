#ifndef TRANSACTIONTHREAD_H
#define TRANSACTIONTHREAD_H

#include <QtCore>

#include <QMutex>
#include <QQueue>
#include <QThread>
#include <QWaitCondition>

class Transaction;

class TransactionThread : public QThread
{
    Q_OBJECT

public:
    TransactionThread();
    ~TransactionThread();
    void addTransaction(Transaction *transact);

protected:
    void run();

private:
    QQueue<Transaction *> transactions;
    QWaitCondition transactionAdded;
    QMutex mutex;
};


#endif // TRANSACTIONTHREAD_H
