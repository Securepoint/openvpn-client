#include "tapdriver.h"
#include "debug.h"

#include <Windows.h>

extern bool g_bPortable;

TapDriver::TapDriver()
    : tapDriverAvailable (false),
      tapDriverInstalledSuccess (false),
      tapDriverRemovedSuccess (false)
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

bool TapDriver::isTapDriverInstalled() {
    this->tapDriverAvailable = false;
    this->checkTapDriver();
    return this->tapDriverAvailable;
}

void TapDriver::checkTapDriver() {
    //
    // Tapinstall.exe starten zum auslesen der verfügbaren TAP-Devices
    //

     QString binDir = QLatin1String("/bin") + (g_bPortable ? QString("/%1").arg(arch) : "");

    QString drvInstallApp (QCoreApplication::applicationDirPath() + binDir + QLatin1String("/tapinstall.exe"));

    // Argumentliste bauen
    QStringList argIsDrvInstalled;
    argIsDrvInstalled << QLatin1String ("hwids");
    argIsDrvInstalled << QLatin1String ("tap0901");

    // Signale binden zum lesen der Prozessausgabe
    QObject::connect (&this->drvProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readDriverData()));
    QObject::connect (&this->drvProc, SIGNAL(readyReadStandardError()), this, SLOT(readDriverData()));

    // Tapinstall starten
    this->drvProc.start(drvInstallApp, argIsDrvInstalled);

    // Wenn nach 30 Sekunden der Prozess nicht gestartet und beendet wurde, ist ein Fehler aufgetreten
    if(!this->drvProc.waitForFinished()) {
        Debug::error(QLatin1String("TAP driver check process failed!"));
        return;
    }
}

bool TapDriver::installTapDriver() {
    //
    // Einen Tap Device installieren mit Hilfe der tapinstall.exe
    //

    this->tapDriverInstalledSuccess = false;

    QString binDir = QLatin1String("/bin") + (g_bPortable ? QString("/%1").arg(arch) : "");

    // Pfade für die Exe und die Treiber inf
    QString drvInstallApp (QCoreApplication::applicationDirPath() + binDir + QLatin1String("/tapinstall.exe"));
    QString drvPath (QCoreApplication::applicationDirPath() + binDir + QLatin1String("/driver/OemVista.inf"));

    // Argumente bauen
    QStringList argDrvInstall;
    argDrvInstall << QLatin1String ("install");
    argDrvInstall << drvPath;
    argDrvInstall << QLatin1String ("tap0901");


    // Die Signale binden
    QObject::connect (&this->drvInstallProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readDriverData()));
    QObject::connect (&this->drvInstallProc, SIGNAL(readyReadStandardError()), this, SLOT(readDriverData()));

    // Prozess starten
    this->drvInstallProc.start(drvInstallApp, argDrvInstall);

    // Warten das der Prozess beendet ist
    if(!this->drvInstallProc.waitForFinished()) {
        Debug::error(QLatin1String("TAP driver install process failed!"));
        return false;
    }

    return this->tapDriverInstalledSuccess;
}

bool TapDriver::removeTapDriver() {
    //
    // Entfernt alle Tap Divices
    //

    // Wenn keine Tap-Devices da sind brauchen wir auch keine entfernen
    if (!this->isTapDriverInstalled()) {
        Debug::log(QLatin1String("No tap devices available."));
        return true;
    }

    this->tapDriverRemovedSuccess = false;

    QString binDir = QLatin1String("/bin") + (g_bPortable ? QString("/%1").arg(arch) : "");

    QString drvInstallApp (QCoreApplication::applicationDirPath() + binDir + QLatin1String("/tapinstall.exe"));

    // Argumente
    QStringList argDrvRemove;
    argDrvRemove << QLatin1String ("remove");
    argDrvRemove << QLatin1String ("tap0901");

    // Signale binden
    QObject::connect (&this->drvRemoveProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readDriverData()));
    QObject::connect (&this->drvRemoveProc, SIGNAL(readyReadStandardError()), this, SLOT(readDriverData()));

    // Prozess starten
    drvRemoveProc.start(drvInstallApp, argDrvRemove);

    // Auf das Beenden des Prozesses warten
    if(!this->drvRemoveProc.waitForFinished(60000)) {
        Debug::error(QLatin1String("TAP driver remove process failed!"));
        return false;
    }

    return this->tapDriverRemovedSuccess;
}

void TapDriver::readDriverData() {
    //
    // Liest und wertet die Ausgabe der Prozesse aus und setzt die Statusvariablen
    //

    // Den Sender wieder in ein QProcess-Objekt wandeln
    QProcess *signalSender = qobject_cast<QProcess*>(sender());

    QByteArray line;
    line = signalSender->readAllStandardOutput();
    if (line.isEmpty())
        line = signalSender->readAllStandardError();
    if (!line.isEmpty()) {
        // QString statt QBA
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

    QString binDir = QLatin1String("/bin") + (g_bPortable ? QString("/%1").arg(arch) : "");

     QString drvInstallApp (QCoreApplication::applicationDirPath() + binDir + QLatin1String("/tapinstall.exe"));

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