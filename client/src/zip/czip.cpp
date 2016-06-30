#include "czip.h"

#undef UNICODE
//#include <WinSock2.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "zip.h"
#include "unzip.h"

bool CZip::extract(const QString & filePath, const QString & extDirPath, const QString & singleFileName, const QString &password)
{
    HZIP hz = OpenZip(filePath.toStdString().c_str(), password.isEmpty() ? 0 : password.toStdString().c_str());

    if(!hz) {
        return false;
    }

    SetUnzipBaseDir(hz,extDirPath.toStdString().c_str());

    if (!singleFileName.isEmpty()) {
        ZIPENTRY ze;
        int i; 
        FindZipItem(hz, singleFileName.toStdString().c_str(), true, &i, &ze);
        UnzipItem(hz, i, ze.name);
        CloseZip(hz);

        return true;
    }

    for (int i = 0; ; i++) {
        ZIPENTRY ze;
        ZRESULT zr = GetZipItem(hz, i, &ze); 

        if (zr != ZR_OK) {

            if(zr == ZR_FAILED || zr == ZR_NOTFOUND) {
                CloseZip(hz);
                return false;
            }

            break; // no more
        }
        UnzipItem(hz, i, ze.name);
    }

    CloseZip(hz);

    return true;
}

#include <functional>

bool RecurseDirectory(HZIP hz, QString sDir, QString prevDir = "") 
{
       QDir dir(sDir);
       QFileInfoList list = dir.entryInfoList();
       for (int iList = 0; iList < list.count(); iList++) {
           QFileInfo info = list[iList];

           QString sFilePath = info.filePath();
           if (info.isDir()) {
               // recursive
               if (info.fileName()!=".." && info.fileName()!=".")
               {
                   return RecurseDirectory(hz, sFilePath);
               }
           }
           else
           {
               // Do something with the file here
               
               if(ZipAdd(hz, (/*sDir.right(sDir.size() - sDir.lastIndexOf("/") -1) + "/" + */info.fileName()).toLatin1().data(), info.absoluteFilePath().toLatin1().data()) != ZR_OK)
                   return false;
           }
       }

       return true;
}

bool CZip::archive(const QString &filePath, const QDir &dir, const QString &comment)
{
   Q_UNUSED(comment)

   HZIP hz = CreateZip(filePath.toLatin1().data(), 0); 
   if (hz == 0) {
       qDebug() << "* Failed to create folders.zip";
   }

   ZRESULT zr;

   if(!RecurseDirectory(hz, dir.absolutePath())) {
       zr = CloseZip(hz);
       return false;
   }
   zr = CloseZip(hz);

   return true;
 }

bool CZip::recurseAddDir(const QDir &dir, QStringList *sl)
{
    Q_UNUSED(dir)
    Q_UNUSED(sl)
    //
    return true;
}

bool CZip::archiveFile(const QString &FileArchPath, const QString &FileSourcePath, const bool &AddMode , const QString &comment)
{
    Q_UNUSED(FileArchPath)
    Q_UNUSED(FileSourcePath)
    Q_UNUSED(AddMode)
    Q_UNUSED(comment)

    return true;
}
