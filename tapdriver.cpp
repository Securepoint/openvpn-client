#include "tapdriver.h"
#include "message.h"

TapDriver *TapDriver::mInst = NULL;

TapDriver *TapDriver::instance()
{
    if (!mInst) {
        mInst = new TapDriver ();
    }
    return mInst;
}

TapDriver::TapDriver()
    : tapDriverAvailable (false),
      tapDriverInstalledSuccess (false),
      tapDriverRemovedSuccess (false)
{
}

bool TapDriver::isTapDriverInstalled()
{
    this->tapDriverAvailable = false;
    this->checkTapDriver();

    return this->tapDriverAvailable;
}

void TapDriver::checkTapDriver()
{

    QString drvInstallApp (QCoreApplication::applicationDirPath() + QLatin1String("/app/bin/tapinstall.exe"));

    if (Check64::isRunning64Bit()) {
        drvInstallApp = QCoreApplication::applicationDirPath() + QLatin1String("/app/bin/tapinstall64.exe");
    }

    QStringList argIsDrvInstalled;
    argIsDrvInstalled << QLatin1String ("hwids");
    argIsDrvInstalled << QLatin1String ("tap0901");

    QObject::connect (&this->drvProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readDriverData()));
    QObject::connect (&this->drvProc, SIGNAL(readyReadStandardError()), this, SLOT(readDriverData()));

    this->drvProc.start(drvInstallApp, argIsDrvInstalled);

    if(!this->drvProc.waitForFinished()) {
        Message::error(QObject::tr("TAP driver check process failed!"));
        QApplication::exit(1);
        return;
    }
}

bool TapDriver::installTapDriver()
{
    //Treiber installieren
    this->tapDriverInstalledSuccess = false;

    QString drvInstallApp (QCoreApplication::applicationDirPath() + QLatin1String("/app/bin/tapinstall.exe"));
    QString drvPath (QCoreApplication::applicationDirPath() + QLatin1String("/app/bin/driver/OemWin2k.inf"));

    if (Check64::isRunning64Bit()) {
        drvInstallApp = QCoreApplication::applicationDirPath() + QLatin1String("/app/bin/tapinstall64.exe");
        drvPath = QCoreApplication::applicationDirPath() + QLatin1String("/app/bin/driver/64bit/OemWin2k.inf");
    }

    QStringList argDrvInstall;
    argDrvInstall << QLatin1String ("install");
    argDrvInstall << drvPath;
    argDrvInstall << QLatin1String ("tap0901");

    QObject::connect (&this->drvInstallProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readDriverData()));
    QObject::connect (&this->drvInstallProc, SIGNAL(readyReadStandardError()), this, SLOT(readDriverData()));

    this->drvInstallProc.start(drvInstallApp, argDrvInstall);


    if(!this->drvInstallProc.waitForFinished()) {
        Message::error(QObject::tr("TAP driver install process failed!"));
        QApplication::exit(1);
        return false;
    }

    return this->tapDriverInstalledSuccess;
}

bool TapDriver::removeTapDriver()
{

    if (!this->isTapDriverInstalled()) {
        return true;
    }

    this->tapDriverRemovedSuccess = false;

    QString drvInstallApp (QCoreApplication::applicationDirPath() + QLatin1String("/app/bin/tapinstall.exe"));

    if (Check64::isRunning64Bit()) {
        drvInstallApp = QCoreApplication::applicationDirPath() + QLatin1String("/app/bin/tapinstall64.exe");
    }

    QStringList argDrvRemove;
    argDrvRemove << QLatin1String ("remove");
    argDrvRemove << QLatin1String ("tap0901");

    QObject::connect (&this->drvRemoveProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readDriverData()));
    QObject::connect (&this->drvRemoveProc, SIGNAL(readyReadStandardError()), this, SLOT(readDriverData()));

    this->drvRemoveProc.start(drvInstallApp, argDrvRemove);


    if(!this->drvRemoveProc.waitForFinished()) {
        Message::error(QObject::tr("TAP driver remove process failed!"));
        QApplication::exit(1);
        return false;
    }

    return this->tapDriverRemovedSuccess;
}

void TapDriver::readDriverData()
{

    QProcess *signalProcess = qobject_cast<QProcess*> (sender());

    QByteArray line;
    line = signalProcess->readAllStandardError();
    if (line.isEmpty()) {
        line = signalProcess->readAllStandardOutput();
    }

    // Daten da?
    if (!line.isEmpty()) {
        QString lineConvert (line);
        if (lineConvert.contains(QLatin1String("TAP-Win32"), Qt::CaseInsensitive)) {
            this->tapDriverAvailable = true;
        } else if (lineConvert.contains(QLatin1String("Drivers installed successfully."), Qt::CaseInsensitive)) {
            this->tapDriverInstalledSuccess = true;
        } else if (lineConvert.contains(QLatin1String("device(s) were removed."), Qt::CaseInsensitive)) {
            this->tapDriverRemovedSuccess = true;
        }
    }
}
