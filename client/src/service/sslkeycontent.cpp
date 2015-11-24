#include "sslkeycontent.h"

SslKeyContent *SslKeyContent::mInst = NULL;

SslKeyContent *SslKeyContent::instance() {
    //
    // Return a single instance from the key content object
    //
    if (!mInst) {
        mInst = new SslKeyContent ();
    }

    return mInst;
}

SslKeyContent::SslKeyContent()
{
    //
    // Build a new key content object
    // Open the key file and read all content
    //
    this->keyContent.clear();

    // Open file
    QFile file(QCoreApplication::applicationDirPath() + QLatin1Char('/') + PRIVATEKEY_FILE);
    if (file.open(QIODevice::ReadOnly)) {
        this->keyContent = file.readAll();        
        // Wait 1 second before the file is closed
        file.waitForReadyRead(1000);
        file.close();
    }
}

const bool SslKeyContent::keyIsValid() const {
    //
    // Return true if the key file content is not empty
    //
    return (this->keyContent.isEmpty() ? false : true);
}

const QByteArray SslKeyContent::getKey() const {
    //
    // Returns the key file content
    //
    return this->keyContent;
}

