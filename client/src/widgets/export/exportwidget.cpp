#include "src/widgets/export/exportwidget.h"

#include <frmmain.h>
#include <widgetfactory.h>
#include <QtGui/qpainter.h>
#include <database/Crypt.h>
#include <zip/czip.h>
#include <message.h>

ExportWidget::ExportWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.txtExportKey->setEchoMode(QLineEdit::Password);
}

ExportWidget::~ExportWidget()
{

}

void ExportWidget::on_cmdBack_clicked()
{
    //
    // Close me
    //

    FrmMain::instance()->mainWidget()->showWidget(MainView);
}

void ExportWidget::on_cmdExport_clicked()
{
    QString exportKey = "insecure";

    if(!ui.txtExportKey->text().isEmpty())
        exportKey = ui.txtExportKey->text();
    else
    {
        // TODO: notify user password is not secure
    }

    bool fError (false);
    QString errMes ("");

    if (ui.txtDestFile->text().isEmpty()) {
        fError = true;
        errMes = QObject::tr("Invalid path specify[empty]!");
    }

    if (!fError) {
        if (exportKey.isEmpty()) {
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
                Message::error(QObject::tr("Can't open config!"), QObject::tr("Export Configuration"), this);
                return;
            }

            // Besteht das Verzeichnis schon?
            QString dirPath = ui.txtDestFile->text();
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
                    Message::error(QObject::tr("Unable to create directory!"), QObject::tr("Export Configuration"), this);
                    return;
                }
            } else {
                // Verzeichnis existiert
                QFile file(cryptFile);
                if (file.exists()) {
                    // Datei existiert bereits
                    // löschen
                    if (!file.remove()) {
                        Message::error(file.errorString(), QObject::tr("Export Configuration"), this);
                        return;
                    }
                }
            }

            // Alle Daten da, packen
            bool error (false);
			if (!CZip::archive(zipFile, this->configPath.left(this->configPath.lastIndexOf("/")), false)) {
                error = true;
            }

            if (error) {
                Message::error(QObject::tr("Can't zip the data."), QObject::tr("Export Configuration"), this);
                return;
            }

            // Nun verschlüsseln
            if (!fError) {
                QFile zipToCrypt (zipFile);
                if (zipToCrypt.open(QIODevice::ReadOnly)) {
                    QFile cryptedFile (cryptFile);
                    if (cryptedFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                        // Daten einlesen, verschlüsseln und wieder schreiben
                        QByteArray data;
                        data = zipToCrypt.readAll();
                        // Crypt bauen

                        auto oldKey = Crypt::secretKey;
                        // Den Schlüssel setzen
                        Crypt::setSecretKey(exportKey);
                        // Verschlüsseln und schreiben
                        cryptedFile.write(Crypt::encodePlaintextAscii(data).toLatin1());
                        cryptedFile.waitForBytesWritten(2000);
                        cryptedFile.flush();
                        cryptedFile.waitForBytesWritten(2000);

                        // Nun alles wieder schliessen
                        cryptedFile.close();
                        zipToCrypt.close();

                        Crypt::setSecretKey(oldKey);
                        // Alles fertig
                    } else {
                        Message::error(QObject::tr("Can't open crypt file."), QObject::tr("Export Configuration"), this);
                    }
                } else {
                    Message::error(QObject::tr("Can't read zip data."), QObject::tr("Export Configuration"), this);
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
           FrmMain::instance()->mainWidget()->showWidget(MainView);
           Message::error(QObject::tr("No configuration"), QObject::tr("Export Configuration"), this);
        }
    }

    if (fError) {
        Message::error(errMes, QObject::tr("Export Configuration"), this);
        return;
    } else {
        Message::information(QObject::tr("Export successfully ended!"), QObject::tr("Export Configuration"), this);
        FrmMain::instance()->mainWidget()->showWidget(MainView);
    }
}

void ExportWidget::paintEvent(QPaintEvent *pe)
{
    //
    // Draw it with stylesheet support
    // Sometimes I hate Qt!
    // For further information, take a look at
    // http://stackoverflow.com/questions/18344135/why-do-stylesheets-not-work-when-subclassing-qwidget-and-using-q-object
    //

    Q_UNUSED(pe)

    QStyleOption option;
    option.initFrom(this);
    //
    QPainter painter(this);
    //
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}

void ExportWidget::showEvent(QShowEvent *event)
{
    //
    // Clear all fields
    //

    Q_UNUSED(event)

    ui.txtDestFile->clear();
    ui.txtExportKey->clear();
}

void ExportWidget::on_cmdSelectImportFile_clicked()
{
    QFileDialog exportDirDialog;
    QString dirname = exportDirDialog.getExistingDirectory(this, tr("Open Directory"), QApplication::applicationDirPath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dirname.isEmpty()) {
        ui.txtDestFile->setText(dirname);
    }
}

void ExportWidget::SetConfigPath(QString configPath)
{
    this->configPath = configPath;
}
