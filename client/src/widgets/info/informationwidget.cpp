#include "informationwidget.h"
#include "ui_informationwidget.h"

#include <frmmain.h>
#include <widgetfactory.h>
#include <QtGui/qpainter.h>
#include <userinfo.h>

InformationWidget::InformationWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InformationWidget)
{
    ui->setupUi(this);

    QObject::connect(ui->cmdYT, &QPushButton::clicked, []() {
       QDesktopServices::openUrl(QUrl("http://youtube.com/Securepoint10/"));
    });

    QObject::connect(ui->cmdFB, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://www.facebook.com/pages/Securepoint-GmbH/132451210137395"));
    });

    QObject::connect(ui->cmdTwitter, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("https://twitter.com/SecurepointStat"));
    });

    QObject::connect(ui->cmdForum, &QPushButton::clicked, []() {
        QDesktopServices::openUrl(QUrl("http://support.securepoint.de/"));
    });

    ui->lblUserInfo->setText(QObject::tr("SSLVPN: %1")
                             .arg(UserInfo::instance()->currentThreadUser()));
    ui->lblLogonUserInfo->setText(QObject::tr("Logon: %1")
                             .arg(UserInfo::instance()->currentLogonUser()));
}

InformationWidget::~InformationWidget()
{
    delete ui;
}

void InformationWidget::on_cmdBack_clicked()
{
    //
    // Close me
    //

    FrmMain::instance()->mainWidget()->showWidget(MainView);
}

void InformationWidget::paintEvent(QPaintEvent *pe)
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
