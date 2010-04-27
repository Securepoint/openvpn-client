#include "wiz_vpnwizard.h"
#include <QtGui>

VpnWizard *VpnWizard::mInst = NULL;

VpnWizard *VpnWizard::getInstance() {
    if (!mInst)
        mInst = new VpnWizard ();
    return mInst;
}

VpnWizard::VpnWizard()
    : QWizard()
{
    addPage(new StartPage);
    addPage(new GeneralPage);
    addPage(new RemotePage);
    addPage(new CertPage);
    addPage(new AdvPage);
    addPage(new EndPage);

    setWizardStyle(ModernStyle);
    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner_wiz.png"));
    setWindowIcon(QIcon(":/images/logo.png"));
    setWindowTitle(tr("Create a new SSL VPN connection"));
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
    setModal(true);
}

void VpnWizard::accept()
{    
    QString configFilePath;
    QString dirPath;


    configFilePath = AppFunc::getInstance()->getAppSavePath()
                     + QString ("/")
                     + field("txtConfigName").toString()
                     + QString("/")
                     + field("txtConfigName").toString()
                     + QString(".ovpn");

    dirPath = AppFunc::getInstance()->getAppSavePath()
              + QString ("/")
              + field("txtConfigName").toString();

    // Ist das Verzeichnis schon da und gib es schon eine Configfile?
    QDir dirobj (dirPath);
        if (!dirobj.exists(dirPath)){
            //Verzeichnis existiert nicht
            // Pfad erstellen
            if (!dirobj.mkpath(dirPath)) {
                // Pfad konnte nicht erstellt werden
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                msgBox.setText(tr("Create a new SSL VPN connection"));
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
            QFile file(configFilePath);
            if (file.exists()) {
                // Datei existiert bereits
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
                msgBox.setText(tr("Create a new SSL VPN connection"));
                msgBox.setWindowIcon(QIcon(":/images/logo.png"));
                msgBox.setInformativeText(tr("Configfile already exists!"));
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
                msgBox.exec();
                return;
            }
            file.close();
        }

    // Verzeichnis ist da und die Datei existiert noch nicht.
    // Config erstellen und die CA, CERT und KEY Dateien kopieren
    QFile configFile (configFilePath);
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        // Datei kann nicht erstellt werden
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Create a new SSL VPN connection"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(tr("Can't create Configfile!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
        return;
    }
    //  Datei ist offen und wartet auf Daten
    QTextStream out (&configFile);
    // Ist Client
    out << "client\n";
    // Tun Device benutzen
    out << "dev tun\n";
    // MTU festlegen
    out << "tun-mtu 1500\n";
    if (field("txtMssFix").toBool())
        out << "mssfix\n";

    if (field("txtFloat").toBool())
        out << "float\n";

    if (field("txtRemoteProtocol").toInt() == 0)
        out << "proto tcp\n";
    else
        out << "proto udp\n";

    out << "remote " << field("txtRemoteIP").toString() << " " << field("txtRemotePort").toString() << "\n";

    if (field("txtNobind").toBool())
        out << "nobind\n";

    out << "persist-key\n";
    out << "persist-tun\n";

    QString caPath = field("txtCAPath").toString();
    out << "ca \"" << caPath.right(caPath.size() - caPath.lastIndexOf("/") -1) << "\"\n";

    QString certPath = field("txtCertPath").toString();
    out << "cert \"" << certPath.right(certPath.size() - certPath.lastIndexOf("/") -1) << "\"\n";

    QString keyPath = field("txtKeyPath").toString();
    out << "key \"" << keyPath.right(keyPath.size() - keyPath.lastIndexOf("/") -1) << "\"\n";

    if (field("txtServerCert").toBool())
        out << "ns-cert-type server\n";

    if (field("txtRouteMethod").toInt() != 0) {
        if (field("txtRouteMethod").toInt() == 1)
            out << "route-method exe\n";
        else
            out << "route-method ipapi\n";
    }

    if (field("txtVerbose").toString() != "")
        out << "verb " << field("txtVerbose").toString() << "\n";

    if (field("txtMute").toString() != "")
        out << "mute " << field("txtMute").toString() << "\n";

    if (field("txtRouteDelay").toString() != "")
        out << "route-delay " << field("txtRouteDelay").toString() << "\n";

    if (field("txtCompLzo").toBool())
        out << "comp-lzo\n";

    if (field("txtUserPass").toBool())
        out << "auth-user-pass\n";

    // Config ist geschrieben, Datei schließen
    configFile.close();
    // Nun die Dateien kopieren
    QFile caFile (field("txtCAPath").toString());
    QString caDestFile (dirPath + QString("/") + caPath.right(caPath.size() - caPath.lastIndexOf("/") -1));
    if (!caFile.copy(caDestFile)){
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Create a new SSL VPN connection"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(tr("Unable to copy CA file!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
        return;
    }

    QFile certFile (field("txtCertPath").toString());
    QString certDestFile (dirPath + QString("/") + certPath.right(certPath.size() - certPath.lastIndexOf("/") -1));
    if (certDestFile != caDestFile)
        if (!certFile.copy(certDestFile)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
            msgBox.setText(tr("Create a new SSL VPN connection"));
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setInformativeText(tr("Unable to copy certificate file!"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
            msgBox.exec();
            return;
        }

    QFile keyFile (field("txtKeyPath").toString());
    QString keyDestFile (dirPath + QString("/") + keyPath.right(keyPath.size() - keyPath.lastIndexOf("/") -1));
    if (keyDestFile != caDestFile && keyDestFile != certDestFile)
        if (!keyFile.copy(keyDestFile)){
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
            msgBox.setText(tr("Create a new SSL VPN connection"));
            msgBox.setWindowIcon(QIcon(":/images/logo.png"));
            msgBox.setInformativeText(tr("Unable to copy key file!"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
            msgBox.exec();
            return;
        }
    // Alles erstellt und kopiert!
    caFile.close();
    certFile.close();
    keyFile.close();
    // Refresh des Dialoges
    // Generieren eines neuen Openvpn Objektes
    // Das Objekt wird dann an die ObjList angehängt
    // Dann muss nur noch der Dialog aktualisiert werden
    /*OpenVpn *item = new OpenVpn ();
    item->configName = field("txtConfigName").toString();
    item->configPath = AppFunc::getInstance()->getAppSavePath() + QString ("/") + field("txtConfigName").toString();
    item->connectionStable = false;
    Configs::getInstance()->appendConfigToList(item);*/
    MainWindowControll::getInstance()->refreshConfigs();
    MainWindowControll::getInstance()->setConnectionStatus();

    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
    msgBox.setText(tr("Create a new SSL VPN connection"));
    msgBox.setWindowIcon(QIcon(":/images/logo.png"));
    msgBox.setInformativeText(tr("Config is successfully created!"));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    msgBox.exec();

    MainWindowControll::getInstance()->refreshDialog();
    this->close();
    QDialog::accept();
}
