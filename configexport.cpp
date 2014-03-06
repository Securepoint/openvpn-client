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
			if (!Zip::archive(zipFile, this->configPath.left(this->configPath.lastIndexOf("/")), false)) {
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
