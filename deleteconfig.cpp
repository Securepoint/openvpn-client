#include "deleteconfig.h"
#include "ui_deleteconfig.h"

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
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    this->obj = NULL;
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
    // Mittig ausrichten
    int screenH = qApp->desktop()->height();
    int screenW = qApp->desktop()->width();
    int winH = 250;
    int winW = 230;
    // Nun die neuen setzen
    this->setGeometry((screenW / 2) - (winW / 2), (screenH / 2) - (winH / 2), winW, winH);
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
    bool fError = false;
    QString errMes = "";

    if (m_ui->cbCA->isChecked()) {
        QString ca = this->getCAFromConfig();
        if (ca != "") {
            QString caPath;
            if (ca.indexOf("/") != -1)
                caPath = ca;
            else
                caPath = this->obj->configPath + "/" + ca;

            QFile caFile(caPath);
            if (caFile.exists())
                if (!caFile.remove()) {
                    fError = true;
                    errMes =  caFile.errorString();
                }
        }
    }

    if (!fError)
        if (m_ui->cbCert->isChecked()) {
            QString cert = this->getCertFromConfig();
            if (cert != "") {
                QString certPath;
                if (cert.indexOf("/") != -1)
                    certPath = cert;
                else
                    certPath = this->obj->configPath + "/" + cert;

                QFile certFile(certPath);
                if (certFile.exists())
                    if (!certFile.remove()) {
                        fError = true;
                        errMes =  certFile.errorString();
                    }
            }
        }

    if (!fError)
        if (m_ui->cbKey->isChecked()) {
            QString key = this->getKeyFromConfig();
            if (key != "") {
                QString keyPath;
                if (key.indexOf("/") != -1)
                    keyPath = key;
                else
                    keyPath = this->obj->configPath + "/" + key;

                QFile keyFile(keyPath);
                if (keyFile.exists())
                    if (!keyFile.remove()) {
                        fError = true;
                        errMes =  keyFile.errorString();
                    }

            }
        }

    if (!fError)
        if (m_ui->cbConfigFile->isChecked()) {
            QFile configFile(this->obj->configPath + "/" + this->obj->configName + ".ovpn");
            if (configFile.exists())
                if (!configFile.remove()) {
                    fError = true;
                    errMes = configFile.errorString();
                }
        }

    if (!fError)
        if (m_ui->cbUserdata->isChecked()) {
            QFile userFile(this->obj->configPath + "/auth.dat");
            if (userFile.exists())
                if (!userFile.remove()) {
                    fError = true;
                    errMes = userFile.errorString();
                }
        }

    if (!fError)
        if (m_ui->cbScript->isChecked()) {
            QFile scriptFile(this->obj->configPath + "/scripts.conf");
            if (scriptFile.exists())
                if (!scriptFile.remove()) {
                    fError = true;
                    errMes = scriptFile.errorString();
                }
        }

    if (!fError)
        if (m_ui->cbDir->isChecked()) {
            // Dir löschen wenn leer
            QDir delDir (this->obj->configPath);
            if (!delDir.rmdir(this->obj->configPath)){
                fError = true;
                errMes = QString(tr("Directory is not empty! Please delete it by hand!"));
            }
        }

    if (fError) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Delete Configuration"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(errMes);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
        return;
    }

    // Daten aktualisieren
    MainWindowControll::getInstance()->refreshConfigs();
    MainWindowControll::getInstance()->refreshDialog();
    MainWindowControll::getInstance()->setConnectionStatus();
    // Fertig und schliessen
    this->close();
}

void DeleteConfig::setOpenVpnObject(OpenVpn *obj) {
    this->obj = obj;
}

QString DeleteConfig::getCAFromConfig() {
    QString retVal = "";
    QFile conf (obj->configPath + "/" + obj->configName + ".ovpn");
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
    QFile conf (obj->configPath + "/" + obj->configName + ".ovpn");
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

QString DeleteConfig::getKeyFromConfig() {
    QString retVal = "";
    QFile conf (obj->configPath + "/" + obj->configName + ".ovpn");
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

OpenVpn *DeleteConfig::getOpenVpnObject() {
    return this->obj;
}
