// "single_application.h"

#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H

#include <QApplication>
#include <QSharedMemory>

class Preferences;
#include "preferences.h"

class SingleApplication : public QApplication
{
        Q_OBJECT
public:
        SingleApplication(int &argc, char *argv[], const QString uniqueKey);
        Preferences *prefParent;
        bool isRunning();
        bool sendMessage(const QString &message);

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
