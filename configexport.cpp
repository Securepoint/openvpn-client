#include "configexport.h"
#include "ui_configexport.h"

ConfigExport *ConfigExport::mInst = NULL;

ConfigExport *ConfigExport::getInstance() {
    if (!mInst)
        mInst = new ConfigExport ();
    return mInst;
}

ConfigExport::ConfigExport() :
    QDialog(),
    m_ui(new Ui::ConfigExport)
{
    m_ui->setupUi(this);
    m_ui->txtExportPwd->setEchoMode(QLineEdit::Password);
    this->configPath = "";
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
}

void ConfigExport::setConfigPath(QString path) {
    this->configPath = path;
}

void ConfigExport::changeEvent(QEvent *e)
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

void ConfigExport::showEvent(QShowEvent *e) {
    m_ui->txtExportPwd->setText("");
    m_ui->txtSaveTo->setText("");
    // Mittig ausrichten
    int screenH = qApp->desktop()->height();
    int screenW = qApp->desktop()->width();
    int winH = 270;
    int winW = 220;
    // Nun die neuen setzen
    this->setGeometry((screenW / 2) - (winW / 2), (screenH / 2) - (winH / 2), winW, winH);
    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
}


void ConfigExport::on_cmdCancel_clicked()
{
    this->close();
}

void ConfigExport::on_cmdExport_clicked()
{
    bool fError = false;
    QString errMes = "";

    if (m_ui->txtSaveTo->text() == "") {
        fError = true;
        errMes = QString(tr("Invalid path specify[empty]!"));
    }

    if (!fError)
        if (m_ui->txtExportPwd->text() == "") {
            fError = true;
            errMes = QString(tr("Invalid password specify[empty]!"));
        }
    // Viel zu tun
    // Dateien ermitteln, packen, verschlüsseln und löschen
    if (!fError) {
        if (this->configPath != "") {
            // Config öffen und Zertifikate auslesen
            QFile configFile (this->configPath);

            if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                msgBox.setText(tr("Export Configuration"));
                msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                msgBox.setInformativeText(tr("Can't open config!"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                msgBox.exec();
                return;
            }
            // Datei offen, einlesen
            QTextStream in (&configFile);
            QString sCA;
            QString sCert;
            QString sKey;
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.left(2).toUpper().trimmed() == "CA") {
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        sCA = QString("");
                    else {
                        QString val = keyvalList[1];
                        sCA = val.replace("\"","");
                    }
                }
                if (line.left(4).toUpper().trimmed() == "CERT") {
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        sCert = QString("");
                    else {
                        QString val = keyvalList[1];
                        sCert = val.replace("\"","");
                    }
                }
                if (line.left(3).toUpper().trimmed() == "KEY") {
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        sKey = QString("");
                    else {
                        QString val = keyvalList[1];
                        sKey = val.replace("\"","");
                    }
                }
            }
            configFile.close();

            // Zertifikatpfade bauen
            if (sCA.indexOf("/") == -1) {
                // Kein Pfad angeben
                sCA = this->configPath.left(this->configPath.lastIndexOf("/")) + "/" + sCA;
            }
            if (sCert.indexOf("/") == -1) {
                // Kein Pfad angeben
                sCert = this->configPath.left(this->configPath.lastIndexOf("/")) + "/" + sCert;
            }
            if (sKey.indexOf("/") == -1) {
                // Kein Pfad angeben
                sKey = this->configPath.left(this->configPath.lastIndexOf("/"))  + "/" + sKey;
            }
            // COnfig, CA, Cert und KEy Pfade sind da
            // nun noch die Script Config
            QString scriptConfig  = this->configPath.left(this->configPath.lastIndexOf("/"))  + "/scripts.conf";
            QFile scriptFile (scriptConfig);
            if (!scriptFile.exists())
                scriptConfig = QString("");
            scriptFile.close();

            // Besteht das Verzeichnis schon?
            QString dirPath = m_ui->txtSaveTo->text();
            QString zipFile = dirPath + QString("/export.7z");
            QString configName = this->configPath.right(this->configPath.size() - this->configPath.lastIndexOf("/") -1);
            configName = configName.left(configName.size()-5);
            QString cryptFile = dirPath + QString("/") + configName + QString(".crypt");
            QDir dirobj (dirPath);
            if (!dirobj.exists(dirPath)){
                //Verzeichnis existiert nicht
                // Pfad erstellen
                if (!dirobj.mkpath(dirPath)) {
                    // Pfad konnte nicht erstellt werden
                    QMessageBox msgBox;
                    msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                    msgBox.setText(tr("Export Configuration"));
                    msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                    msgBox.setInformativeText(tr("Unable to create directory!"));
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.setDefaultButton(QMessageBox::Ok);
                    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                    msgBox.exec();
                    return;
                }
            } else {
                // Verzeichnis existiert
                QFile file(cryptFile);
                if (file.exists()) {
                    // Datei existiert bereits
                    // löschen
                    if (!file.remove()) {
                        QMessageBox msgBox;
                        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                        msgBox.setText(tr("Export Configuration"));
                        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                        msgBox.setInformativeText(file.errorString());
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.setDefaultButton(QMessageBox::Ok);
                        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                        msgBox.exec();
                        return;
                    }
                }
            }

            // Alle Daten da, packen
            QProcess packProc;
            QStringList arguments;
            QString program = "./app/bin/7za.exe";

            arguments << QString("a");
            arguments << zipFile;
            arguments << QString("-p") + m_ui->txtExportPwd->text();
            arguments << QString("-mhe");
            arguments << this->configPath;
            arguments << sCA;
            arguments << sCert;
            arguments << sKey;
            arguments << scriptConfig;
            packProc.start(program, arguments);

            if (!packProc.waitForFinished(3000)) {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                msgBox.setText(tr("Export Configuration"));
                msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                msgBox.setInformativeText(tr("7z process still running!"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                msgBox.exec();
                return;
            }


            QProcess packCrypt;
            QStringList argCrypt;
            QString programCrypt = "./app/bin/openssl.exe";

            argCrypt << QString("des3");
            argCrypt << QString("-in");
            argCrypt << zipFile;
            argCrypt << QString("-out");
            argCrypt << cryptFile;
            argCrypt << QString("-e");
            argCrypt << QString("-salt");
            argCrypt << QString("-k");
            argCrypt << m_ui->txtExportPwd->text();
            packCrypt.start(programCrypt, argCrypt);


            if (!packCrypt.waitForFinished(3000)) {
                fError = true;
                errMes = QString(tr("OpenSSL process still running!"));
            }

            // Datei löschen
            if (!fError) {
                QFile configZip (zipFile);
                if (!configZip.remove()) {
                    fError = true;
                    errMes = configZip.errorString();
                }
            }            
        } else {
            this->close();
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
            msgBox.setText(tr("Export Configuration"));
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setInformativeText(tr("No configuration"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
            msgBox.exec();
        }
    }

    if (fError) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Export Configuration"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(errMes);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
        return;
    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Export Configuration"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(tr("Export successfully ended!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
        this->close();
    }
}

void ConfigExport::on_cmdOpenDir_clicked()
{
    QFileDialog exportDirDialog;
    QString dirname = exportDirDialog.getExistingDirectory(this, tr("Open Directory"),
                                                 QApplication::applicationDirPath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (dirname != "") {
       m_ui->txtSaveTo->setText(dirname);
    }
}
