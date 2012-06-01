#include <QtGui>
#include "vpnwizzard.h"

VpnWizzard::VpnWizzard(QWidget *parent)
    : QWizard(parent)
{
    addPage(new StartPage);
    addPage(new GeneralPage);
    addPage(new RemotePage);
    addPage(new EndPage);

    setPixmap(QWizard::BannerPixmap, QPixmap(":/images/banner.png"));
    setWindowIcon(QIcon(":/images/inaktiv.svg"));
    setWindowTitle(tr("Create a new OpenVPN config"));
}

void VpnWizzard::accept()
{
    QString configName = field("txtConfigName").toString();
    qDebug() << configName;
    QDialog::accept();
}
