#ifndef SERVICE_H
#define SERVICE_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QSystemTrayIcon>
#include <QtCore/QEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMovie>
#include <QtCore/qstack.h>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include "qtservice.h"
#include "sslserver.h"

class Service  : public QObject, public QtService<QCoreApplication>
{
    Q_OBJECT
public:
    Service(int argc, char **argv);
    static Service * instance()
    {
        return _sInstance;
    }

    SslServer * GetServer()
    {
        return server;
    }
protected:
    void start();
    void stop();
    void pause();
    void resume();



private:
    SslServer *server;
    static Service * _sInstance;
};

#endif // SERVICE_H
