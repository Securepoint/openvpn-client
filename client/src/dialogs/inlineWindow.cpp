#include "inlineWindow.h"
#include "ui_inlineWindow.h"

#include "message.h"
#include <service/servicelogdata.h>

#include <QFileDialog>
#include <QtGui/qguiapplication.h>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollbar>
#include "widgets/settings/config/advconfigsettingswidget.h"

InlineWindow::InlineWindow(QString tag, QString cert, QString* p_cert, AdvConfigSettingsWidget* p_parent) :
    FramelessDialog(),
    m_ui(new Ui::InlineWindow),
    tag(tag),
    cert(cert),
    p_cert(p_cert),
    p_parent(p_parent)
{
    this->setWindowFlags(Qt::Tool);

    m_ui->setupUi((QDialog*)this->getWrapperWidget());

    QMetaObject::connectSlotsByName(this);

    this->setupFrameless();
    this->postSetupFrameless();

    connect(this, SIGNAL(sendSave()), p_parent, SLOT(setPathInline()));

}

InlineWindow::~InlineWindow()
{
    delete m_ui;
}

void InlineWindow::changeEvent(QEvent *e)
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

void InlineWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void InlineWindow::showEvent(QShowEvent *e)
{
    m_ui->memLog->clear();
    this->getCert(cert);

    int winW = this->width();
    int winH = this->height();

    int left (0);
    int top (0);
    QRect geom (0,0,0,0);

    {
        // Desktop auswerten
        geom = qApp->primaryScreen()->geometry();
        top = geom.height();
        left = geom.width();
        // Breite
        left = (left - winH) / 2;
        // Height
        top = (top - winH) / 2;
    }
    // Nun die neuen setzen
    this->setGeometry(left, top, winW, winH);

    e->accept();
}

void InlineWindow::on_cmdSave_clicked()
{
        // get text from window
        QString text = m_ui->memLog->toPlainText();

        // send inline text to advConfig Window
        *p_cert = text;

        // set path inline
        emit sendSave();

        //close
        this->close();
}

void InlineWindow::on_cmdClose_clicked()
{
    this->close();
}

void InlineWindow::getCert(QString cert)
{
    m_ui->memLog->appendPlainText(cert);
}
