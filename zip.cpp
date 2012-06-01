#include "zip.h"

#include <QtGui>

Zip::Zip()
{
}

bool Zip::extract(const QString & filePath, const QString & extDirPath, const QString & singleFileName)
{
    QuaZip zip(filePath);

    if (!zip.open(QuaZip::mdUnzip)) {
        return false;
    }

    zip.setFileNameCodec("IBM866");

    QuaZipFileInfo info;

    QuaZipFile file(&zip);

    QFile out;
    QString name;
    char c;
    for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {

        if (!zip.getCurrentFileInfo(&info)) {
            return false;
        }

        if (!singleFileName.isEmpty())
            if (!info.name.contains(singleFileName))
                continue;

        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }

        name = QString("%1/%2").arg(extDirPath).arg(file.getActualFileName());

        if (file.getZipError() != UNZ_OK) {
            return false;
        }

        out.setFileName(name);

        // this will fail if "name" contains subdirectories, but we don't mind that
        out.open(QIODevice::WriteOnly);
        // Slow like hell (on GNU/Linux at least), but it is not my fault.
        // Not ZIP/UNZIP package's fault either.
        // The slowest thing here is out.putChar(c).
        while (file.getChar(&c)) out.putChar(c);

        out.close();

        if (file.getZipError() != UNZ_OK) {
            return false;
        }

        if (!file.atEnd()) {
            return false;
        }

        file.close();

        if (file.getZipError() != UNZ_OK) {
            return false;
        }
    }

    zip.close();

    if (zip.getZipError() != UNZ_OK) {
        return false;
    }

    return true;
}

bool Zip::archive(const QString &filePath, const QDir &dir, const QString &comment)
{
    QuaZip zip(filePath);

    if (!zip.open(QuaZip::mdCreate)) {
        return false;
    }

    if (!dir.exists()) {
        return false;
    }


    QFile inFile;
    QStringList sl;
    recurseAddDir(dir, &sl);

    QFileInfoList files;
    foreach (QString fn, sl) {
        files << QFileInfo(fn);
    }

    QuaZipFile outFile(&zip);

    char c;
    foreach(QFileInfo fileInfo, files) {

        if (!fileInfo.isFile()) {
            continue;
        }

        QString fileNameWithRelativePath (fileInfo.filePath().remove(0, dir.absolutePath().length() + 1));

        inFile.setFileName(fileInfo.filePath());
        //KeyProvider kp;
        if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileNameWithRelativePath, fileInfo.filePath())/*, kp.GetKey().toAscii()*/)) {
          return false;
        }

        if (!inFile.open(QIODevice::ReadOnly)) {
            return false;
        }

        while (inFile.getChar(&c) && outFile.putChar(c));

        if (outFile.getZipError() != UNZ_OK) {
            return false;
        }

        outFile.close();

        if (outFile.getZipError() != UNZ_OK) {
            return false;
        }

        inFile.close();
    }

    if (!comment.isEmpty()) {
        zip.setComment(comment);
    }

    zip.close();

    if (zip.getZipError() != 0) {
        return false;
    }

    return true;
 }

bool Zip::recurseAddDir(const QDir &dir, QStringList *sl)
{
    bool result (true);

    Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
        if (info.isDir()) {
            sl->append(info.absoluteFilePath());
            QDir tmpdir(info.absoluteFilePath());
            result = recurseAddDir(tmpdir, sl);
        } else {
            sl->append(info.absoluteFilePath());
        }

        if (!result) {
            return result;
        }
    }

    return result;
}

bool Zip::archiveFile(const QString &FileArchPath, const QString &FileSourcePath, const bool &AddMode , const QString &comment)
{
    // Keine Datei oder Pfad ohne Suffix werfen keinen Fehler
    if (FileSourcePath.isEmpty() || FileSourcePath.indexOf(".") == -1) {
        return true;
    }

    QuaZip::Mode mode;    

    QuaZip zip(FileArchPath);
    if (AddMode) {
        QFile src(FileArchPath);
        if (!src.exists()) {
            mode = QuaZip::mdCreate;
        } else {
            mode = QuaZip::mdAdd;
        }
    } else {
        QFile src(FileArchPath);
        if (src.exists()) {
            src.remove();
        }
        mode = QuaZip::mdCreate;
    }

    if (!zip.open(mode)) {        
        return false;
    }

    QFile inFile;
    QFileInfo  fileInfo(FileSourcePath);

    QuaZipFile outFile(&zip);
    char c;

    QString fileNameWithRelativePath = fileInfo.filePath().remove(0, fileInfo.filePath().length() - fileInfo.fileName().length());

    inFile.setFileName(fileInfo.filePath());
    if (!inFile.open(QIODevice::ReadOnly)) {        
        return false;
    }

    if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileNameWithRelativePath, fileInfo.filePath()))) {        
        return false;
    }

    while (inFile.getChar(&c) && outFile.putChar(c));

    if (outFile.getZipError() != UNZ_OK) {        
        return false;
    }

    outFile.close();

    if (outFile.getZipError() != UNZ_OK) {        
        return false;
    }

    inFile.close();

    if (!comment.isEmpty()) {
        zip.setComment(comment);
    }

    zip.close();

    if (zip.getZipError() != 0) {        
        return false;
    }

    return true;
}
