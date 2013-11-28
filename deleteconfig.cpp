#include "deleteconfig.h"
#include "ui_deleteconfig.h"
#include "preferences.h"

#include "message.h"

DeleteConfig *DeleteConfig::mInst = NULL;

DeleteConfig *DeleteConfig::getInstance() {
    if (!mInst)
        mInst = new DeleteConfig ();
    return mInst;
}

DeleteConfig::DeleteConfig() :
    QDialog(),
    m_ui(new Ui::DeleteConfig)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
    this->obj = NULL;
    this->explorerPath = "";
}

void DeleteConfig::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DeleteConfig::showEvent(QShowEvent *e) {    
    m_ui->rbAll->setChecked(true);
    // Mittig ausrichten
    int winW = this->width();
    int winH = this->height();

    int left (0);
    int top (0);
    if (Preferences::instance()->isVisible()) {
        // Wenn das Hauptfenster offen ist mittig über diesem plazieren
        left = Preferences::instance()->geometry().x();
        top = Preferences::instance()->geometry().y();
        left = left + (Preferences::instance()->geometry().width() - winW) / 2;
        //
        top  = top + (Preferences::instance()->geometry().height() - winH) / 2;
    } else {
        // Desktop auswerten
        top = qApp->desktop()->height();
        left = qApp->desktop()->width();
        // Die Breite bei virtuellen Desktops vierteln
        if (left > 2000 && qApp->desktop()->isVirtualDesktop()) {
            left /= 4;
        } else {
            // Normaler Desktop
            left = (left - winH) / 2;
        }
        // Height
        if (top > 2000 && qApp->desktop()->isVirtualDesktop()) {
            top /= 4;
        } else {
            top = (top - winH) / 2;
        }
    }
    // Nun die neuen setzen
    this->setGeometry(left, top, winW, winH);
    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
}

void DeleteConfig::on_cmdCancel_clicked()
{
    this->close();
}

void DeleteConfig::on_cmdDelete_clicked()
{    
    // Eintrag in der Datenbank entfernen
    this->removeDatabaseEntry();
    // Sollen auch die Dateien gelöscht werden?
    if (m_ui->rbAll->isChecked()) {
        this->removeFiles();
    }

    // Daten aktualisieren
    Preferences::instance()->refreshConfigList();
    Preferences::instance()->refreshDialog();
    Preferences::instance()->setConnectionStatus();
    Preferences::instance()->setIcon();
    // Fertig und schliessen
    this->close();
}

void DeleteConfig::openExplorer() {

    QDesktopServices::openUrl(QUrl(this->explorerPath, QUrl::TolerantMode));
}

void DeleteConfig::setOpenVpnObject(OpenVpn *obj) {
    this->obj = obj;
}

QString DeleteConfig::getCAFromConfig() {
    QString retVal ("");
    QFile conf (obj->getConfigPath());
    if (conf.open(QIODevice::ReadOnly)) {
        // Nach ca suchen
        QTextStream in (&conf);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed().left(2).toLower() == "ca") {
                // ca gefunden
                QStringList keyvalList = line.split(" ");
                if (keyvalList.size() == 2) {
                    QString val = keyvalList[1];
                    retVal = val.replace("\"","");
                    break;
                }
            }
        }
        conf.close();
    }
    return retVal;
}

QString DeleteConfig::getCertFromConfig() {
    QString retVal = "";
    QFile conf (obj->getConfigPath());
    if (conf.open(QIODevice::ReadOnly)) {
        // Nach ca suchen
        QTextStream in (&conf);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed().left(4).toLower() == "cert") {
                // ca gefunden
                QStringList keyvalList = line.split(" ");
                if (keyvalList.size() == 2) {
                    QString val = keyvalList[1];
                    retVal = val.replace("\"","");
                    break;
                }
            }
        }
        conf.close();
    }
    return retVal;
}

QString DeleteConfig::getP12FromConfig()
{
    QString retVal = "";
    QFile conf (obj->getConfigPath());
    if (conf.open(QIODevice::ReadOnly)) {
        // Nach ca suchen
        QTextStream in (&conf);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed().left(6).toLower() == "pkcs12") {
                // ca gefunden
                QStringList keyvalList = line.split(" ");
                if (keyvalList.size() == 2) {
                    QString val = keyvalList[1];
                    retVal = val.replace("\"","");
                    break;
                }
            }
        }
        conf.close();
    }
    return retVal;
}

QString DeleteConfig::getKeyFromConfig()
{
    QString retVal = "";
    QFile conf (obj->getConfigPath());
    if (conf.open(QIODevice::ReadOnly)) {
        // Nach ca suchen
        QTextStream in (&conf);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.trimmed().left(3).toLower() == "key") {
                // ca gefunden
                QStringList keyvalList = line.split(" ");
                if (keyvalList.size() == 2) {
                    QString val = keyvalList[1];
                    retVal = val.replace("\"","");
                    break;
                }
            }
        }
        conf.close();
    }
    return retVal;
}

OpenVpn *DeleteConfig::getOpenVpnObject()
{
    return this->obj;
}

void DeleteConfig::removeDatabaseEntry()
{
    //
    // Remove the config entry in the database
    //

    Preferences::instance()->removeConfigInDatabase(this->obj->id());
}

void DeleteConfig::removeFiles()
{
    this->explorerPath = "";
    bool fError = false;
    QString errMes;

    QString ca = this->getCAFromConfig();
    if (!ca.isEmpty()) {
        QString caPath;
        if (ca.indexOf("/") != -1) {
            caPath = ca;
        } else {
            caPath = this->obj->getConfigDirectory() + "/" + ca;
        }

        QFile caFile(caPath);
        if (caFile.exists()) {
            if (!caFile.remove()) {
                fError = true;
                errMes =  caFile.errorString();
            }
        }
    }

    if (!fError) {
        QString cert = this->getCertFromConfig();
        if (!cert.isEmpty()) {
            QString certPath;
            if (cert.indexOf("/") != -1) {
                certPath = cert;
            } else {
                certPath = this->obj->getConfigDirectory() + "/" + cert;
            }

            QFile certFile(certPath);
            if (certFile.exists()) {
                if (!certFile.remove()) {
                    fError = true;
                    errMes =  certFile.errorString();
                }
            }
        }
    }

    if (!fError) {
        QString key = this->getP12FromConfig();
        if (!key.isEmpty()) {
            QString keyPath;
            if (key.indexOf("/") != -1) {
                keyPath = key;
            } else {
                keyPath = this->obj->getConfigDirectory() + "/" + key;
            }

            QFile keyFile(keyPath);
            if (keyFile.exists()) {
                if (!keyFile.remove()) {
                    fError = true;
                    errMes =  keyFile.errorString();
                }
            }
        }
    }

    if (!fError) {
        QString key = this->getKeyFromConfig();
        if (!key.isEmpty()) {
            QString keyPath;
            if (key.indexOf("/") != -1) {
                keyPath = key;
            } else {
                keyPath = this->obj->getConfigDirectory() + "/" + key;
            }

            QFile keyFile(keyPath);
            if (keyFile.exists()) {
                if (!keyFile.remove()) {
                    fError = true;
                    errMes =  keyFile.errorString();
                }
            }
        }
    }

    if (!fError) {
        QFile configFile(this->obj->getConfigPath());
        if (configFile.exists()) {
            if (!configFile.remove()) {
                fError = true;
                errMes = configFile.errorString();
            }
        }
    }

    if (!fError) {
        QFile scriptFile(this->obj->getConfigDirectory() + "/scripts.conf");
        if (scriptFile.exists()) {
            if (!scriptFile.remove()) {
                fError = true;
                errMes = scriptFile.errorString();
            }
        }
    }

    if (!fError) {
        // Dir löschen wenn leer
        QDir delDir (this->obj->getConfigDirectory());
        if (!delDir.rmdir(this->obj->getConfigDirectory())){
            fError = true;
            errMes = QString(tr("Directory is not empty! Please delete it by hand!"));
        }
    }

    if (fError) {
        if (Message::confirm(errMes + QLatin1String ("\n") + QObject::tr("Do you want to open the explorer to check the files?\nOtherwise maybe malfunctions can be occurred."))) {
            this->explorerPath = QLatin1String("file:///") + this->obj->getConfigDirectory();
            if (!this->explorerPath.isEmpty()) {
                QTimer::singleShot(300, this, SLOT(openExplorer()));
            }
        }
    }
}
