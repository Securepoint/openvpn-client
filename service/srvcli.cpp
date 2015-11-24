#include "srvcli.h"

#include "qthreadexec.h"
#include "debug.h"

#include <QtCore>
#include <QtNetwork>

SrvCLI::SrvCLI(QSslSocket *  socket)
    : connectionIsOnline (false),
      port (3655),
      sslSocket(socket)
{
    this->connectionIsOnline = true;
}

void SrvCLI::send(const QString &command, const QString &params, bool fastmode)
{
    Debug::log (QString("SRVCLI: Sending: %1 with %2")
                .arg(params)
                .arg(command));

    if (!this->connectionIsOnline){
         Debug::log ("SRVCLI: No connection");
        return;
    }
    if(params.toUpper() == "LOG" && command.size() > 300)
        Debug::log("Skip too long");
    else
    {
        this->sendRequest(params, command);
        Debug::log(QLatin1String("Send done"));
    }
}

void SrvCLI::setPort(const quint16 &clientPort)
{
    this->port = clientPort;
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
    Debug::log(QString::number(block.size() - sizeof(quint64)));
    // Block an das Socket schicken und senden
    this->sslSocket->write(block);
    this->sslSocket->waitForBytesWritten();
    this->sslSocket->flush();
    this->sslSocket->waitForBytesWritten();
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

void SrvCLI::closeConnection()
{
    this->sslSocket->close();
    this->connectionIsOnline = false; // neu zum Testen
}
