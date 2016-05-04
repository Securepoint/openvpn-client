#include "tapdriver.h"
#include "message.h"

#include <QtWidgets/QApplication>

TapDriver *TapDriver::mInst = NULL;

extern bool g_bPortable;

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
      tapDriverRemovedSuccess (false),
      tapCount(0)
{
        auto Isx64 = []() -> bool {
        typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

        LPFN_ISWOW64PROCESS fnIsWow64Process;

        BOOL bIsWow64 = FALSE;

        //IsWow64Process is not available on all supported versions of Windows.
        //Use GetModuleHandle to get a handle to the DLL that contains the function
        //and GetProcAddress to get a pointer to the function if available.

        fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
            GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

        if(NULL != fnIsWow64Process) {
            if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)) {
                // handle error
            }
        }
        return (bIsWow64 == TRUE);
    };

    arch  = "x32";
    if (Isx64()) {
        arch = QLatin1String("x64");
    }
}

bool TapDriver::isTapDriverInstalled()
{
    this->tapDriverAvailable = false;
    this->checkTapDriver();

    return this->tapDriverAvailable;
}

void TapDriver::checkTapDriver()
{

    QString drvInstallApp (this->getTapPath());

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

QString TapDriver::getTapPath()
{

    QString binDir = QLatin1String("/bin") + (g_bPortable ? QString("/%1").arg(arch) : "");

    QString drvInstallApp (QCoreApplication::applicationDirPath() + binDir + QLatin1String("/tapinstall.exe"));

    return drvInstallApp;
}

bool TapDriver::installTapDriver()
{
    //Treiber installieren
    this->tapDriverInstalledSuccess = false;

    QString drvInstallApp (this->getTapPath());
    QString drvPath (QCoreApplication::applicationDirPath() + QString("/bin/driver/OemWin2k.inf"));


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

    QString drvInstallApp (this->getTapPath());

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
        if (lineConvert.contains(QLatin1String("TAP-Windows"), Qt::CaseInsensitive)) {
            this->tapDriverAvailable = true;
        } else if (lineConvert.contains(QLatin1String("Drivers installed successfully."), Qt::CaseInsensitive)) {
            this->tapDriverInstalledSuccess = true;
        } else if (lineConvert.contains(QLatin1String("device(s) were removed."), Qt::CaseInsensitive)) {
            this->tapDriverRemovedSuccess = true;
        }
    }
}

int TapDriver::deviceCount()
{
    //
    // Returns the tap device count
    //
    QString drvInstallApp (this->getTapPath());

    QStringList argIsDrvInstalled;
    argIsDrvInstalled << QLatin1String ("hwids");
    argIsDrvInstalled << QLatin1String ("tap0901");

    QObject::connect (&this->drvCountProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readTapCount()));
    QObject::connect (&this->drvCountProc, SIGNAL(readyReadStandardError()), this, SLOT(readTapCount()));

    this->wait = true;
    this->drvCountProc.start(drvInstallApp, argIsDrvInstalled);

    QDateTime startDate (QDateTime::currentDateTime());
    while (this->wait) {
        if (qApp->hasPendingEvents()) {
            qApp->processEvents();
        }

        if (startDate.msecsTo(QDateTime::currentDateTime()) > 1500) {
            this->tapCount = -1;
            break;
        }
    }

    return tapCount;

}

void TapDriver::readTapCount()
{
    //
    // Read the application response to determine the tap device count
    //

    QProcess *signalProcess = qobject_cast<QProcess*> (sender());

    QByteArray line;
    line = signalProcess->readAllStandardError();
    if (line.isEmpty()) {
        line = signalProcess->readAllStandardOutput();
    }

    // Daten da?
    if (!line.isEmpty()) {
        QString lineConvert (line);
        if (lineConvert.contains(QLatin1String("matching device(s) found."), Qt::CaseInsensitive)) {
            // Get tap count
            this->tapCount = 0;
            QStringList sl (lineConvert.split("\r\n"));
            foreach (QString str, sl) {
                if (str.contains(QLatin1String("matching device(s) found."))) {
                    str = str.left(str.indexOf(QLatin1String("matching device(s) found.")));
                    str = str.trimmed();
                    bool ok;
                    int val (str.toInt(&ok));
                    if (ok) {
                        this->tapCount = val;
                    }
                }
            }

            this->wait = false;
        } else if (lineConvert.startsWith(QLatin1String("No matching devices found."))) {
            this->tapCount = 0;
            this->wait = false;
        }
    }
}


