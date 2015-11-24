#include "checkupdate.h"

#include <widgets/settings/client/settings.h>

CheckUpdate::CheckUpdate()
{
    QObject::connect(&this->nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finishedSlot(QNetworkReply*)));
    //
    this->url = Settings::instance()->updateUrl();
}

CheckUpdate::~CheckUpdate()
{
}

void CheckUpdate::sendRequest()
{
    this->nam.get(QNetworkRequest(this->url));
}

void CheckUpdate::finishedSlot(QNetworkReply *reply)
{
    // No error received?
    if (reply->error() == QNetworkReply::NoError) {
        // Reading bytes form the reply
        QByteArray bytes = reply->readAll();  // bytes
        QString string(bytes); // string
        emit finished(string);
    } else {
        emit error(reply->errorString());
    }
}
