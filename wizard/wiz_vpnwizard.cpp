#include "wiz_vpnwizard.h"
#include "preferences.h"

#include "wiz_startpage.h"
#include "wiz_generalpage.h"
#include "wiz_remotepage.h"
#include "wiz_endpage.h"
#include "wiz_certpage.h"
#include "wiz_advpage.h"
//
#include "appfunc.h"
#include "openvpn.h"
#include "Configs.h"

#include "message.h"


VpnWizard::VpnWizard()    
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
    setWindowTitle(QObject::tr("Create a new SSL VPN connection"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    setModal(true);
}

void VpnWizard::accept()
{    
    QString configFilePath;
    QString dirPath;


    configFilePath = AppFunc::getAppSavePath()
                     + QLatin1String ("/")
                     + field("txtConfigName").toString()
                     + QLatin1String("/")
                     + field("txtConfigName").toString()
                     + QLatin1String(".ovpn");

    dirPath = AppFunc::getAppSavePath() + QLatin1String ("/") + field(QLatin1String("txtConfigName")).toString();

    // Ist das Verzeichnis schon da und gib es schon eine Configfile?
    QDir dirobj (dirPath);
        if (!dirobj.exists(dirPath)){
            //Verzeichnis existiert nicht
            // Pfad erstellen
            if (!dirobj.mkpath(dirPath)) {
                // Pfad konnte nicht erstellt werden
                Message::error(QObject::tr("Unable to create directory!"), QObject::tr("Create a new SSL VPN connection"));
                return;
            }
        } else {
            // Verzeichnis existiert
            QFile file(configFilePath);
            if (file.exists()) {
                // Datei existiert bereits
                Message::error(QObject::tr("Configfile already exists!"), QObject::tr("Create a new SSL VPN connection"));
                return;
            }
            file.close();
        }

    // Verzeichnis ist da und die Datei existiert noch nicht.
    // Config erstellen und die CA, CERT und KEY Dateien kopieren
    QFile configFile (configFilePath);
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        // Datei kann nicht erstellt werden
        Message::error(QObject::tr("Can't create config file!"), QObject::tr("Create a new SSL VPN connection"));
        return;
    }

    //  Datei ist offen und wartet auf Daten
    QTextStream out (&configFile);
    // Ist Client
    out << QLatin1String("client\n");
    // Tun Device benutzen
    out << QLatin1String("dev tun\n");
    // MTU festlegen
    out << QLatin1String("tun-mtu 1500\n");
    if (field(QLatin1String("txtMssFix")).toBool()) {
        out << QLatin1String("mssfix\n");
    }

    if (field(QLatin1String("txtFloat")).toBool()) {
        out << QLatin1String("float\n");
    }

    if (field(QLatin1String("txtRemoteProtocol")).toInt() == 0) {
        out << QLatin1String("proto tcp\n");
    } else {
        out << QLatin1String("proto udp\n");
    }

    out << QLatin1String("remote ") << field(QLatin1String("txtRemoteIP")).toString() << QLatin1String(" ") << field(QLatin1String("txtRemotePort")).toString() << QLatin1String("\n");

    if (field(QLatin1String("txtNobind")).toBool())
        out << QLatin1String("nobind\n");

    out << QLatin1String("persist-key\n");
    out << QLatin1String("persist-tun\n");

    QString caPath = field(QLatin1String("txtCAPath")).toString();
    out << QLatin1String("ca \"") << caPath.right(caPath.size() - caPath.lastIndexOf("/") - 1) << QLatin1String("\"\n");

    QString certPath = field(QLatin1String("txtCertPath")).toString();
    out << QLatin1String("cert \"") << certPath.right(certPath.size() - certPath.lastIndexOf("/") - 1) << QLatin1String("\"\n");

    QString keyPath = field(QLatin1String("txtKeyPath")).toString();
    out << QLatin1String("key \"") << keyPath.right(keyPath.size() - keyPath.lastIndexOf("/") - 1) << QLatin1String("\"\n");

    if (field(QLatin1String("txtServerCert")).toBool())
        out << QLatin1String("ns-cert-type server\n");

    if (field(QLatin1String("txtRouteMethod")).toInt() != 0) {
        if (field(QLatin1String("txtRouteMethod")).toInt() == 1) {
            out << QLatin1String("route-method exe\n");
        } else {
            out << QLatin1String("route-method ipapi\n");
        }
    }

    // Das Windir auswerten
    if (!field(QLatin1String("cbDefault")).toBool()) {
        // Custom path oder Env
        if (field(QLatin1String("cbEnvironment")).toBool()) {
            out << QLatin1String("win-sys env") << QLatin1String("\n");
        } else {
            // Sind Leerzeichen im String
            QString winDir (field(QLatin1String("txtPath")).toString());
            winDir = winDir.trimmed();
            if (winDir.indexOf(" ") > -1 ) {
                winDir = QLatin1String("\"") + winDir + QLatin1String("\"");
            }
            out << QLatin1String("win-sys ") << winDir << QLatin1String("\n");
        }
    }

    if (!field(QLatin1String("txtVerbose")).toString().isEmpty())
        out << QLatin1String("verb ") << field(QLatin1String("txtVerbose")).toString() << QLatin1String("\n");

    if (!field(QLatin1String("txtMute")).toString().isEmpty())
        out << QLatin1String("mute ") << field(QLatin1String("txtMute")).toString() << QLatin1String("\n");

    if (!field(QLatin1String("txtRouteDelay")).toString().isEmpty())
        out << QLatin1String("route-delay ") << field(QLatin1String("txtRouteDelay")).toString() << QLatin1String("\n");

    if (field(QLatin1String("txtCompLzo")).toBool())
        out << QLatin1String("comp-lzo\n");

    if (field(QLatin1String("txtUserPass")).toBool())
        out << QLatin1String("auth-user-pass\n");

    // Config ist geschrieben, Datei schließen
    configFile.close();
    // Nun die Dateien kopieren
    QFile caFile (field(QLatin1String("txtCAPath")).toString());
    QString caDestFile (dirPath + QLatin1String("/") + caPath.right(caPath.size() - caPath.lastIndexOf("/") -1));
    if (!caFile.copy(caDestFile)){
        Message::error(QObject::tr("Unable to copy CA file!"), QObject::tr("Create a new SSL VPN connection"));
        return;
    }

    QFile certFile (field(QLatin1String("txtCertPath")).toString());
    QString certDestFile (dirPath + QLatin1String("/") + certPath.right(certPath.size() - certPath.lastIndexOf("/") -1));
    if (certDestFile != caDestFile) {
        if (!certFile.copy(certDestFile)){
            Message::error(QObject::tr("Unable to copy certificate file!"), QObject::tr("Create a new SSL VPN connection"));
            return;
        }
    }

    QFile keyFile (field(QLatin1String("txtKeyPath")).toString());
    QString keyDestFile (dirPath + QLatin1String("/") + keyPath.right(keyPath.size() - keyPath.lastIndexOf("/") -1));
    if (keyDestFile != caDestFile && keyDestFile != certDestFile) {
        if (!keyFile.copy(keyDestFile)){
            Message::error(QObject::tr("Unable to copy key file!"), QObject::tr("Create a new SSL VPN connection"));
            return;
        }
    }
    // Alles erstellt und kopiert!
    caFile.close();
    certFile.close();
    keyFile.close();
    // Refresh des Dialoges
    // Generieren eines neuen Openvpn Objektes
    // Das Objekt wird dann an die ObjList angehängt
    // Dann muss nur noch der Dialog aktualisiert werden
    Preferences::instance()->refreshConfigList();
    Preferences::instance()->setConnectionStatus();

    Message::information(QObject::tr("Config is successfully created!"), QObject::tr("Create a new SSL VPN connection"));

    Preferences::instance()->refreshDialog();
    Preferences::instance()->setIcon();
    this->close();
    QDialog::accept();
}
