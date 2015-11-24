#include "importwidget.h"
#include "ui_importwidget.h"

#include <frmmain.h>
#include <widgetfactory.h>
#include <QtGui/qpainter.h>
#include <QtWidgets/QFileDialog>
#include <config/Configs.h>
#include <config/configvalues.h>
#include <utils.h>
#include <FrmMain.h>
#include <zip/czip.h>
#include <database/crypt.h>
#include <dialogs/frmgetuserdata.h>
#include <message.h>

ImportWidget::ImportWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportWidget)
{
    ui->setupUi(this);

    //QObject::connect(ui->cmdImport, SIGNAL(clicked()), this, SLOT(on_cmdImport_clicked()));
}

ImportWidget::~ImportWidget()
{
    delete ui;
}

void ImportWidget::on_cmdBack_clicked()
{
    //
    // Close me
    //

    FrmMain::instance()->mainWidget()->showWidget(MainView);
}

void ImportWidget::paintEvent(QPaintEvent *pe)
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

void ImportWidget::on_cmdSelectImportFile_clicked()
{
    //
    // Open a select file dialog
    //
    QString homeDir;
   homeDir = QStandardPaths::locate(QStandardPaths::HomeLocation, homeDir, QStandardPaths::LocateOption::LocateDirectory);
    static QString lastDirectory (homeDir);

    QString importFile (QFileDialog::getOpenFileName(this, QObject::tr("Select import file"), lastDirectory , QObject::tr("Configurations (*.ovpn);;Backups (*.crypt)")));
    //
    if (importFile.isEmpty()) {
        return;
    }

    // Set new last dir
    lastDirectory = importFile.left(importFile.lastIndexOf("/"));

    // Fill ui
    ui->txtSourceFile->setText(importFile);
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

void ImportWidget::on_cmdImport_clicked()
{
    if (!ui->txtSourceFile->text().isEmpty())
    {
        qDebug() << "No file specified";
    }
    

    if(ui->txtSourceFile->text().endsWith(".ovpn"))
    {
        // Import existing file
        QString configName (ui->txtSourceFile->text().replace(".ovpn", ""));
        configName = configName.right(configName.size() - configName.lastIndexOf("/") - 1);

        // Anderer name als Dateiname
        if (ui->rbCustomName->isChecked()) {
            if (!ui->txtCustomName->text().isEmpty()) {
                configName = ui->txtCustomName->text();
            }
        }

        QString pathToConfig (ui->txtSourceFile->text());

        // If file is available copy it
        // First create folder
        QString newConfigFolderPath (QString("%1/config/%2")
            .arg(Utils::userApplicationDataDirectory())
            .arg(configName));
        QDir newConfigFolderPathDirectory (newConfigFolderPath);
        if (newConfigFolderPathDirectory.exists(newConfigFolderPath)){
            // A config with this name is already existing
            qDebug() << "A configuration with this name is already existing!";
            Message::error(QObject::tr("A configuration with this name is already existing!"), QObject::tr("An error occurred"), this);
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
            qDebug() << sourceDirectory;
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

                qDebug() << sourcePath;
                qDebug() << newConfigFolderPath;
                qDebug() << destName;

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

        Configs::instance()->findConfigsInDir((Utils::userApplicationDataDirectory() + "/config"));

        // Fertig
        Configs::instance()->refreshConfigs();
        ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.LoadConnections();
        FrmMain::instance()->mainWidget()->showWidget(MainView);
        
    } else {
#if 1

        // TODO: request password for crypt file
        QString strPassword = "";
        FrmGetUserData dialog(InputType::CryptPassword, "Crypt Import");
        QObject::connect(&dialog, SIGNAL(cryptKey(QString)), this, SLOT(cryptKey(QString)));

        dialog.setWindowFlags(dialog.windowFlags() | Qt::WindowStaysOnTopHint);
        dialog.exec();

        if(this->cryptPassword.isEmpty())
        {
            // Notify user
            return;
        }
        strPassword =  this->cryptPassword;
         this->cryptPassword = "";
#if 0
        // Import crypt file
        if (ui->txtPassword->text().isEmpty()) {
            Message::error(QObject::tr("No password specified!"), QObject::tr("Import Configuration"), this);
            return;
        }
#endif
        if (!ui->txtSourceFile->text().isEmpty()) {
            if (ui->rbCustomName->isChecked() && ui->txtCustomName->text().isEmpty()) {
                Message::error(QObject::tr("No import name specified!"), QObject::tr("Import Configuration"), this);
                return;
            }

            // Portale oder install
            QString dirPath;
            QString configName;

            if (!ui->rbCustomName->isChecked()) {
                configName = ui->txtSourceFile->text().right(ui->txtSourceFile->text().size() - ui->txtSourceFile->text().lastIndexOf("/") -1);
                configName = configName.left(configName.size()-6);
            } else {
                configName = ui->txtCustomName->text().trimmed();
            }

            dirPath = Utils::userApplicationDataDirectory() + QString ("/config/") + configName;

            // Verzeichnis da?
            QDir dirobj (dirPath);
            if (!dirobj.exists(dirPath)){
                //Verzeichnis existiert nicht
                // Pfad erstellen
                if (!dirobj.mkpath(dirPath)) {
                    // Pfad konnte nicht erstellt werden
                    Message::error(QObject::tr("Unable to create directory!"), QObject::tr("Import Configuration"), this);
                    return;
                }
            } else {
                // Verzeichnis existiert
                Message::error(QObject::tr("A diretory with this name already exists!"), QObject::tr("Import Configuration"), this);
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
            QFile crypted (ui->txtSourceFile->text());
            if (crypted.open(QIODevice::ReadOnly)) {
                // Nun das Ziel öfffnen
                QFile targetFile (packFile);
                if (targetFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
                    // Alles offen, kann losgehen
                    QByteArray data = crypted.readAll();

                    // Store the current crypt key
                    auto oldCryptKey = Crypt::secretKey;

                    // Set the crypt key to the password
                    Crypt::setSecretKey(QLatin1String(strPassword.toLatin1()));

                    // Encrypt data
                    targetFile.write(Crypt::decodeToPlaintext(data));

                    // Reset the original crypt key
                    Crypt::setSecretKey(QLatin1String(oldCryptKey.toLatin1()));
                    targetFile.waitForBytesWritten(3000);
                    targetFile.flush();
                    targetFile.waitForBytesWritten(3000);
                        
                    // Alles Ok
                    targetFile.close();
                    crypted.close();
                }
            } else {
                Message::error(QObject::tr("Can't open crypted file"), QObject::tr("Import Configuration"), this);
                return;
            }

            // Nun die Datei entpacken
            if (!CZip::extract(packFile, dirPath, "", "")) {
                Message::error(QObject::tr("Can't open zip file"), QObject::tr("Import Configuration"), this);
                return;
            }

            // Nun ist alles entpackt, das Ziparchiv nun löschen
            QFile configZip (packFile);
            if (!configZip.remove()) {
                Message::error(configZip.errorString(), QObject::tr("Import Configuration"), this);
            }

            QString saveName;
            QString savePath;
            QString ovpnFilePath = ui->txtSourceFile->text().right(ui->txtSourceFile->text().size() - ui->txtSourceFile->text().lastIndexOf("/") -1);

            saveName = ovpnFilePath.left(ovpnFilePath.size()-6);
            savePath = dirPath + QString("/") + ovpnFilePath.left(ovpnFilePath.size()-6) + QString(".ovpn");

            if (ui->rbCustomName->isChecked()) {
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
                Message::error(QObject::tr("Import failed! Removing empty directory."), QObject::tr("Import Configuration"), this);
                dirobj.rmdir(dirPath);
            } else {
                Configs::instance()->refreshConfigs();
                ((MainListView*)FrmMain::instance()->mainWidget()->widget(MainView))->model.LoadConnections();
                FrmMain::instance()->mainWidget()->showWidget(MainView);
#if 0
                // do we need this?
                Preferences::instance()->addNewConfigToDatabase(saveName, savePath.replace("\\", "/"));
                Preferences::instance()->refreshConfigList();
                Preferences::instance()->setConnectionStatus();
                Message::information(QObject::tr("Import successfully ended!"), QObject::tr("Import Configuration"), this);
                Preferences::instance()->refreshDialog();
                Preferences::instance()->setIcon();

                this->close();
#endif
            }
        } else {
           Message::error(QObject::tr("No import file selected!"), QObject::tr("Import Configuration"), this);
        }
#endif
    }
}

void ImportWidget::cryptKey(QString password)
{
    this->cryptPassword = password;
}