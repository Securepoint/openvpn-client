#ifndef CHECKUPDATE_H
#define CHECKUPDATE_H

#include <QCoreApplication>
#include <QtCore>
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>
#include <QtNetwork/QAuthenticator>
#include "netmanager.h"

class CheckUpdate : public QObject
{
    Q_OBJECT
public:
    CheckUpdate();
    ~CheckUpdate();
    void sendRequest();

private:
    NetManager nam;
    QUrl url;
    QString _error;

signals:
    void finished(const QString &xmlStr);
    void error (const QString &errorMessage);

private slots:
    void finishedSlot(QNetworkReply *reply);
};

#endif // CHECKUPDATE_H
