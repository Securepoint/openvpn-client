#include "importconfig.h"
#include "ui_importconfig.h"

#include "preferences.h"
#include "appfunc.h"
#include "crypt.h"
#include "zip.h"

#include "message.h"
#include "settings.h"
#include "configvalues.h"

ImportConfig::ImportConfig(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ImportConfig)
{
    m_ui->setupUi(this);
    m_ui->txtPassword->setEchoMode(QLineEdit::Password);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
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

void ImportConfig::showEvent(QShowEvent *e) {
    m_ui->txtImportPath->clear();
    m_ui->txtNewName->clear();
    m_ui->txtPassword->clear();
    m_ui->txtNewName->setEnabled(false);
    m_ui->rbSaveAsFile->setChecked(true);
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

void ImportConfig::on_cmdOpenCryptFile_clicked()
{
    QFileDialog certFileDialog;
    QString filename = certFileDialog.getOpenFileName(this, tr("Find import file"), QApplication::applicationDirPath(), tr("Import files (*.crypt)"));
    if (!filename.isEmpty()) {
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

QString keys[] = {
	"ca",
	"cert",
	"key",
	"pkcs12",
	"auth-user-pass",
	"secret",
	"replay-persist",
	"dh",
	"cert",
	"extra-certs",
	"key",
	"pkcs12",
	"tls-auth",
	"tls-auth",
	"askpass",
	"crl-verify"
};

void ImportConfig::on_cmdImport_clicked()
{
    if (!m_ui->txtExistingOvpn->text().isEmpty()) {
        // Import existing file
        QString configName (m_ui->txtExistingOvpn->text().replace(".ovpn", ""));
        configName = configName.right(configName.size() - configName.lastIndexOf("/") - 1);

        // Anderer name als Dateiname
        if (m_ui->rbSaveAsName->isChecked()) {
            if (!m_ui->txtNewName->text().isEmpty()) {
                configName = m_ui->txtNewName->text();
            }
        }

        QString pathToConfig (m_ui->txtExistingOvpn->text());
        // Copy config to local app data and rename it, if an other name is specified
        // Only in service mode!
        if(!Settings::getInstance()->getIsPortableClient()) {

            // If file is available copy it
            // First create folder
            QString newConfigFolderPath (QString("%1/config/%2")
                                         .arg(AppFunc::getAppSavePath())
                                         .arg(configName));
            QDir newConfigFolderPathDirectory (newConfigFolderPath);
            if (newConfigFolderPathDirectory.exists(newConfigFolderPath)){
                // A config with this name is already existing
                Message::error(QObject::tr("A configuration with this name is already existing!"));
                //
                return;
            }

            // Create path
            newConfigFolderPathDirectory.mkpath(newConfigFolderPath);

            // Now copy Files
            QString newConfigPath (QString("%1/%2.ovpn")
                                   .arg(newConfigFolderPath)
                                   .arg(configName));

            QFile::copy(pathToConfig, newConfigPath);
            //
            QString sourceDirectory (pathToConfig.left(pathToConfig.lastIndexOf("/")));
            //
            // Override old path
            pathToConfig = newConfigPath;
            // Ca

			auto copyConfigFile = [&](const QString &key)
			{
				auto keyValue = (ConfigValues::instance()->valueFromConfigKey(pathToConfig, key));

				if(!keyValue.isEmpty())
				{
					QString sourcePath (QString("%1/%2")
						.arg(sourceDirectory)
						.arg(keyValue));

					QString destName (keyValue);

					if (ConfigValues::instance()->isGivenPathAbsolute(keyValue)) {
						// Yes, override path
						sourcePath = keyValue;
						// Get the file name from path
						destName = ConfigValues::instance()->fileNameOfAbsolutePath(keyValue);
						// Change value in config
						ConfigValues::instance()->changeKeyValueInConfig(pathToConfig, key, QString("\"%1\"")
							.arg(destName));
					}
					// Copy
					QFile::copy(sourcePath, QString("%1/%2")
						.arg(newConfigFolderPath)
						.arg(destName));
				}
			};

			for(const auto &key : keys)
			{
				copyConfigFile(key);
			}
        }

        Preferences::instance()->addNewConfigToDatabase(configName, pathToConfig);
        Preferences::instance()->refreshConfigList();
        Preferences::instance()->setConnectionStatus();
        Preferences::instance()->setIcon();

        // Fertig
        Message::information(QObject::tr("Import successfully ended!"), QObject::tr("Import Configuration"));
        Preferences::instance()->refreshDialog();
        this->close();
    } else {
        // Import crypt file
        if (m_ui->txtPassword->text().isEmpty()) {
            Message::error(QObject::tr("No password specified!"), QObject::tr("Import Configuration"));
            return;
        }
        if (!m_ui->txtImportPath->text().isEmpty()) {
            if (m_ui->rbSaveAsName->isChecked() && m_ui->txtNewName->text().isEmpty()) {
                Message::error(QObject::tr("No import name specified!"), QObject::tr("Import Configuration"));
                return;
            }

            // Portale oder install
            QString dirPath;
            QString configName;

            if (!m_ui->rbSaveAsName->isChecked()) {
                configName = m_ui->txtImportPath->text().right(m_ui->txtImportPath->text().size() - m_ui->txtImportPath->text().lastIndexOf("/") -1);
                configName = configName.left(configName.size()-6);
            } else {
                configName = m_ui->txtNewName->text().trimmed();
            }

            dirPath = AppFunc::getAppSavePath() + QString ("/config/") + configName;

            // Verzeichnis da?
            QDir dirobj (dirPath);
            if (!dirobj.exists(dirPath)){
                //Verzeichnis existiert nicht
                // Pfad erstellen
                if (!dirobj.mkpath(dirPath)) {
                    // Pfad konnte nicht erstellt werden
                    Message::error(QObject::tr("Unable to create directory!"), QObject::tr("Import Configuration"));
                    return;
                }
            } else {
                // Verzeichnis existiert
                Message::error(QObject::tr("A diretory with this name already exists!"), QObject::tr("Import Configuration"));
                return;
            }
            // Datei ins neue Verzeichnis kopieren
            //QFile importFileCrypt (m_ui->txtImportPath->text());
            QString packFile = dirPath + QString("/") + configName + QString(".zip");

            // Erstmal entschlüsseln
            if(QFile::exists(packFile)) {
                QFile::remove(packFile);
            }

            // Die Daten einlesen
            {
                QFile crypted (m_ui->txtImportPath->text());
                if (crypted.open(QIODevice::ReadOnly)) {
                    // Nun das Ziel öfffnen
                    QFile targetFile (packFile);
                    if (targetFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                        // Alles offen, kann losgehen
                        QByteArray data = crypted.readAll();
                        Crypt crypt;
                        crypt.setSecretKey(m_ui->txtPassword->text());
                        targetFile.write(crypt.cryptToPlainTextExt(data));
                        targetFile.waitForBytesWritten(3000);
                        targetFile.flush();
                        targetFile.waitForBytesWritten(3000);

                        // Alles Ok
                        targetFile.close();
                        crypted.close();
                    }
                } else {
                    Message::error(QObject::tr("Can't open crypted file"));
                    return;
                }
            }

            // Nun die Datei entpacken
            if (!Zip::extract(packFile, dirPath)) {
                Message::error(QObject::tr("Can't open zip file"));
                return;
            }

            // Nun ist alles entpackt, das Ziparchiv nun löschen
            QFile configZip (packFile);
            if (!configZip.remove()) {
                Message::error(configZip.errorString(), QObject::tr("Import Configuration"));
            }

            QString saveName;
            QString savePath;
            QString ovpnFilePath = m_ui->txtImportPath->text().right(m_ui->txtImportPath->text().size() - m_ui->txtImportPath->text().lastIndexOf("/") -1);

            saveName = ovpnFilePath.left(ovpnFilePath.size()-6);
            savePath = dirPath + QString("/") + ovpnFilePath.left(ovpnFilePath.size()-6) + QString(".ovpn");

            if (m_ui->rbSaveAsName->isChecked()) {
                // ovpn umbennen
                QFile ovpnFile (savePath);
                if (ovpnFile.exists()) {
                    // umbenennen
                    ovpnFile.rename(dirPath + QString("/") + configName + QString(".ovpn"));
                    saveName = configName;
                }
            }
            savePath = dirPath + QString("/") + saveName + QString(".ovpn");

            QFile ovpnFile (dirPath + QString("/") + configName + QString(".ovpn"));
            if (!ovpnFile.exists()) {
                Message::error(QObject::tr("Import failed! Removing empty directory."), QObject::tr("Import Configuration"));
                dirobj.rmdir(dirPath);
            } else {
                Preferences::instance()->addNewConfigToDatabase(saveName, savePath.replace("\\", "/"));
                Preferences::instance()->refreshConfigList();
                Preferences::instance()->setConnectionStatus();
                Message::information(QObject::tr("Import successfully ended!"), QObject::tr("Import Configuration"));
                Preferences::instance()->refreshDialog();
                Preferences::instance()->setIcon();
                this->close();
            }
        } else {
            Message::error(QObject::tr("No import file selected!"), QObject::tr("Import Configuration"));
        }
    }
}

void ImportConfig::showProcessError (QProcess::ProcessError err) {
    QString errMessage;
    switch (err) {
        case QProcess::FailedToStart:
            errMessage = QLatin1String ("The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.");
            break;
        case QProcess::Crashed:
            errMessage = QLatin1String ("The process crashed some time after starting successfully.");
            break;
        case QProcess::Timedout:
            errMessage = QLatin1String ("The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.");
            break;
        case QProcess::WriteError:
            errMessage = QLatin1String ("An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.");
            break;
        case QProcess::ReadError:
            errMessage = QLatin1String ("An error occurred when attempting to read from the process. For example, the process may not be running.");
            break;
        case QProcess::UnknownError:
            errMessage = QLatin1String ("An unknown error occurred. This is the default return value of error().");
            break;
        default:
            errMessage = QLatin1String ("No valid error code!");
            break;
    }

    // Daten ausgeben
    Message::error(errMessage, QObject::tr("Import Configuration"));
}


void ImportConfig::on_cmdOpenOvpnFile_clicked()
{
    QFileDialog certFileDialog;
    QString filename (certFileDialog.getOpenFileName(this, QObject::tr("Find ovpn file"), AppFunc::getAppSavePath(), QObject::tr("OpenVPN configs (*.ovpn)")));

    if (!filename.isEmpty()) {
        m_ui->txtExistingOvpn->setText(filename);
    }
}
