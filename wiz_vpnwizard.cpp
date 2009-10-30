#include "wiz_vpnwizard.h"
#include <QtGui>

VpnWizard::VpnWizard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new StartPage);
    addPage(new GeneralPage);
    addPage(new RemotePage);
    addPage(new CertPage);
    addPage(new AdvPage);
    addPage(new EndPage);


    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner_wiz.png"));
    setWindowIcon(QIcon(":/images/appicon.png"));
    setWindowTitle(tr("Create a new VPN connection"));
    setModal(true);
}

void VpnWizard::accept()
{
    AppFunc app;
    QString configFilePath;
    QString dirPath;


    configFilePath = app.getAppSavePath()
                     + QString ("/")
                     + field("txtConfigName").toString()
                     + QString("/")
                     + field("txtConfigName").toString()
                     + QString(".ovpn");

    dirPath = app.getAppSavePath()
              + QString ("/")
              + field("txtConfigName").toString();

    // Ist das Verzeichnis schon da und gib es schon eine Configfile?
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
            QFile file(configFilePath);
            if (file.exists()) {
                // Datei existiert bereits
                QMessageBox::critical(0,"Securepoint VPN Client", "Configfile already exists!");
                return;
            }
            file.close();
        }

    // Verzeichnis ist da und die Datei existiert noch nicht.
    // Config erstellen und die CA, CERT und KEY Dateien kopieren
    QFile configFile (configFilePath);
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        // Datei kann nicht erstellt werden
        QMessageBox::critical(0,"Securepoint VPN Client", "Can't create Configfile!");
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

    if (field("txtRouteMethod").toInt() != 0)
        if (field("txtRouteMethod").toInt() == 1)
            out << "route-method exe\n";
        else
            out << "route-method ipapi\n";

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
    if (!caFile.copy(dirPath + QString("/") + caPath.right(caPath.size() - caPath.lastIndexOf("/") -1))){
        QMessageBox::critical(0,"Securepoint VPN Client", "Unable to copy CA file!");
        return;
    }

    QFile certFile (field("txtCertPath").toString());
    if (!certFile.copy(dirPath + QString("/") + certPath.right(certPath.size() - certPath.lastIndexOf("/") -1))){
        QMessageBox::critical(0,"Securepoint VPN Client", "Unable to copy certificate file!");
        return;
    }

    QFile keyFile (field("txtKeyPath").toString());
    if (!keyFile.copy(dirPath + QString("/") + keyPath.right(keyPath.size() - keyPath.lastIndexOf("/") -1))){
        QMessageBox::critical(0,"Securepoint VPN Client", "Unable to copy key file!");
        return;
    }
    // Alles erstellt und kopiert!
    caFile.close();
    certFile.close();
    keyFile.close();
    Preferences *prefDialog = dynamic_cast<Preferences*> (this->parent());
    prefDialog->refreshConfigList();

    QMessageBox::information(0,"Securepoint VPN Client", "Config is successfully created!");
    this->close();


    QDialog::accept();
}
