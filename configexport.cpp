#include "configexport.h"
#include "ui_configexport.h"

ConfigExport::ConfigExport(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ConfigExport)
{
    m_ui->setupUi(this);
}

ConfigExport::~ConfigExport()
{
    delete m_ui;
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

void ConfigExport::on_cmdCancel_clicked()
{
    this->close();
}

void ConfigExport::on_cmdExport_clicked()
{
    if (m_ui->txtExportPwd->text() == "") {
        QMessageBox::critical(0,QString("OpenVPN CLient"), QString("Invalid password specify[empty]!"));
        return;
    }
    // Viel zu tun
    // Dateien ermitteln, packen, verschlüsseln und löschen
    if (this->configPath != "") {
        // Config öffen und Zertifikate auslesen
        QFile configFile (this->configPath);

        if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(0,QString("OpenVPN Client"), QString("Can't open config!"));
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
        QString dirPath = this->configPath.left(this->configPath.lastIndexOf("/"))  + "/export/";
        QString zipFile = this->configPath.left(this->configPath.lastIndexOf("/")) + QString("/export/export.7z");
        QString configName = this->configPath.right(this->configPath.size() - this->configPath.lastIndexOf("/") -1);
        configName = configName.left(configName.size()-5);
        QString cryptFile = this->configPath.left(this->configPath.lastIndexOf("/")) + QString("/export/") + configName + QString(".crypt");
        QDir dirobj (dirPath);
        if (!dirobj.exists(dirPath)){
            //Verzeichnis existiert nicht
            // Pfad erstellen
            if (!dirobj.mkpath(dirPath)) {
                // Pfad konnte nicht erstellt werden
                QMessageBox::critical(0,"Securepoint OpenVPN Client", "Unable to create directory!");
                return;
            }
        } else {
            // Verzeichnis existiert
            QFile file(cryptFile);
            if (file.exists()) {
                // Datei existiert bereits
                // löschen
                if (!file.remove()) {
                    QMessageBox::critical(0, QString("OpenVPN Client"), file.errorString());
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

        if (!packProc.waitForFinished()) {
            QMessageBox::critical(0,QString("OpenVPN CLient"), QString("7z process still running!"));
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


        if (!packCrypt.waitForFinished()) {
                QMessageBox::critical(0,QString("OpenVPN Client"), QString("OpenSSL process still running!"));
                return;
        }

        // Datei löschen
        QFile configZip (zipFile);
        if (!configZip.remove()) {
            QMessageBox::critical(0, QString("OpenVPN Client"), configZip.errorString());
        }
        QMessageBox::information(0, QString("OpenVPN Client"), QString("Export successfully ended!"));
        this->close();

    } else {
        this->close();
        QMessageBox::critical(0,QString("OpenVPN Client"), QString("No config!"));
    }

}
