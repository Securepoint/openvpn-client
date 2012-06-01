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
    m_ui->cbCA->setChecked(true);
    m_ui->cbCert->setChecked(true);
    m_ui->cbConfigFile->setChecked(true);
    m_ui->cbDir->setChecked(true);
    m_ui->cbKey->setChecked(true);
    m_ui->cbUserdata->setChecked(true);
    m_ui->cbScript->setChecked(true);
    m_ui->cbRemovePkcs12->setChecked(true);
    // Mittig ausrichten
    int winW = this->width();
    int winH = this->height();

    int left = Preferences::instance()->geometry().x();
    left = left + (Preferences::instance()->geometry().width() - winW) / 2;

    // Nun die neuen setzen
    this->setGeometry(left, (qApp->desktop()->height() / 2) - (winH / 2), winW, winH);
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
    if (obj) {
        if (this->obj->getConfigFullPath() == Settings::getInstance()->getCommandConfigPath()) {
            Message::error(QObject::tr("Can't remove a start config. Please release it first."));
            this->close();
            return;
        }
    }

    this->explorerPath = "";
    bool fError = false;
    QString errMes;

    if (m_ui->cbCA->isChecked()) {
        QString ca = this->getCAFromConfig();
        if (!ca.isEmpty()) {
            QString caPath;
            if (ca.indexOf("/") != -1) {
                caPath = ca;
            } else {
                caPath = this->obj->getConfigPath() + "/" + ca;
            }

            QFile caFile(caPath);
            if (caFile.exists()) {
                if (!caFile.remove()) {
                    fError = true;
                    errMes =  caFile.errorString();
                }
            }
        }
    }

    if (!fError && m_ui->cbCert->isChecked()) {
        QString cert = this->getCertFromConfig();
        if (!cert.isEmpty()) {
            QString certPath;
            if (cert.indexOf("/") != -1) {
                certPath = cert;
            } else {
                certPath = this->obj->getConfigPath() + "/" + cert;
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

    if (!fError && m_ui->cbRemovePkcs12->isChecked()) {
        QString key = this->getP12FromConfig();
        if (!key.isEmpty()) {
            QString keyPath;
            if (key.indexOf("/") != -1) {
                keyPath = key;
            } else {
                keyPath = this->obj->getConfigPath() + "/" + key;
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

    if (!fError && m_ui->cbKey->isChecked()) {
        QString key = this->getKeyFromConfig();
        if (!key.isEmpty()) {
            QString keyPath;
            if (key.indexOf("/") != -1) {
                keyPath = key;
            } else {
                keyPath = this->obj->getConfigPath() + "/" + key;
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

    if (!fError && m_ui->cbConfigFile->isChecked()) {
        QFile configFile(this->obj->getConfigPath() + "/" + this->obj->getConfigName() + ".ovpn");
        if (configFile.exists()) {
            if (!configFile.remove()) {
                fError = true;
                errMes = configFile.errorString();
            }
        }
    }

    if (!fError && m_ui->cbUserdata->isChecked()) {
        this->getOpenVpnObject()->removeCredentials(false);
    }

    if (!fError && m_ui->cbScript->isChecked()) {
        QFile scriptFile(this->obj->getConfigPath() + "/scripts.conf");
        if (scriptFile.exists()) {
            if (!scriptFile.remove()) {
                fError = true;
                errMes = scriptFile.errorString();
            }
        }
    }

    if (!fError && m_ui->cbDir->isChecked()) {
        // Dir löschen wenn leer
        QDir delDir (this->obj->getConfigPath());
        if (!delDir.rmdir(this->obj->getConfigPath())){
            fError = true;
            errMes = QString(tr("Directory is not empty! Please delete it by hand!"));
        }
    }

    if (fError) {                
        if (Message::confirm(errMes + QLatin1String ("\n") + QObject::tr("Do you want to open the explorer to check the files?\nOtherwise maybe malfunctions can be occurred."))) {
            this->explorerPath = QLatin1String("file:///") + this->obj->getConfigPath();
            if (!this->explorerPath.isEmpty()) {
                QTimer::singleShot(300, this, SLOT(openExplorer()));
            }
        }
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
    QFile conf (obj->getConfigPath() + QLatin1String("/") + obj->getConfigName() + QLatin1String(".ovpn"));
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
    QFile conf (obj->getConfigPath() + "/" + obj->getConfigName() + ".ovpn");
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
    QFile conf (obj->getConfigPath() + "/" + obj->getConfigName() + ".ovpn");
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
    QFile conf (obj->getConfigPath() + "/" + obj->getConfigName() + ".ovpn");
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
