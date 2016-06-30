#ifndef NETMANAGER_H
#define NETMANAGER_H

#include <QtCore>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QAuthenticator>

class NetManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetManager(QObject *parent = 0);

protected:
    QNetworkReply *createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData = 0);

private:
    QNetworkProxy proxy;

private slots:
    void proxyAuthenticationRequired(const QNetworkProxy &proxy, QAuthenticator *auth);

signals:
    void requestCreated(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QNetworkReply *reply);
};

#endif // NETMANAGER_H
