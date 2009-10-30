#include "tapdriver.h"

TapDriver::TapDriver()
{
}

bool TapDriver::isTapDriverInstalled() {
    this->tapDriverAvailable = false;
    this->checkTapDriver();
    return this->tapDriverAvailable;
}

void TapDriver::checkTapDriver() {
    drvProc = new QProcess(this);
    QString drvInstallApp = QString("./app/bin/tapinstall.exe");
    QStringList argIsDrvInstalled;
    argIsDrvInstalled << QString ("hwids") << QString ("tap0901");
    drvProc->start(drvInstallApp, argIsDrvInstalled);
    connect (drvProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readDriverData()));
    connect (drvProc, SIGNAL(readyReadStandardError()), this, SLOT(readDriverData()));

    if(!drvProc->waitForFinished()) {
        QMessageBox::critical(0, QString("Securepoint VPN Client"), QString("TAP driver check process failed!"));
        QApplication::exit(1);
        return;
    }
}

bool TapDriver::installTapDriver() {
    //Treiber installieren
    this->tapDriverInstalledSuccess = false;
    drvInstallProc = new QProcess(this);
    QString drvInstallApp = QString("./app/bin/tapinstall.exe");
    QStringList argDrvInstall;
    argDrvInstall << QString ("install");
    argDrvInstall << QString ("./app/bin/driver/OemWin2k.inf");
    argDrvInstall << QString ("tap0901");
    drvInstallProc->start(drvInstallApp, argDrvInstall);
    connect (drvInstallProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readDriverInstallData()));
    connect (drvInstallProc, SIGNAL(readyReadStandardError()), this, SLOT(readDriverInstallData()));

    if(!drvInstallProc->waitForFinished()) {
        QMessageBox::critical(0, QString("Securepoint VPN Client"), QString("TAP driver install process failed!"));
        QApplication::exit(1);
        return false;
    }

    return this->tapDriverInstalledSuccess;
}

bool TapDriver::removeTapDriver() {
    this->tapDriverRemovedSuccess = false;
    drvRemoveProc = new QProcess(this);
    QString drvInstallApp = QString("./app/bin/tapinstall.exe");
    QStringList argDrvRemove;
    argDrvRemove << QString ("remove");
    argDrvRemove << QString ("tap0901");
    drvRemoveProc->start(drvInstallApp, argDrvRemove);
    connect (drvRemoveProc, SIGNAL(readyReadStandardOutput()), this, SLOT(readDriverRemoveData()));
    connect (drvRemoveProc, SIGNAL(readyReadStandardError()), this, SLOT(readDriverRemoveData()));

    if(!drvRemoveProc->waitForFinished()) {
        QMessageBox::critical(0, QString("Securepoint VPN Client"), QString("TAP driver remove process failed!"));
        QApplication::exit(1);
        return false;
    }
    return this->tapDriverRemovedSuccess;
}

void TapDriver::readDriverData() {
    QByteArray line;
    line = this->drvProc->readAllStandardError();
    if (line == "")
        line = this->drvProc->readAllStandardOutput();
    if (line != "") {
        QString lineConvert (line);
        if (lineConvert.contains("TAP-Win32", Qt::CaseInsensitive)) {
            this->tapDriverAvailable = true;
        } else if (lineConvert.contains("Drivers installed successfully.", Qt::CaseInsensitive)) {
            this->tapDriverInstalledSuccess = true;
        } else if (lineConvert.contains("1 device(s) were removed.", Qt::CaseInsensitive)) {
            this->tapDriverRemovedSuccess = true;
        }
    }
}

void TapDriver::readDriverInstallData() {
    QByteArray line;
    line = this->drvInstallProc->readAllStandardError();
    if (line == "")
        line = this->drvInstallProc->readAllStandardOutput();
    if (line != "") {
        QString lineConvert (line);
        if (lineConvert.contains("Drivers installed successfully.", Qt::CaseInsensitive)) {
            this->tapDriverInstalledSuccess = true;
        }
    }
}

void TapDriver::readDriverRemoveData() {
    QByteArray line;
    line = this->drvRemoveProc->readAllStandardError();
    if (line == "")
        line = this->drvRemoveProc->readAllStandardOutput();
    if (line != "") {
        QString lineConvert (line);
        if (lineConvert.contains("1 device(s) were removed.", Qt::CaseInsensitive)) {
            this->tapDriverRemovedSuccess = true;
        }
    }
}
