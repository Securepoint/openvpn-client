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


    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setWindowIcon(QIcon(":/images/inaktiv.svg"));
    setWindowTitle(tr("Create a new OpenVPN config"));
}

void VpnWizard::accept()
{
    QString configName = field("txtConfigName").toString();
    qDebug() << configName;
    AppFunc app;
    if (!app.isAppPortable()) {
        // Installierte Version
        // Dateien ins Homeverzeichnis/securepoint/OpenVPN kopieren


    } else {
        // Portable Version
        // Dateien ins App Verzeichnis kopieren
    }
    QDialog::accept();
}
