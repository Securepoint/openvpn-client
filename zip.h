#ifndef ZIP_H
#define ZIP_H

#include "quazip/quazip.h"
#include "quazip/quazipfile.h"

#include <QtCore>

class Zip
{
public:
    Zip();
    static bool extract(const QString &filePath, const QString &extDirPath, const QString &singleFileName = QString(""));
    static bool archive(const QString &filePath, const QDir &dir, const QString &comment = QString(""));
    static bool recurseAddDir(const QDir &dir, QStringList *sl);
    static bool archiveFile(const QString &FileArchPath, const QString &FileSourcePath, const bool &AddMode = true, const QString &comment = QString(""));
};

#endif // ZIP_H
