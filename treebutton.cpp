#include "treebutton.h"

TreeButton::TreeButton()
{
    // Pointer init
    this->parentItem = NULL;
    this->timer = new QTimer (this);
    connect (this->timer, SIGNAL(timeout ()), this, SLOT(refreshConnectedSince()));
}

void TreeButton::setParentItem(QTreeWidgetItem *item) {
    this->parentItem = item;
}

QTreeWidgetItem *TreeButton::getParentItem() {
    return this->parentItem;
}

TreeConItem *TreeButton::getParentItemCast() {
    return dynamic_cast<TreeConItem*>(this->getParentItem());
}

void TreeButton::setReadyToConnect() {
    this->setText(tr("Connect"));
    this->getParentItem()->setText(4, QString ("0"));
    this->getParentItem()->setText(2, QString (tr("Ready to connect.")));
    this->getParentItem()->setBackgroundColor(1, QColor("#ffffff")); // weiss
    this->getParentItem()->setBackgroundColor(2, QColor("#ffffff")); // weiss
    this->getParentItem()->setBackgroundColor(3, QColor("#ffffff")); // weiss
    this->timer->stop();
}

void TreeButton::setConnecting() {
    this->setText(tr("Cancel"));
    this->getParentItem()->setText(4, QString ("1"));
    this->getParentItem()->setText(2, QString (tr("Connecting ...")));
    this->getParentItem()->setBackgroundColor(1, QColor("#FE9A2E")); // orange
    this->getParentItem()->setBackgroundColor(2, QColor("#FE9A2E")); // orange
    this->getParentItem()->setBackgroundColor(3, QColor("#FE9A2E")); // orange
    this->timer->stop();
}

void TreeButton::setConnected(QString ip) {
    this->setText(tr("Disconnect"));
    this->getParentItem()->setText(4, QString ("2"));
    this->getParentItem()->setText(2, QString (tr("Connection established.")) + QString("\n") + QString (tr("IP:")) + QString (" ") + ip);
    this->getParentItem()->setBackgroundColor(1, QColor("#01DF01")); // grün
    this->getParentItem()->setBackgroundColor(2, QColor("#01DF01")); // grün
    this->getParentItem()->setBackgroundColor(3, QColor("#01DF01")); // grün    
    this->timer->start(1000);
}

void TreeButton::setError(QString error) {    
    this->setText(tr("Connect"));
    this->getParentItem()->setText(4, QString ("4"));
    bool makeReconnect = false;
    if (error == "1025") {
        if (!Settings::getInstance()->getIsAutoReconnect()) {
            goto endjump;
        } else {
            if (Settings::getInstance()->getIsShowNoUserAuth() && (!Settings::getInstance()->getIsStartWithPwd() || !Settings::getInstance()->getIsStartWithUser())) {
                error = tr("Process unexpected closed! Reconnect disabled, maybe a loop will be create!");
            } else {
                error = tr("Process unexpected closed! Reconnect!");
                makeReconnect = true;
            }
        }
    }
    this->getParentItem()->setText(2, QString (tr("An error has occurred.")) + QString ("\n") + error);
    this->getParentItem()->setBackgroundColor(1, QColor("#ff0000")); // rot
    this->getParentItem()->setBackgroundColor(2, QColor("#ff0000")); // rot
    this->getParentItem()->setBackgroundColor(3, QColor("#ff0000")); // rot
    if (makeReconnect)
        QTimer::singleShot(500, this, SLOT(doReconnect()));
    // Schmutziger Jump, mir fällt aber grad nichts anderes ein, um den bug zu beheben
    endjump: {}

    this->timer->stop();
}

void TreeButton::receivedError(QString errMessage) {
    this->setError(errMessage);
    MainWindowControll::getInstance()->enableTreeButtons();
}
void TreeButton::receivedConnctionIsStable(QString ip) {    
    this->setConnected(ip);
}

void TreeButton::receivedDisconnect() {
    this->setReadyToConnect();
    MainWindowControll::getInstance()->enableTreeButtons();
}

void TreeButton::doReconnect() {
    this->setConnecting();
    this->getParentItemCast()->getOpenVPN()->openConnect();
}

void TreeButton::receivedConnecting() {
    int rc = 0;
    foreach (OpenVpn *obj, Configs::getInstance()->getConfigsObjects()) {
        if (obj == this->getParentItemCast()->getOpenVPN()) {
            if (rc != 0) {
                Configs::getInstance()->swap(rc, 0);
                break;
            }
        }
        rc++;
    }
    this->setConnecting();
    MainWindowControll::getInstance()->enableTreeButtons();
    MainWindowControll::getInstance()->refreshDialog();
}

void TreeButton::refreshConnectedSince() {
    if (MainWindowControll::getInstance()->isMainWindowOpen()) {
        QDate cDate = this->getParentItemCast()->getOpenVPN()->getConnectedSinceDate();
        QTime cTime = this->getParentItemCast()->getOpenVPN()->getConnectedSinceTime();
        // Nun die Zet ermitteln
        int diffDays = cDate.daysTo(QDate::currentDate());
        int diffSeconds = cTime.secsTo(QTime::currentTime());

        int hours = 0;
        int minutes = 0;
        int seconds = 0;
        if (diffSeconds < 0) {
            // Bei einem negativen Ergebniss liegt die Zeit in der Zukunft,
            // es muss aber in der Vergangeheit liegen, es kann ein neuer Tag sein
            // Sekunden zurücksetzen
            diffSeconds = 0;
            this->getParentItemCast()->getOpenVPN()->setConnectedSinceTime(QTime::currentTime());
        }

        if (diffSeconds >= 3600) {
            // Stunden sind da
            // Stunden ermitteln
            hours = (int) diffSeconds / 3600;
            // Neuen Rest
            diffSeconds = diffSeconds - (3600 * hours);
            if (diffDays > 0) {
                // Tage auf Stunden rechnen
                hours = hours + diffDays * 24;
            }
        }
        // Minuten da?
        if (diffSeconds >= 60) {
            minutes = (int) diffSeconds / 60;
            // Neuen Rest ermitteln
            diffSeconds = diffSeconds - (minutes * 60);
        }

        // Sind noch Sekunden übrig?
        if (diffSeconds > 0) {
            seconds = diffSeconds;
        }
        // Alles ermittelt nun setzen
        this->getParentItem()->setText(1, this->getParentItemCast()->getOpenVPN()->configName + QString ("\n") + (hours < 10 ? "0" : "") + QString::number(hours) + QString (":") + (minutes < 10 ? "0" : "") + QString::number(minutes) + QString (":") + (seconds < 10 ? "0" : "") + QString::number(seconds));
    }
}
