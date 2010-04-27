#include "servicelog.h"
#include "ui_servicelog.h"

ServiceLog *ServiceLog::mInst = NULL;

ServiceLog *ServiceLog::getInstance() {
    if (!mInst)
        mInst = new ServiceLog ();
    return mInst;
}

ServiceLog::ServiceLog() :
    QDialog(),
    m_ui(new Ui::ServiceLog)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);
    // Timer für die Aktualisierung aktualisieren
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshData()));
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
    if (timer->isActive()) {
        timer->stop();
        m_ui->cmdStopLog->setText(tr("Start Log"));
    } else {
        timer->start(1000);
        m_ui->cmdStopLog->setText(tr("Stop Log"));
    }
}

void ServiceLog::closeEvent(QCloseEvent *event) {
    timer->stop();
    event->accept();
}

void ServiceLog::showEvent(QShowEvent *e) {
    this->refreshData();
    timer->start(1000);
    m_ui->cmdStopLog->setText(tr("Stop Log"));
    // Mittig ausrichten
    int screenH = qApp->desktop()->height();
    int screenW = qApp->desktop()->width();
    int winH = 430;
    int winW = 600;
    // Nun die neuen setzen
    this->setGeometry((screenW / 2) - (winW / 2), (screenH / 2) - (winH / 2), winW, winH);
    e->accept();
}

void ServiceLog::on_cmdSave_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Log"),
                            QApplication::applicationDirPath(),
                            tr("Log files (*.log *.txt)"));
    if (fileName != "") {
        // open and write
        QFile wFile (fileName);
        if (!wFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
            QMessageBox::critical(0,QString (tr("Securepoint SSL VPN")), QString(tr("Can't open file to write!")));
            return;
        }
        QTextStream out (&wFile);
        out << m_ui->memLog->toPlainText();
        wFile.close();
    }
}

void ServiceLog::refreshData() {
    this->m_ui->memLog->clear();
    for (int x = 0; x < this->srvLog.size(); x++) {
        this->m_ui->memLog->appendPlainText(this->srvLog.at(x) + "\n");
    }
}

void ServiceLog::on_cmdClose_clicked()
{
    timer->stop();
    this->close();
}

void ServiceLog::appendToLog(QString message) {
    this->srvLog.append(message);
}

void ServiceLog::clearLog() {
    this->srvLog.clear();
}
