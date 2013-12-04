#include "treebutton.h"
#include "preferences.h"

#include "message.h"
#include "network/srvcli.h"
#include "servicelogdata.h"

TreeButton::TreeButton()
    : parentItem (0),
      timer (0),
      state (-1)
{
    // Der Timer für das aktualisieren der Verbindungszeit
    //this->timer = new QTimer (this);
    this->timer.setInterval(1000);
    QObject::connect (&this->timer, SIGNAL(timeout ()), this, SLOT(refreshConnectedSince()));

    // Den Click binden, war früher in der Preferencces und damit vollkommen fehl am Platze
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(clickMe()));
}

void TreeButton::setParentItem(QTreeWidgetItem *item)
{
    this->parentItem = item;
}

QTreeWidgetItem *TreeButton::getParentItem()
{    
    return this->parentItem;
}

TreeConItem *TreeButton::getParentItemCast()
{
    return dynamic_cast<TreeConItem*>(this->getParentItem());
}

void TreeButton::setReadyToConnect()
{
    this->setEnabled(true);
    this->setText(QObject::tr("Connect"));
    this->getParentItem()->setText(4, QLatin1String ("0"));
    this->state = 0;
    this->getParentItem()->setText(2, QObject::tr("Ready to connect."));
    this->getParentItem()->setToolTip(2,  QLatin1String(""));
    this->getParentItemCast()->getOpenVPN()->setIsError(false);
    this->getParentItemCast()->getOpenVPN()->setErrorString(QLatin1String(""));
    QColor bg ("#FFFFFF"); // weiss
    this->getParentItem()->setBackgroundColor(1, bg);
    this->getParentItem()->setBackgroundColor(2, bg);
    this->getParentItem()->setBackgroundColor(3, bg);
    OpenVpn *obj = this->getParentItemCast()->getOpenVPN();
    this->getParentItem()->setText(1, obj->getConfigName() + obj->getAdvName());

    this->timer.stop();
}

void TreeButton::setConnecting()
{    
    this->setText(QObject::tr("Cancel"));    
    this->setEnabled(true);
    this->getParentItem()->setText(4, QLatin1String ("1"));
    this->state = 1;
    this->getParentItem()->setText(2, QObject::tr("Connecting ..."));
    this->getParentItem()->setToolTip(2,  QObject::tr("Connecting ..."));
    QColor bg ("#FE9A2E"); // Orange    
    this->getParentItem()->setBackgroundColor(1, bg);
    this->getParentItem()->setBackgroundColor(2, bg);
    this->getParentItem()->setBackgroundColor(3, bg);
    this->timer.stop();
}

void TreeButton::setConnected(QString ip)
{    
    this->setText(QObject::tr("Disconnect"));
    this->getParentItem()->setText(4, QLatin1String ("2"));
    this->state = 2;
    this->getParentItem()->setText(2, QObject::tr("Connection established.") + QLatin1String("\n") + QObject::tr("IP:") + QLatin1String (" ") + ip);
    this->getParentItem()->setToolTip(2, QObject::tr("Connection established.") + QLatin1String("\n") + QObject::tr("IP:") + QLatin1String (" ") + ip);
    QColor bg ("#01DF01"); // grün
    this->getParentItem()->setBackgroundColor(1, bg);
    this->getParentItem()->setBackgroundColor(2, bg);
    this->getParentItem()->setBackgroundColor(3, bg);

    this->timer.start();
}

int TreeButton::getState() const
{
    return this->state;
}

void TreeButton::setError(QString error)
{
    this->setText(QObject::tr("Connect"));
    this->getParentItem()->setText(4, QLatin1String ("4"));
    this->state= 4;
    bool makeReconnect (false);
    QColor bg("#FF0000"); // rot

    if (error == QLatin1String("1025")) {
        if (!Settings::getInstance()->getIsAutoReconnect()) {
            goto endjump;
        } else {
            error = QObject::tr("Process unexpected closed! Reconnect!");
            makeReconnect = true;
        }
    }
    this->getParentItemCast()->getOpenVPN()->setIsError(true);
    this->getParentItemCast()->getOpenVPN()->setErrorString(error);
    this->getParentItem()->setText(2, QObject::tr("An error has occurred.") + QLatin1String ("\n") + error);
    this->getParentItem()->setToolTip(2, QObject::tr("An error has occurred.") + QLatin1String ("\n") + error);
    this->getParentItem()->setBackgroundColor(1, bg);
    this->getParentItem()->setBackgroundColor(2, bg);
    this->getParentItem()->setBackgroundColor(3, bg);

    if (makeReconnect) {
        QTimer::singleShot(500, this, SLOT(doReconnect()));
    }

    // Sprungmarke für Fehler 1025 und kein Reconnect
endjump: {}

    this->timer.stop();
}

void TreeButton::receivedError(QString errMessage)
{
    this->setError(errMessage);    
}

void TreeButton::receivedConnctionIsStable(QString ip)
{
    this->setConnected(ip);
}

void TreeButton::receivedDisconnect()
{
    this->setReadyToConnect();    
    Preferences::instance()->setIcon();
}

void TreeButton::doReconnect()
{    
    this->setConnecting();
    this->getParentItemCast()->getOpenVPN()->openConnect();
}

void TreeButton::receivedConnecting()
{     
    this->setConnecting();
    Preferences::instance()->refreshDialog();
}

void TreeButton::refreshConnectedSince()
{
    this->timer.stop();

    if (Preferences::instance()->isVisible()) {
        QDate cDate (this->getParentItemCast()->getOpenVPN()->getConnectedSinceDate());
        QTime cTime (this->getParentItemCast()->getOpenVPN()->getConnectedSinceTime());

        // Nun die Zet ermitteln
        int diffDays (cDate.daysTo(QDate::currentDate()));
        int diffSeconds (cTime.secsTo(QTime::currentTime()));

        int hours (0);
        int minutes (0);
        int seconds (0);
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
        this->getParentItem()->setText(1, this->getParentItemCast()->getOpenVPN()->getConfigName()
                                          + QLatin1String ("\n") + (hours < 10 ? "0" : "") + QString::number(hours)
                                          + QLatin1String (":") + (minutes < 10 ? "0" : "") + QString::number(minutes)
                                          + QLatin1String (":") + (seconds < 10 ? "0" : "") + QString::number(seconds));
    }

    this->timer.start();
}

void TreeButton::clickMe()
{
    switch (this->state) {
        case 1:
        case 2:
            if (this->state == 2) {
                if (!Message::confirm(QObject::tr("Do you want to disconnect the connection?"), QObject::tr("Disconnect"))) {
                    return;
                }
            }
            // Es soll ein Disconnect durchgeführt werden
            this->setEnabled(false);
            if (Settings::getInstance()->getIsRunAsSevice()) {
                // Script vor dem Disconnect ausführen
                this->getParentItemCast()->getOpenVPN()->runScript(QLatin1String("BD"));
                // Nun den Disconnect senden
                SrvCLI::instance()->send(QLatin1String("Close"), QString::number(this->getParentItemCast()->getOpenVPN()->id()));
            } else {                
                this->getParentItemCast()->getOpenVPN()->disconnectVpn();                
            }
            break;
        default:
            if (Settings::getInstance()->getIsRunAsSevice()) {
                // Per SrvCli senden
                // Das Kommando zusammensetzen
                // 0: ID; 1: Config Path; 2: Interact

                this->setConnecting();
                this->getParentItemCast()->getOpenVPN()->setIsConnecting(true);
                Preferences::instance()->setIcon();

                // Script vor dem Connect ausführen
                this->getParentItemCast()->getOpenVPN()->runScript(QLatin1String("BC"));
                // Log löschen
                ServiceLogData::instance()->clearId(this->getParentItemCast()->getOpenVPN()->id());
                // Verbinden senden
                SrvCLI::instance()->send(QLatin1String("Open"), QString::number(this->getParentItemCast()->getOpenVPN()->id())
                                                                + QLatin1String(";")
                                                                + this->getParentItemCast()->getOpenVPN()->getConfigFullPath()
                                                                + QLatin1String (";")
                                                                + (Settings::getInstance()->getUseNoInteract() ? QLatin1String("0") : QLatin1String("1"))
                                                                + QLatin1String (";")
                                                                + this->getParentItemCast()->getOpenVPN()->makeProxyString().join(" "));
            } else {
                this->getParentItemCast()->getOpenVPN()->openConnect();
            }            
    }
}
