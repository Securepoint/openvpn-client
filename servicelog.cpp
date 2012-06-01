#include "servicelog.h"
#include "ui_servicelog.h"

#include "message.h"
#include "servicelogdata.h"
#include "preferences.h"

ServiceLog::ServiceLog(int id) :
    QDialog(),
    m_ui(new Ui::ServiceLog),
    openvpnId(id)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);
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

void ServiceLog::showEvent(QShowEvent *e)
{
    this->refreshData();
    this->timer->start();

    m_ui->cmdStopLog->setText(QObject::tr("Stop Log"));

    int winW = this->width();
    int winH = this->height();

    int left (0);
    if (Preferences::instance()->isVisible()) {
        // Wenn das Hauptfenster offen ist mittig über diesem plazieren
        left = Preferences::instance()->geometry().x();
        left = left + (Preferences::instance()->geometry().width() - winW) / 2;
    } else {
        // Desktop auswerten
        left = qApp->desktop()->width();
        // Die Breite bei virtuellen Desktops vierteln
        if (left > 2000 && qApp->desktop()->isVirtualDesktop()) {
            left /= 4;
        }
    }

    // Nun die neuen setzen
    this->setGeometry(left, (qApp->desktop()->height() / 2) - (winH / 2), winW, winH);

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

    m_ui->memLog->clear();
    m_ui->memLog->appendPlainText(ServiceLogData::instance()->logs(this->openvpnId).join(""));
    // Da das TextEdit readonly ist, nehmen wir die scrollbar für den auto scroll. ansonsten ist der cursor besser
    QScrollBar *sb = m_ui->memLog->verticalScrollBar();
    sb->setValue(sb->maximum());

    this->timer->start();
}

void ServiceLog::on_cmdClose_clicked()
{
    timer->stop();
    this->close();
}

