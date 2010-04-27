#include "mainwindowcontroll.h"
#include "preferences.h"
#include "openvpn.h"

MainWindowControll *MainWindowControll::mInst = NULL;

MainWindowControll * MainWindowControll::getInstance() {
    if (!mInst)
        mInst = new MainWindowControll ();
    return mInst;
}

MainWindowControll::MainWindowControll()
{
    this->mainWin = NULL;
}

bool MainWindowControll::isMainWindowOpen() {
    bool retVal = false;
    if (this->mainWin)
        retVal = this->mainWin->isVisible();
    return retVal;
}

void MainWindowControll::setMainWindow(Preferences *mainW) {
    this->mainWin = mainW;
}

void MainWindowControll::setTrayIcon(QIcon icon) {
    this->mainWin->trayIcon->setIcon(icon);
}

void MainWindowControll::showTrayMessage(QString message) {
    // Nur die Ballon Message anzeigen, wenn der Hauptdialog
    // geschlossen ist und im der CLI kein showNoBallon angegeben wurde.
    if (!this->isMainWindowOpen())
        if (!Settings::getInstance()->getIsShowNoBallonMessage())
            this->mainWin->trayIcon->showMessage("Securepoint SSL VPN", message, QSystemTrayIcon::Information, 6000);
}

void MainWindowControll::enableTreeButtons() {
    this->mainWin->enableTreeButtons();
}

void MainWindowControll::refreshDialog() {
    this->mainWin->refreshDialog();
}

QMenu *MainWindowControll::getSystray() {
    return this->mainWin->findChild<QMenu*>("SYSTRAYMENU");
}

void MainWindowControll::setConnectionStatus() {
    this->mainWin->setConnectionStatus();
}

void MainWindowControll::refreshConfigs() {
    this->mainWin->refreshConfigList();
}

void MainWindowControll::setConfigToTop(OpenVpn *sourceObj) {
    int rc = 0;
    foreach (OpenVpn *obj, Configs::getInstance()->getConfigsObjects()) {
        if (obj == sourceObj) {
            if (rc != 0) {
                Configs::getInstance()->swap(rc, 0);
                break;
            }
        }
        rc++;
    }
    this->refreshDialog();
}
