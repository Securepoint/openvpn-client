#include "sslcertsdialog.h"
#include "ui_sslcertsdialog.h"
#include <QtGui>
#include <QtGui/qguiapplication.h>
#include <QtWidgets/QDesktopWidget>

float windowsDpiScale();

SSLDialog::SSLDialog(const QString &certificateInfo, QWidget * parent)
    : FramelessDialog(parent),
    ui(new Ui::SSLDialog),
    certificateAcceptState(false)
{
   
    // 0 - Username
    // 1 - Pwd
    // 2 - OTP
    // 3 - PKCS12
    // 4 - Private Key f�r Crypted User Data

    ui->setupUi((QDialog*)this->getWrapperWidget());  
    //
    this->setLayout(ui->gridLayout);

    //
    ui->plainTextEdit->appendHtml(certificateInfo);

    QMetaObject::connectSlotsByName(this);

    auto geom = this->getWrapperWidget()->geometry();
    auto size = geom.size() * windowsDpiScale();
    //geom = geometry();
    geom.setSize(size);
    this->getWrapperWidget()->setGeometry(geom);

    this->setupFrameless();
}

SSLDialog::~SSLDialog()
{
    delete ui;
}

bool SSLDialog::isCertificateAccepted()
{
    return this->certificateAcceptState;
}

void SSLDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SSLDialog::receivedCloseMe()
{

}

void SSLDialog::closeEvent(QCloseEvent *e)
{
    e->accept();
}

void SSLDialog::showEvent(QShowEvent *e) {
    int winW = this->width();
    int winH = this->height();

    int left (0);
    int top (0);

    {
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

    // �ffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
}

void SSLDialog::on_CmdYes_clicked()
{
    this->certificateAcceptState = true;
    //
    this->close();
}

void SSLDialog::on_cmdNo_clicked()
{
    this->certificateAcceptState = false;
    //
    this->close();
}
