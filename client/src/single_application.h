#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H

#include <QApplication>
#include <QSharedMemory>
#include <windows.h>
#include <service\srvcli.h>


class SingleApplication : public QApplication
{
        Q_OBJECT
public:
        SingleApplication(int &argc, char *argv[], const QString uniqueKey);
        SingleApplication(int &argc, char *argv[]);
        bool isRunning();
        bool sendMessage(const QString &message);
        void setSharedKey(const QString &key);

protected:
        virtual bool winEventFilter(MSG* msg, long* result);

public slots:
        void checkForMessage();
        void receiveMessage(QString message);

signals:
        void messageAvailable(QString message);

private:
        bool _isRunning;
        QSharedMemory sharedMemory;


};
#endif // SINGLE_APPLICATION_H
