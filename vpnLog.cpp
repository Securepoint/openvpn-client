#include "vpnlog.h"
#include "ui_vpnlog.h"

VpnLog::VpnLog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::VpnLog)
{
    m_ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshData()));
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
    timer->stop();
    hide ();
    event->ignore();
}

void VpnLog::showDialog() {
    this->refreshData();
    this->show();
    timer->start(1000);
}

void VpnLog::refreshData() {
    m_ui->memLog->clear();
    for (int x = 0; x < logList->size(); x++) {
        m_ui->memLog->appendPlainText(logList->at(x) + "\n");
    }
}

void VpnLog::on_cmdClose_clicked()
{
    timer->stop();
    this->close();
}

void VpnLog::setContent(QString con){
    m_ui->memLog->setPlainText(con);
}

void VpnLog::on_cmdSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save log"),
                            QApplication::applicationDirPath(),
                            tr("Log files (*.log *.txt)"));
    if (fileName != "") {
        // open and write
        QFile wFile (fileName);
        if (!wFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QMessageBox::critical(0,QString ("Securepoint VPN Client"), QString("Can't open file to write!"));
            return;
        }
        QTextStream out (&wFile);
        out << m_ui->memLog->toPlainText();
        wFile.close();
    }
}
