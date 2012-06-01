#include "configexport.h"
#include "ui_configexport.h"

#include "preferences.h"
#include "message.h"
#include "zip.h"

#include "crypt.h"

ConfigExport::ConfigExport(const QString &path) :
    QDialog(),
    m_ui(new Ui::ConfigExport),
    configPath (path)
{
    m_ui->setupUi(this);
    m_ui->txtExportPwd->setEchoMode(QLineEdit::Password);    
    this->setWindowFlags(Qt::WindowCloseButtonHint);
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
    m_ui->txtExportPwd->clear();
    m_ui->txtSaveTo->clear();
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


void ConfigExport::on_cmdCancel_clicked()
{
    this->close();
}

void ConfigExport::on_cmdExport_clicked()
{
    bool fError (false);
    QString errMes ("");

    if (m_ui->txtSaveTo->text().isEmpty()) {
        fError = true;
        errMes = QObject::tr("Invalid path specify[empty]!");
    }

    if (!fError) {
        if (m_ui->txtExportPwd->text().isEmpty()) {
            fError = true;
            errMes = QObject::tr("Invalid password specify[empty]!");
        }
    }
    // Viel zu tun
    // Dateien ermitteln, packen, verschlüsseln und löschen
    if (!fError) {
        if (!this->configPath.isEmpty()) {
            // Config öffen und Zertifikate auslesen
            QFile configFile (this->configPath);

            if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                Message::error(QObject::tr("Can't open config!"), QObject::tr("Export Configuration"));
                return;
            }
            // Datei offen, einlesen
            QTextStream in (&configFile);
            QString sCA;
            QString sCert;
            QString sKey;
            QString sPkcs12;

            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.left(2).toUpper().trimmed() == QLatin1String("CA")) {
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        sCA = QString("");
                    else {
                        QString val = keyvalList[1];
                        sCA = val.replace("\"","");
                    }
                }
                if (line.left(4).toUpper().trimmed() == QLatin1String("CERT")) {
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        sCert = QString("");
                    else {
                        QString val = keyvalList[1];
                        sCert = val.replace("\"","");
                    }
                }
                if (line.left(3).toUpper().trimmed() == QLatin1String("KEY")) {
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        sKey = QString("");
                    else {
                        QString val = keyvalList[1];
                        sKey = val.replace("\"","");
                    }
                }
                // Pkcs12
                if (line.left(6).toUpper().trimmed() == QLatin1String("PKCS12")) {
                    QStringList keyvalList = line.split(" ");
                    if (keyvalList.size() != 2)
                        sPkcs12 = QString("");
                    else {
                        QString val = keyvalList[1];
                        sPkcs12 = val.replace("\"","");
                    }
                }
            }
            configFile.close();

            // Zertifikatpfade bauen
            if (sCA.indexOf("/") == -1 && sCA.indexOf("\\") == -1) {
                // Kein Pfad angeben
                sCA = this->configPath.left(this->configPath.lastIndexOf("/")) + "/" + sCA;
            }
            if (sCert.indexOf("/") == -1 && sCert.indexOf("\\") == -1) {
                // Kein Pfad angeben
                sCert = this->configPath.left(this->configPath.lastIndexOf("/")) + "/" + sCert;
            }
            if (sKey.indexOf("/") == -1 && sKey.indexOf("\\") == -1) {
                // Kein Pfad angeben
                sKey = this->configPath.left(this->configPath.lastIndexOf("/"))  + "/" + sKey;
            }

            if (sPkcs12.indexOf("/") == -1 && sPkcs12.indexOf("\\") == -1) {
                // Kein Pfad angeben
                sPkcs12 = this->configPath.left(this->configPath.lastIndexOf("/"))  + "/" + sPkcs12;
            }

            // COnfig, CA, Cert und KEy Pfade sind da
            // nun noch die Script Config
            QString scriptConfig  = this->configPath.left(this->configPath.lastIndexOf("/"))  + QLatin1String("/scripts.conf");
            QFile scriptFile (scriptConfig);
            if (!scriptFile.exists())
                scriptConfig = QString("");
            scriptFile.close();

            // Besteht das Verzeichnis schon?
            QString dirPath = m_ui->txtSaveTo->text();
            QString zipFile = dirPath + QLatin1String("/export.zip");
            QString configName = this->configPath.right(this->configPath.size() - this->configPath.lastIndexOf("/") -1);
            configName = configName.left(configName.size()-5);
            QString cryptFile = dirPath + QLatin1String("/") + configName + QLatin1String(".crypt");
            QDir dirobj (dirPath);
            if (!dirobj.exists(dirPath)){
                //Verzeichnis existiert nicht
                // Pfad erstellen
                if (!dirobj.mkpath(dirPath)) {
                    // Pfad konnte nicht erstellt werden
                    Message::error(QObject::tr("Unable to create directory!"), QObject::tr("Export Configuration"));
                    return;
                }
            } else {
                // Verzeichnis existiert
                QFile file(cryptFile);
                if (file.exists()) {
                    // Datei existiert bereits
                    // löschen
                    if (!file.remove()) {
                        Message::error(file.errorString(), QObject::tr("Export Configuration"));
                        return;
                    }
                }
            }

            // Alle Daten da, packen
            bool error (false);
            if (!Zip::archiveFile(zipFile, this->configPath, false)) {
                error = true;
            }

            if (!Zip::archiveFile(zipFile, sCA, true)) {
                error = true;
            }

            if (!Zip::archiveFile(zipFile, sCert, true)) {
                error = true;
            }

            if (!Zip::archiveFile(zipFile, sKey, true)) {
                error = true;
            }

            if (!Zip::archiveFile(zipFile, scriptConfig, true)) {
                error = true;
            }

            if (!Zip::archiveFile(zipFile, sPkcs12, true)) {
                error = true;
            }

            if (error) {
                Message::error(QObject::tr("Can't zip the data."), QObject::tr("Export Configuration"));
                return;
            }

            // Nun verschlüsseln
            if (!fError) {
                {
                    QFile zipToCrypt (zipFile);
                    if (zipToCrypt.open(QIODevice::ReadOnly)) {
                        QFile cryptedFile (cryptFile);
                        if (cryptedFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                            // Daten einlesen, verschlüsseln und wieder schreiben
                            QByteArray data;
                            data = zipToCrypt.readAll();
                            // Crypt bauen
                            Crypt crypt;
                            // Den Schlüssel setzen
                            crypt.setSecretKey(m_ui->txtExportPwd->text());
                            // Verschlüsseln und schreiben
                            cryptedFile.write(crypt.cryptPlainTextExt(data));
                            cryptedFile.waitForBytesWritten(2000);
                            cryptedFile.flush();
                            cryptedFile.waitForBytesWritten(2000);

                            // Nun alles wieder schliessen
                            cryptedFile.close();
                            zipToCrypt.close();
                            // Alles fertig
                        } else {
                            Message::error(QObject::tr("Can't open crypt file."), QObject::tr("Export Configuration"));
                        }
                    } else {
                        Message::error(QObject::tr("Can't read zip data."), QObject::tr("Export Configuration"));
                    }
                }               
            }

            // Das Zip-File nun löschen
            QFile configZip (zipFile);
            if (configZip.exists()) {
                if (!configZip.remove()) {
                    fError = true;
                    errMes = configZip.errorString();
                }
            }
        } else {
            this->close();
            Message::error(QObject::tr("No configuration"), QObject::tr("Export Configuration"));
        }
    }

    if (fError) {
        Message::error(errMes, QObject::tr("Export Configuration"));
        return;
    } else {
        Message::information(QObject::tr("Export successfully ended!"), QObject::tr("Export Configuration"));
        this->close();
    }
}

void ConfigExport::on_cmdOpenDir_clicked()
{
    QFileDialog exportDirDialog;
    QString dirname = exportDirDialog.getExistingDirectory(this, tr("Open Directory"), QApplication::applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dirname.isEmpty()) {
       m_ui->txtSaveTo->setText(dirname);
    }
}
