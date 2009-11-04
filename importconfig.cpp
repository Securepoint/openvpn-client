#include "importconfig.h"
#include "ui_importconfig.h"

ImportConfig::ImportConfig(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ImportConfig)
{
    m_ui->setupUi(this);
    m_ui->txtPassword->setEchoMode(QLineEdit::Password);
}

ImportConfig::~ImportConfig()
{
    delete m_ui;
}

void ImportConfig::changeEvent(QEvent *e)
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

void ImportConfig::on_cmdOpenCryptFile_clicked()
{
    QFileDialog certFileDialog;
    QString filename = certFileDialog.getOpenFileName(this, tr("Find import file"), QApplication::applicationDirPath(), "Import files (*.crypt)");
    if (filename != "") {
        m_ui->txtImportPath->setText(filename);
    }
}

void ImportConfig::on_cmdCancel_clicked()
{
    this->close();
}

void ImportConfig::on_rbSaveAsName_toggled(bool checked)
{
    if (checked) {
        m_ui->txtNewName->setEnabled(true);
    } else {
        m_ui->txtNewName->setText("");
        m_ui->txtNewName->setEnabled(false);
    }
}

void ImportConfig::resetFields() {
    m_ui->txtImportPath->setText("");
    m_ui->txtNewName->setText("");
    m_ui->txtNewName->setEnabled(false);
    m_ui->rbSaveAsFile->setChecked(true);
}

void ImportConfig::on_cmdImport_clicked()
{
    if (m_ui->txtExistingOvpn->text() != "") {
        // Import existing file
        QFile linkOvpn (QApplication::applicationDirPath() + QString ("/configs.txt"));
        if (!linkOvpn.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
            QMessageBox::critical(0,"Securepoint VPN Client", "Unable to open configs.txt!");
            return;
        }
        // Datei offen, Config schreiben
        QTextStream out (&linkOvpn);
        out << m_ui->txtExistingOvpn->text() + QString ("\n");
        // Liste aktualisieren
        linkOvpn.close();
        Preferences *prefDialog = dynamic_cast<Preferences*> (this->parent());
        prefDialog->refreshConfigList();
        // Fertig
        QMessageBox::information(0, QString("Securepoint VPN Client"), QString("Import successfully ended!"));
        this->close();
    } else {
        // Import crypt file
        if (m_ui->txtPassword->text() == "") {
            QMessageBox::critical(0, QString("Securepoint VPN Client"), QString ("No password specifyed!"));
            return;
        }
        if (m_ui->txtImportPath->text() != "") {
            if (m_ui->rbSaveAsName->isChecked() && m_ui->txtNewName->text() == "") {
                QMessageBox::critical(0, QString("Securepoint VPN Client"), QString ("No import name specifyed!"));
                return;
            }

            // Portale oder install
            AppFunc app;
            QString dirPath;
            QString configName;

            if (!m_ui->rbSaveAsName->isChecked()) {
                configName = m_ui->txtImportPath->text().right(m_ui->txtImportPath->text().size() - m_ui->txtImportPath->text().lastIndexOf("/") -1);
                configName = configName.left(configName.size()-6);
            } else {
                configName = m_ui->txtNewName->text().trimmed();
            }

            dirPath = app.getAppSavePath() + QString ("/") + configName;

            // Verzeichnis da?
            QDir dirobj (dirPath);
            if (!dirobj.exists(dirPath)){
                //Verzeichnis existiert nicht
                // Pfad erstellen
                if (!dirobj.mkpath(dirPath)) {
                    // Pfad konnte nicht erstellt werden
                    QMessageBox::critical(0,"Securepoint VPN Client", "Unable to create directory!");
                    return;
                }
            } else {
                // Verzeichnis existiert
                QMessageBox::critical(0, QString("Securepoint VPN Client"), QString ("A diretory with this name already exists!"));
                return;
            }
            // Datei ins neue Verzeichnis kopieren
            //QFile importFileCrypt (m_ui->txtImportPath->text());
            QString packFile = dirPath + QString("/") + configName + QString(".7z");

            QProcess packCrypt;
            QStringList argCrypt;
            QString programCrypt = "./app/bin/openssl.exe";

            argCrypt << QString("des3");
            argCrypt << QString("-in");
            argCrypt << m_ui->txtImportPath->text();
            argCrypt << QString("-out");
            argCrypt << packFile;
            argCrypt << QString("-d");
            argCrypt << QString("-salt");
            argCrypt << QString("-k");
            argCrypt << m_ui->txtPassword->text().trimmed();
            packCrypt.start(programCrypt, argCrypt);
            connect( &packCrypt, SIGNAL(error ( QProcess::ProcessError) ), this, SLOT(showProcessError (QProcess::ProcessError)));


            if (!packCrypt.waitForFinished()) {
                    QMessageBox::critical(0,QString("Securepoint VPN Client"), QString("OpenSSL process still running!"));
                    return;
            }

            QProcess packProc;
            QStringList arguments;
            QString program = "./app/bin/7za.exe";

            arguments << QString("e");
            arguments << packFile;
            arguments << QString("-p") + m_ui->txtPassword->text().trimmed();
            arguments << QString("-mhe");
            arguments << QString("-o") + dirPath;
            packProc.start(program, arguments);
            connect( &packProc, SIGNAL(error ( QProcess::ProcessError) ), this, SLOT(showProcessError (QProcess::ProcessError)));

            if (!packProc.waitForFinished()) {
                QMessageBox::critical(0,QString("Securepoint VPN Client"), QString("7z process still running!"));
                return;
            }

            // Datei löschen
            QFile configZip (packFile);
            if (!configZip.remove()) {
                QMessageBox::critical(0, QString("Securepoint VPN Client"), configZip.errorString());
            }
            if (m_ui->rbSaveAsName->isChecked()) {
                // ovpn umbennen
                QString ovpnFilePath = m_ui->txtImportPath->text().right(m_ui->txtImportPath->text().size() - m_ui->txtImportPath->text().lastIndexOf("/") -1);
                        ovpnFilePath = dirPath + QString("/") + ovpnFilePath.left(ovpnFilePath.size()-6) + QString(".ovpn");
                QFile ovpnFile (ovpnFilePath);
                if (ovpnFile.exists()) {
                    // umbenennen
                    ovpnFile.rename(dirPath + QString("/") + configName + QString(".ovpn"));
                }
            }
            Preferences *prefDialog = dynamic_cast<Preferences*> (this->parent());
            prefDialog->refreshConfigList();
            QMessageBox::information(0, QString("Securepoint VPN Client"), QString("Import successfully ended!"));
            this->close();


        } else {
            QMessageBox::critical(0, QString("Securepoint VPN Client"), QString ("No import file selected!"));
            return;
        }
    }
}

void ImportConfig::showProcessError (QProcess::ProcessError err) {
    QString errMessage;
    switch (err) {
        case QProcess::FailedToStart:
            errMessage = QString ("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
            break;
        case QProcess::Crashed:
            errMessage = QString ("The process crashed some time after starting successfully.");
            break;
        case QProcess::Timedout:
            errMessage = QString ("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.");
            break;
        case QProcess::WriteError:
            errMessage = QString ("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.");
            break;
        case QProcess::ReadError:
            errMessage = QString ("An error occurred when attempting to read from the process. For example, the process may not be running.");
            break;
        case QProcess::UnknownError:
            errMessage = QString ("An unknown error occurred. This is the default return value of error().");
            break;
        default:
            errMessage = QString ("No valid error code!");
            break;
    }

    // Daten ausgeben
    QMessageBox::critical(0, QString("Securepoint VPN Client"), errMessage);
}


void ImportConfig::on_cmdOpenOvpnFile_clicked()
{
    AppFunc app;
    QFileDialog certFileDialog;
    QString filename = certFileDialog.getOpenFileName(this, tr("Find ovpn file"), app.getAppSavePath(), "OpenVPN configs (*.ovpn)");
    if (filename != "") {
        m_ui->txtExistingOvpn->setText(filename);
    }
}
