#include "vpnlog.h"
#include "ui_vpnlog.h"

#include "message.h"
#include "preferences.h"

VpnLog::VpnLog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::VpnLog)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);
    this->timer = new QTimer(this);
    this->timer->setInterval(1000);
    QObject::connect(this->timer, SIGNAL(timeout()), this, SLOT(refreshData()));
}

VpnLog::~VpnLog()
{
    delete m_ui;
}

void VpnLog::changeEvent(QEvent *e)
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

void VpnLog::closeEvent(QCloseEvent *event) {
    this->timer->stop();
    hide ();
    event->ignore();
}

void VpnLog::showDialog() {
    this->refreshData();

    int winW = this->width();
    int winH = this->height();

    int left (0);
    int top (0);
    if (Preferences::instance()->isVisible()) {
        // Wenn das Hauptfenster offen ist mittig über diesem plazieren
        left = Preferences::instance()->geometry().x();
        top = Preferences::instance()->geometry().y();
        left = left + (Preferences::instance()->geometry().width() - winW) / 2;
        //
        top  = top + (Preferences::instance()->geometry().height() - winH) / 2;
    } else {
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

    this->show();

    this->timer->start();

    m_ui->cmdStartStopLog->setText(QObject::tr("Stop Log"));
}

void VpnLog::refreshData() {
    m_ui->memLog->clear();

    for (int x = 0, llsize = logList->size(); x < llsize; x++) {
        m_ui->memLog->appendPlainText(logList->at(x) + "\n");
    }

    // Da das TextEdit readonly ist, nehmen wir die scrollbar für den auto scroll. ansonsten ist der cursor besser
    QScrollBar *sb = m_ui->memLog->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void VpnLog::on_cmdClose_clicked()
{
    timer->stop();
    this->close();
}

void VpnLog::setContent(const QString &con)
{
    m_ui->memLog->setPlainText(con);
}

void VpnLog::on_cmdSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("Save log"), QApplication::applicationDirPath(), QObject::tr("Log files (*.log *.txt)"));

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

void VpnLog::on_cmdStartStopLog_clicked()
{
    if (timer->isActive()) {
        timer->stop();
        m_ui->cmdStartStopLog->setText(QObject::tr("Start Log"));
    } else {
        timer->start();
        m_ui->cmdStartStopLog->setText(QObject::tr("Stop Log"));
    }
}
