#ifndef CHECKUPDATE_H
#define CHECKUPDATE_H
#include <QCoreApplication>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkProxy>
#include <QAuthenticator>
#include "../netmanager.h"

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
