#include "stduserauth.h"
#include "ui_stduserauth.h"

StdUserAuth::StdUserAuth(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::StdUserAuth)
{
    m_ui->setupUi(this);
}

StdUserAuth::~StdUserAuth()
{
    delete m_ui;   
}

void StdUserAuth::changeEvent(QEvent *e)
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

void StdUserAuth::fillFromConfig() {
    QString conDataFilePath;
    conDataFilePath = this->parentVpn->configPath + QString("/data.conf");
    QFile conDataFileRead (conDataFilePath);
    if (conDataFileRead.exists()) {
        m_ui->cbSavePassword->setChecked(true);
        // Userdatei da
        if (!conDataFileRead.open(QIODevice::ReadOnly | QIODevice::Text)) {
             return;
        }
        // Datei offen auslesen
        QTextStream in (&conDataFileRead);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.left(2) == "U:") {
                // Username
                m_ui->txtUser->setText(line.right(line.size() -2).trimmed());
            }
        }
        conDataFileRead.close();
    }
}

void StdUserAuth::clearFields() {
    m_ui->txtPwd->setText("");
    m_ui->txtUser->setText("");
}

void StdUserAuth::getAuth() {

}

void StdUserAuth::on_cmdClose_clicked()
{
    this->close();
}

void StdUserAuth::on_cmdOK_clicked()
{
    QString conDataFilePath;
    conDataFilePath = this->parentVpn->configPath + QString("/data.conf");
    if (m_ui->cbSavePassword->isChecked()) {
        // User merken
        if (m_ui->txtUser->text() != "") {
            QFile conDataFile (conDataFilePath);
            if (!conDataFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
                // Datei kann nicht erstellt werden
                QMessageBox::critical(0,"Securepoint OpenVPN Client", "Can't create Configfile!");
                return;
            }
            // Datei offen Usernamen schreiben
            QTextStream out(&conDataFile);
            out << "U:" << m_ui->txtUser->text() << "\n";
            conDataFile.close();
        }
    } else {
        // Datei wieder löschen
        QFile conDataFileRemove (conDataFilePath);
        if (conDataFileRemove.exists()) {
            conDataFileRemove.remove();
            conDataFileRemove.close();
        }
    }
    // Lesen
    QFile conDataFileRead (conDataFilePath);
    if (conDataFileRead.exists()) {
        // Kennwortdatei da
        if (!conDataFileRead.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox msgBox;
                        msgBox.setText("Cannot read file.");
                        msgBox.exec();
             return;
        }
        // Datei offen auslesen
        QTextStream in (&conDataFileRead);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.left(2) == "U:") {
                // Username
                this->parentVpn->configUser = line.right(line.size() -2).trimmed();
            } else {
                this->parentVpn->configUser = m_ui->txtUser->text();
            }
        }
        this->parentVpn->configPwd = m_ui->txtPwd->text();
        conDataFileRead.close();
    } else {
        this->parentVpn->configUser = m_ui->txtUser->text();
        this->parentVpn->configPwd = m_ui->txtPwd->text();
    }
    // Zum VPN Server verbinden
    this->parentVpn->connectToVpn();
    this->close();
}
