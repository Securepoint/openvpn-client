#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <QtCore>

class Checksum
{
public:
    static Checksum* instance();

    //
    void createNewChecksum (const QString &content, qint32 configId, const QString &configPath);
    void createNewChecksumFromFile (qint32 configId, const QString &configPath);
    void removeChecksum(qint32 configId);

    QString base64OfContent (const QString &content);
    QString base64OfContent (const QStringList &content);

    QByteArray base64OfFileContent(qint32 configId, const QString &path);

private:
    Checksum();
    static Checksum* thisInstance;

    QByteArray normalizeContent(QString content);
};

#endif // CHECKSUM_H
