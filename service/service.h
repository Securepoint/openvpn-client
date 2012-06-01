#ifndef SERVICE_H
#define SERVICE_H

#include <QtCore>
#include "qtservice.h"
#include "sslserver.h"

class Service  : public QObject, public QtService<QCoreApplication>
{
    Q_OBJECT
public:
    Service(int argc, char **argv);

protected:
    void start();
    void stop();
    void pause();
    void resume();

private:
    SslServer *server;
};

#endif // SERVICE_H
