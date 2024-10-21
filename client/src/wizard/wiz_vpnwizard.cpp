#include "wiz_vpnwizard.h"

//#include <WinSock2.h>
#include <Windows.h>
#include <Utils.h>

#include <message.h>

#include "wiz_startpage.h"
#include "wiz_generalpage.h"
#include "wiz_remotepage.h"
#include "wiz_endpage.h"
#include "wiz_certpage.h"
#include "wiz_advpage.h"

float windowsDpiScale();

VpnWizard::VpnWizard(QWidget * parent)
    : FramelessWizard(parent)
{
    addPage(new CStartPage);
    addPage(new GeneralPage);
    addPage(new RemotePage);
    addPage(new CertPage);
    addPage(new AdvPage);
    addPage(new CEndPage);


    //this->button(QWizard::WizardButton::BackButton)->setIcon(QIcon(":/data/images/back_small.png"));
    //this->button(QWizard::WizardButton::NextButton)->setIcon(QIcon(":/data/images/next_small.png"));


    auto geom = this->geometry();
    geom.setHeight(460);
    geom.setWidth(501);

    QSize size = geom.size();
    //
    int h = size.height();
    int w = size.width();
    //

    size.setHeight(qFloor(h * windowsDpiScale()));
    size.setWidth(qFloor(w * windowsDpiScale()));

    setFixedWidth(size.width());
    setFixedHeight(size.height());

    //
    resize(size);

    QPixmap pixmap(":/data/images/banner_wiz.png");
    pixmap = pixmap.scaledToWidth(size.width(), Qt::TransformationMode::SmoothTransformation);
    setPixmap(QWizard::BannerPixmap, pixmap);
    //page(0)->setContentsMargins(0, 20, 0, 0);
    //page(1)->setContentsMargins(0, 20, 0, 0);
    //page(2)->setContentsMargins(0, 20, 0, 0);
    //page(3)->setContentsMargins(0, 20, 0, 0);
    //page(4)->setContentsMargins(0, 20, 0, 0);
    setWindowIcon(QIcon(":/data/images/logo.png"));
    setWindowTitle(QObject::tr("Create a new SSL VPN connection"));
    setWindowFlags(Qt::WindowCloseButtonHint);
    setModal(true);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::FramelessWindowHint);
    DWORD style = GetWindowLong((HWND)winId(), GWL_STYLE);
    SetWindowLong((HWND)winId(), GWL_STYLE, style);
    setWizardStyle(ModernStyle);



    //this->button(QWizard::WizardButton::NextButton)->setLayoutDirection(Qt::RightToLeft);
    this->setButtonText(QWizard::WizardButton::NextButton, QObject::tr("Next"));
    this->setButtonText(QWizard::WizardButton::BackButton, QObject::tr("Back"));
    this->setButtonText(QWizard::WizardButton::CancelButton, QObject::tr("Cancel"));
    this->setButtonText(QWizard::WizardButton::FinishButton, QObject::tr("Finish"));

    setStyleSheet("QPushButton:hover {background-color: rgb(195, 195, 195);}\nQPushButton {;text-align:center;	\npadding-left: 3px;\n	padding-top: 3px;   padding-right: 3px;\n	padding-bottom: 3px;}");

    this->setupFrameless();
}



void VpnWizard::accept()
{
    QString configFilePath;
    QString dirPath;


    configFilePath = Utils::userApplicationDataDirectory()
                     + QLatin1String ("/config/")
                     + field("txtConfigName").toString()
                     + QLatin1String("/")
                     + field("txtConfigName").toString()
                     + QLatin1String(".ovpn");

    dirPath = Utils::userApplicationDataDirectory() + QLatin1String ("/config/") + field(QLatin1String("txtConfigName")).toString();

    // Ist das Verzeichnis schon da und gib es schon eine Configfile?
    QDir dirobj (dirPath);
        if (!dirobj.exists(dirPath)){
            //Verzeichnis existiert nicht
            // Pfad erstellen
            if (!dirobj.mkpath(dirPath)) {
                // Pfad konnte nicht erstellt werden
                Message::error(QObject::tr("Unable to create directory!"), QObject::tr("Create a new SSL VPN connection"), this);
                return;
            }
        } else {
            // Verzeichnis existiert
            QFile file(configFilePath);
            if (file.exists()) {
                // Datei existiert bereits
                Message::error(QObject::tr("Configfile already exists!"), QObject::tr("Create a new SSL VPN connection"), this);
                return;
            }
            file.close();
        }

    // Verzeichnis ist da und die Datei existiert noch nicht.
    // Config erstellen und die CA, CERT und KEY Dateien kopieren
    QFile configFile (configFilePath);
    if (!configFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        // Datei kann nicht erstellt werden
        Message::error(QObject::tr("Can't create config file!"), QObject::tr("Create a new SSL VPN connection"), this);
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

    for(const auto remote : ((RemotePage*)this->page(2))->remoteList()) {
        out << QLatin1String("remote ") << remote << QLatin1String("\n");
    }


    if (field(QLatin1String("txtNobind")).toBool()) {
        out << QLatin1String("nobind\n");
    }

    if (field(QLatin1String("txtUserPass")).toBool()) {
        out << QLatin1String("auth-user-pass\n");
    }

    out << QLatin1String("persist-key\n");
    out << QLatin1String("persist-tun\n");

    QString caPath = field(QLatin1String("txtCAPath")).toString();
    out << QLatin1String("ca \"") << caPath.right(caPath.size() - caPath.lastIndexOf("/") - 1) << QLatin1String("\"\n");

    QString certPath = field(QLatin1String("txtCertPath")).toString();
    out << QLatin1String("cert \"") << certPath.right(certPath.size() - certPath.lastIndexOf("/") - 1) << QLatin1String("\"\n");

    QString keyPath = field(QLatin1String("txtKeyPath")).toString();
    out << QLatin1String("key \"") << keyPath.right(keyPath.size() - keyPath.lastIndexOf("/") - 1) << QLatin1String("\"\n");

    if (field(QLatin1String("txtServerCert")).toBool()) {
        out << QLatin1String("remote-cert-tls server\n");
    }

    if (field(QLatin1String("txtRouteMethod")).toInt() != 0) {
        if (field(QLatin1String("txtRouteMethod")).toInt() == 1) {
            out << QLatin1String("route-method exe\n");
        } else {
            out << QLatin1String("route-method ipapi\n");
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

    // Config ist geschrieben, Datei schlie?en
    configFile.close();
    // Nun die Dateien kopieren
    QFile caFile (field(QLatin1String("txtCAPath")).toString());
    QString caDestFile (dirPath + QLatin1String("/") + caPath.right(caPath.size() - caPath.lastIndexOf("/") -1));
    if (!caFile.copy(caDestFile)){
        Message::error(QObject::tr("Unable to copy CA file!"), QObject::tr("Create a new SSL VPN connection"), this);
        return;
    }

    QFile certFile (field(QLatin1String("txtCertPath")).toString());
    QString certDestFile (dirPath + QLatin1String("/") + certPath.right(certPath.size() - certPath.lastIndexOf("/") -1));
    if (certDestFile != caDestFile) {
        if (!certFile.copy(certDestFile)){
            Message::error(QObject::tr("Unable to copy certificate file!"), QObject::tr("Create a new SSL VPN connection"), this);
            return;
        }
    }

    QFile keyFile (field(QLatin1String("txtKeyPath")).toString());
    QString keyDestFile (dirPath + QLatin1String("/") + keyPath.right(keyPath.size() - keyPath.lastIndexOf("/") -1));
    if (keyDestFile != caDestFile && keyDestFile != certDestFile) {
        if (!keyFile.copy(keyDestFile)){
            Message::error(QObject::tr("Unable to copy key file!"), QObject::tr("Create a new SSL VPN connection"), this);
            return;
        }
    }
    // Alles erstellt und kopiert!
    caFile.close();
    certFile.close();
    keyFile.close();
    // Refresh des Dialoges
    // Generieren eines neuen Openvpn Objektes
    // Das Objekt wird dann an die ObjList angeh?ngt
    // Dann muss nur noch der Dialog aktualisiert werden
   // Preferences::instance()->addNewConfigToDatabase(field("txtConfigName").toString(), configFilePath);
   // Preferences::instance()->refreshConfigList();
   // Preferences::instance()->setConnectionStatus();

   // Preferences::instance()->refreshDialog();
   // Preferences::instance()->setIcon();
    this->close();
    QDialog::accept();
}
