#include "checksum.h"

#include <userinfo.h>
#include <servicelog.h>
#include <srvcli.h>
#include <servicelogdata.h>
#include <cstring>
#include <debug/debug.h>

extern bool g_bPortable;

Checksum* Checksum::thisInstance = nullptr;

Checksum* Checksum::instance() {
    if (!thisInstance) {
        thisInstance = new Checksum();
    }

    return thisInstance;
}

Checksum::Checksum()
{

}

void Checksum::createNewChecksum(const QString &content, qint32 configId, const QString &configPath)
{
    if (g_bPortable) {
        return;
    }

    QString configContent (QByteArray::fromBase64(content.toUtf8()));
    QString hashOfContent(QCryptographicHash::hash(configContent.toUtf8(), QCryptographicHash::Sha256).toHex());


    std::wstring params = QString(QString("-id %1 -user %2 -sid %3 -path %4 -sum %5%6")
                                  .arg(configId)
                                  .arg(UserInfo::instance()->currentLogonUser())
                                  .arg(UserInfo::instance()->currentLogonSID())
                                  .arg(configPath)
                                  .arg(hashOfContent)
                                  .arg((g_bPortable ? " -portable" : ""))
                                  ).toStdWString();

    SHELLEXECUTEINFOW ShExecInfo = { 0 };
    ShExecInfo.cbSize = sizeof(ShExecInfo);
    ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    ShExecInfo.lpFile = L"checksum.exe";
    ShExecInfo.lpParameters = params.c_str();
    ShExecInfo.lpDirectory = qApp->applicationDirPath().toStdWString().c_str();
    ShExecInfo.nShow = SW_SHOW;

    if (!ShellExecuteExW(&ShExecInfo)){
        // Crap
        ServiceLogData::instance()->append(configId, QString("Admin check failed, skipping write of checksum\n"));
        //
        return;
    }

    WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
    CloseHandle(ShExecInfo.hProcess);
}

void Checksum::createNewChecksumFromFile(qint32 configId, const QString &configPath)
{
    this->createNewChecksum(this->base64OfFileContent(configId, configPath), configId, configPath);
}

void Checksum::removeChecksum(qint32 configId)
{
    if (g_bPortable) {
        return;
    }

    // Now remove the checksum of the config
    QString commandArgss = QString ("%1;%2;%3;%4")
                            .arg(configId)
                            .arg(UserInfo::instance()->currentLogonUser())
                            .arg(UserInfo::instance()->currentLogonSID())
                            .arg((g_bPortable ? "1" : "0"));

    SrvCLI::instance()->send(QLatin1String("REMOVE_CHECKSUM"), commandArgss);
}

QString Checksum::base64OfContent(const QStringList &content)
{
    return this->normalizeContent(content.join("")).toBase64();
}

QString Checksum::base64OfContent(const QString &content)
{
    return this->normalizeContent(content).toBase64();
}

QByteArray Checksum::normalizeContent(QString content)
{
    QStringList lines = content.replace("\r\n", "\n").split("\n");
    //
    int linesCount = lines.size();
    QString normalizedContent;
    //
    for (int lineIndex = 0; lineIndex < linesCount; ++lineIndex) {
        QString line = lines.value(lineIndex);
        normalizedContent.append(line.trimmed() + "\n");
    }

    return normalizedContent.toUtf8();
}

QByteArray Checksum::base64OfFileContent(qint32 configId, const QString &path)
{
    QFile config(path);

    //
    if (!config.open(QIODevice::ReadOnly)) {
        // Crap
        ServiceLogData::instance()->append(configId, QString("Process configuration file not found\n"));
        //
        return nullptr;
    }

    // File is open read lines
    QTextStream in(&config);

    //
    QString content;
    //
    while (!in.atEnd()) {
        // Read line
        content += in.readLine() + (in.atEnd() ? "" : "\n");
    }

    //
    config.close();

    QByteArray data = this->normalizeContent(content);

    return data.toBase64();
}

