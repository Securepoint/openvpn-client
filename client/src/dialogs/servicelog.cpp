#include "servicelog.h"
#include "ui_servicelog.h"

#include "message.h"
#include <service/servicelogdata.h>

#include <QFileDialog>
#include <QtGui/qguiapplication.h>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QScrollbar>
#include <QScreen>

ServiceLog::ServiceLog(int id) :
    FramelessDialog(),
    m_ui(new Ui::ServiceLog),
    openvpnId(id)
{
    this->setWindowFlags(Qt::Tool);

    m_ui->setupUi((QDialog*)this->getWrapperWidget());

    QMetaObject::connectSlotsByName(this);

    this->setupFrameless();
    this->postSetupFrameless();

    // Timer für die Aktualisierung aktualisieren
    this->timer = new QTimer(this);
    this->timer->setInterval(1000);
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(refreshData()));
}

ServiceLog::~ServiceLog()
{
    delete m_ui;
}

void ServiceLog::changeEvent(QEvent *e)
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

void ServiceLog::on_cmdStopLog_clicked()
{
    if (this->timer->isActive()) {
        this->timer->stop();
        m_ui->cmdStopLog->setText(QObject::tr("Start Log"));
    } else {
        this->timer->start();
        m_ui->cmdStopLog->setText(QObject::tr("Stop Log"));
    }
}

void ServiceLog::closeEvent(QCloseEvent *event)
{
    this->timer->stop();
    event->accept();
}

static QString lastText = "";

void ServiceLog::showEvent(QShowEvent *e)
{
    m_ui->memLog->clear();

    lastText = "";

    this->refreshData();
    this->timer->start();

    m_ui->cmdStopLog->setText(QObject::tr("Stop Log"));

    int winW = this->width();
    int winH = this->height();

    int left (0);
    int top (0);
    QRect geom (0,0,0,0);

    // TODO: get FrmMain pos
    //if (Preferences::instance()->isVisible()) {
    //    // Wenn das Hauptfenster offen ist mittig über diesem plazieren
    //    left = Preferences::instance()->geometry().x();
    //    top = Preferences::instance()->geometry().y();
    //    left = left + (Preferences::instance()->geometry().width() - winW) / 2;
    //    //
    //    top  = top + (Preferences::instance()->geometry().height() - winH) / 2;
    //} else
    {
        // Desktop auswerten
        geom = qApp->primaryScreen()->geometry();
        top = geom.height();
        left = geom.width();

        // wenn ich das richtig verstanden habe muss man die virtuellen desktops nicht umrechnen

        // Die Breite bei virtuellen Desktops vierteln
        //if (left > 2000 && qApp->desktop()->isVirtualDesktop()) {
        //    left /= 4;
        //} else {
            // Normaler Desktop
            left = (left - winH) / 2;
        //}
        // Height
       // if (top > 2000 && qApp->desktop()->isVirtualDesktop()) {
       //     top /= 4;
       // } else {
            top = (top - winH) / 2;
       // }
    }
    // Nun die neuen setzen
    this->setGeometry(left, top, winW, winH);

    e->accept();
}

void ServiceLog::on_cmdSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Save Log"), QApplication::applicationDirPath(), QObject::tr("Log files (*.log *.txt)"));

    if (!fileName.isEmpty()) {
        // open and write
        QFile wFile (fileName);
        if (!wFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            Message::error(QObject::tr("Can't open file to write!"));
            return;
        }
        QTextStream out (&wFile);
        out << m_ui->memLog->toPlainText();
        wFile.close();
    }
}

void ServiceLog::refreshData()
{
    this->timer->stop();

    auto text = ServiceLogData::instance()->logs(this->openvpnId).join("");


    if(lastText != text)
    {
        m_ui->memLog->clear();

        lastText = text;

        m_ui->memLog->appendPlainText(text);
        // Da das TextEdit readonly ist, nehmen wir die scrollbar für den auto scroll. ansonsten ist der cursor besser
        QScrollBar *sb = m_ui->memLog->verticalScrollBar();
        sb->setValue(sb->maximum());
    }

    this->timer->start();
}

void ServiceLog::on_cmdClose_clicked()
{
    timer->stop();
    this->close();
}


void ServiceLog::on_cmdClearLog_clicked()
{
    ServiceLogData::instance()->clearId(this->openvpnId);
    m_ui->memLog->clear();
}
