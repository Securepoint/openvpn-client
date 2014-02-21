#include "tapdriver.h"
#include "debug.h"

TapDriver::TapDriver()
    : tapDriverAvailable (false),
      tapDriverInstalledSuccess (false),
      tapDriverRemovedSuccess (false)
{
}

bool TapDriver::isTapDriverInstalled() {
    this->tapDriverAvailable = true;
    this->checkTapDriver();
    return this->tapDriverAvailable;
}

void TapDriver::checkTapDriver() {
    //
    // Tapinstall.exe starten zum auslesen der verfügbaren TAP-Devices
    //

    QString drvInstallApp (QCoreApplication::applicationDirPath() + QLatin1String("/bin/tapinstall.exe"));

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

    // Pfade für die Exe und die Treiber inf
    QString drvInstallApp (QCoreApplication::applicationDirPath() + QLatin1String("/bin/tapinstall.exe"));
    QString drvPath (QCoreApplication::applicationDirPath() + QLatin1String("/bin/driver/OemWin2k.inf"));

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

    QString drvInstallApp (QCoreApplication::applicationDirPath() + QLatin1String("/bin/tapinstall.exe"));

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
        if (lineConvert.contains(QLatin1String("No matching devices found."), Qt::CaseInsensitive)) {
            this->tapDriverAvailable = false;
        } else if (lineConvert.contains(QLatin1String("Drivers installed successfully."), Qt::CaseInsensitive)) {
            this->tapDriverInstalledSuccess = true;
        } else if (lineConvert.contains(QLatin1String("device(s) were removed."), Qt::CaseInsensitive)) {
            this->tapDriverRemovedSuccess = true;
        }
    }
}
