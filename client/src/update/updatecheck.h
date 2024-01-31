#ifndef UPDATECHECK_H
#define UPDATECHECK_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkProxyFactory>


class UpdateCheck : public QObject
{
    Q_OBJECT
public:
    UpdateCheck();
    ~UpdateCheck();
    void run();
    void onError(QNetworkReply::NetworkError code);
    void sslErrors(QNetworkReply *reply, const QList<QSslError> &error);
    QString* proxyAuthenticationRequired();
    void finished (QString reply);


private:
    QNetworkProxy proxy;
    QString formatCertificate(QSslCertificate sslCertificate);    
    QByteArray acceptedCertificates ();
    void setAcceptedCertificates (const QByteArray &data);
    //
    int currentMajor;
    int currentMinor;
    int currentPatch;

signals:
    void updateAvailable(QString version);
};

#endif // UPDATECHECK_H
