#include "srvcli.h"

#include "../qthreadexec.h"
#include "../debug.h"
#include "../settings.h"

SrvCLI *SrvCLI::mInst = NULL;

SrvCLI *SrvCLI::instance() {
    if (!mInst)
        mInst = new SrvCLI ();
    return mInst;
}

SrvCLI::SrvCLI()
    : connectionIsOnline (false)
{    
    QObject::connect (&this->sslSocket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));
    QObject::connect (&this->sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error()));
    QObject::connect (&this->sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(slot_sslErrors(QList<QSslError>)));
    QObject::connect (&this->sslSocket, SIGNAL(peerVerifyError(QSslError)), this, SLOT(slot_peerVerifyError(QSslError)));
    QObject::connect (&this->sslSocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(slot_stateChanged(QAbstractSocket::SocketState)));
}

void SrvCLI::send(const QString &command, const QString &params, bool fastmode)
{
    if (!this->connectionIsOnline){        
        if (!this->makeConnection(fastmode)) {    
            // Keine Verbindung
            return;
        }
    }    

    this->sendRequest(command, params);
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
    this->sslSocket.waitForBytesWritten(1000);
    this->sslSocket.flush();
}


void SrvCLI::slot_peerVerifyError(const QSslError &error)
{
    Q_UNUSED (error)
}

void SrvCLI::slot_sslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED (errors)
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
    QThreadExec::Sleep(100);    
}

bool SrvCLI::isOnline() const
{
    return this->connectionIsOnline;
}

void SrvCLI::stopRequest ()
{
    this->response = QString("--stop request");    
    this->closeConnection();
}

void SrvCLI::connectionClosedByServer ()
{
    if (this->nextBlockSize != 0xFFFF) {
        this->response = QString("--error connection closed by server");
    }    
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

bool SrvCLI::makeConnection(bool fastmode)
{
    static int requestErrorCount = 0;

    this->sslSocket.connectToHostEncrypted(QLatin1String("127.0.0.1"), Settings::getInstance()->getServerPort());
    this->sslSocket.ignoreSslErrors();

    if (!this->sslSocket.waitForConnected(6000)) {        
        requestErrorCount++;
        if (fastmode || requestErrorCount == 3) {
            requestErrorCount = 0;            
            Debug::error(QLatin1String("Connection Timeout"));
            return false;
        } else {
            return this->makeConnection(fastmode);
        }
    }

    if (!this->sslSocket.waitForEncrypted(6000)) {        
        requestErrorCount++;
        if (fastmode || requestErrorCount == 7) {
            requestErrorCount = 0;            
            Debug::error(QLatin1String("Encryption error"));
            return false;
        } else {
            return this->makeConnection(fastmode);
        }
    }
    // Hat alles geklappt
    requestErrorCount = 0;
    this->connectionIsOnline = true;

    return true;
}
