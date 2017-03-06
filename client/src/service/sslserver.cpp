#include "sslserver.h"
#include "sslkeycontent.h"


SslServer::SslServer(quint16 port, QObject *parent)
    : QTcpServer(parent),
      connections (new SslConnections)
{
    //
    // Building a TCP Server which is listen at the port
    //
   listen(QHostAddress::LocalHost, port);
}

SslServer::~SslServer()
{
    //
    // Clear the connections list when programs ends
    //

    this->connections->clear();
}

void SslServer::incomingConnection(int socketDescriptor)
{
    //
    // A new connection is incomming, create a new connection object and append it to the connection list
    //
    SslServerConnection *newIncommingConnection = new SslServerConnection (socketDescriptor, this);

    // Add the connection to the current connected list
    this->connections->add(newIncommingConnection);

    // When we receives the connectionIsClosed signal we exit the eventloop in the thread
    QObject::connect(newIncommingConnection, SIGNAL(connectionIsClosed()), this, SLOT(slotQuitThread()));
    // When the thread is finished cleanup
    QObject::connect(newIncommingConnection, SIGNAL(finished()), this, SLOT(slotThreadFinished()));
    // Received a disconnect from the service
    QObject::connect(newIncommingConnection, SIGNAL(setDisconnected(int)), this, SIGNAL(setDisconnected(int)));
    // Received an error from the service
    QObject::connect(newIncommingConnection, SIGNAL(setError(int,QString)), this, SIGNAL(setError(int,QString)));
    // Received a user input is needed
    QObject::connect(newIncommingConnection, SIGNAL(needUserInput(int,int)), this, SIGNAL(needUserInput(int,int)));
    // Received a dummy response
    QObject::connect(newIncommingConnection, SIGNAL(receivedDummy()), this, SIGNAL(receivedDummy()));
    // Connection is stable
    QObject::connect(newIncommingConnection, SIGNAL(receivedIP(int,QString)), this, SIGNAL(receivedIP(int,QString)));
    // Connection is reconnecting
    QObject::connect(newIncommingConnection, SIGNAL(receivedReconnect(int)), this, SIGNAL(receivedReconnect(int)));
    // Tap install controll
    QObject::connect(newIncommingConnection, SIGNAL(receivedTapControl(int)), this, SIGNAL(receivedTapControl(int)));
    // Tap remove controll
    QObject::connect(newIncommingConnection, SIGNAL(receivedRemoveTap(QString)), this, SIGNAL(receivedRemoveTap(QString)));

    QObject::connect(newIncommingConnection, SIGNAL(receivedTapCount(int)), this, SIGNAL(receivedTapCount(int)));

    // Received a status
    QObject::connect(newIncommingConnection, SIGNAL(receivedStatus(int,bool,bool,int,QString)), this, SIGNAL(receivedStatus(int,bool,bool,int,QString)));

    // Start thread now
    newIncommingConnection->start(QThread::NormalPriority);
}

SslConnections *SslServer::connectionsList() const
{
    //
    // Return the connection list
    //
    return this->connections;
}

void SslServer::slotThreadFinished()
{
    //
    // Thread is done, mark the object to delete
    //

    // Remove the connection from the connection list
    SslServerConnection *con = qobject_cast<SslServerConnection*>(sender());
    Q_ASSERT(con);

    if (con) {
        // Remove item with thread id from the list
        this->connections->remove(con->threadId());
    }
    // mark object to delete
    sender()->deleteLater();
}


void SslServer::slotQuitThread()
{
    //
    // Stop the thread
    //
    SslServerConnection *con = qobject_cast<SslServerConnection*>(sender());
    Q_ASSERT(con);

    if (!con) {
        return;
    }
    // Object is valid
    con->quit();
    // Wait 5 seconds to end the thread, then terminate it
    if (!con->wait(5000)) {
        con->terminate();
        // Wait 2 sec
        con->wait(2000);
    }
}
