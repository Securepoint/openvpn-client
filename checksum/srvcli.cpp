#include "srvcli.h"

SrvCLI *SrvCLI::mInst = NULL;

SrvCLI *SrvCLI::instance() {
    if (!mInst)
        mInst = new SrvCLI ();
    return mInst;
}

SrvCLI::SrvCLI()
    : connectionIsOnline (false),
    blockSize (0)
{
    QObject::connect (&this->sslSocket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
    QObject::connect (&this->sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    QObject::connect (&this->sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slot_sslErrors(QList<QSslError>)));
    QObject::connect (&this->sslSocket, SIGNAL(peerVerifyError(QSslError)), this, SLOT(slot_peerVerifyError(QSslError)));
    QObject::connect (&this->sslSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));

    QObject::connect(&this->sslSocket, SIGNAL(disconnected()), SLOT(slotConnectionClosed()));
    QObject::connect(&this->sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(slotError(QAbstractSocket::SocketError)));
}

bool SrvCLI::send(const QString &command, const QString &params)
{
    if (!this->connectionIsOnline){
        if (!this->makeFastConnection()) {
            // Keine Verbindung
            return false;
        }
    }

    this->sendRequest(command, params);
    //
    return true;
}

void SrvCLI::sendRequest (const QString &command, const QString &params)
{
    // Neuen Block zum Senden erstellen
    QByteArray block;
    // Datasteam an den Block binden
    QDataStream out (&block, QIODevice::WriteOnly);
    // DataStream version setzen, hier aktuelle 4.6 = DataStream version 10
    out.setVersion(QDataStream::Qt_4_6);
    // Größe des Blockes erstmal mit 0 initieren und Aktion angeben
    out << quint64(0);
    out << command;
    out << params;
    // Wieder an die erste Stelle des Blockes springen und die Größe neu setzen
    out.device()->seek(0);
    out << quint64(block.size() - sizeof(quint64));
    // Block an das Socket schicken und senden
    this->sslSocket.write(block);
    this->sslSocket.flush();
}


void SrvCLI::slot_peerVerifyError(const QSslError &error)
{
    Q_UNUSED (error)
}

void SrvCLI::slot_sslErrors(const QList<QSslError> &errors)
{

    Q_UNUSED(errors)
    //
    this->sslSocket.ignoreSslErrors();
}

void SrvCLI::slot_stateChanged(QAbstractSocket::SocketState state)
{
    QString stateMessage ("");
    if (state == QAbstractSocket::UnconnectedState) {
        stateMessage = "UnconnectedState: The socket is not connected.";
        this->connectionIsOnline = false;
    } else if (state == QAbstractSocket::HostLookupState) {
        stateMessage = "HostLookupState: The socket is performing a host name lookup.";
    } else if (state == QAbstractSocket::ConnectingState) {
        stateMessage = "ConnectingState: The socket has started establishing a connection.";
    } else if (state == QAbstractSocket::ConnectedState) {
        stateMessage = "ConnectedState: A connection is established.";
    } else if (state == QAbstractSocket::BoundState) {
        stateMessage = "BoundState: The socket is bound to an address and port (for servers).";
    } else if (state == QAbstractSocket::ClosingState) {
        stateMessage = "ClosingState: The socket is about to close (data may still be waiting to be written).";
    } else if (state == QAbstractSocket::ListeningState) {
        stateMessage = "ListeningState: For internal use only.";
    }

    QThread::msleep(100);
}

bool SrvCLI::isOnline() const
{
    return (this->sslSocket.state() == QSslSocket::ConnectedState);
}

void SrvCLI::stopRequest ()
{
    this->response = QString("--stop request");
    this->closeConnection();
}

void SrvCLI::connectionClosedByServer ()
{
    this->closeConnection();
}

void SrvCLI::error ()
{
    this->response = this->sslSocket.errorString();
    this->closeConnection();
}

void SrvCLI::closeConnection()
{
    this->sslSocket.close();
    this->connectionIsOnline = false; // neu zum Testen
}

void SrvCLI::resetConnection()
{
    if (this->connectionIsOnline) {
        this->closeConnection();
    }
}

bool SrvCLI::makeFastConnection()
{
    if (this->sslSocket.state() != QAbstractSocket::UnconnectedState) {
        qDebug() << "socket is not in unconnected state";
        //
        return true;
    }

    //
    this->sslSocket.connectToHost(QLatin1String("127.0.0.1"), 3656);
    //
    if (!this->sslSocket.waitForConnected(1900)) {
        qDebug() << "Connect to service failed";
        //
        return false;
    }

    // All fine, set new state
    this->connectionIsOnline = true;

    return true;
}

void SrvCLI::slotConnectionClosed()
{
    //
    // Connection is closed emit signal after we wait a short time
    //
    emit connectionIsClosed();
}

void SrvCLI::slotError(QAbstractSocket::SocketError err)
{
    //
    // Error while conntecting
    //

    Q_UNUSED(err)

    if (sslSocket.state() != QAbstractSocket::ConnectedState){
        slotConnectionClosed();
    }
}
