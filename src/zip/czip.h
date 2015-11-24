#ifndef czip_h
#define czip_h

#include <QtCore>

class CZip
{
public:
    static bool extract(const QString &filePath, const QString &extDirPath, const QString &singleFileName = QString(""), const QString &password = "");
    static bool archive(const QString &filePath, const QDir &dir, const QString &comment = QString(""));
    static bool recurseAddDir(const QDir &dir, QStringList *sl);
    static bool archiveFile(const QString &FileArchPath, const QString &FileSourcePath, const bool &AddMode = true, const QString &comment = QString(""));
};

#endif // czip_h