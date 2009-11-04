#include "tapinfo.h"
#include "ui_tapinfo.h"
#ifdef Q_OS_WIN32
    #include "../../../../mingw/include/windows.h"
    #include "../../../../mingw/include/winsock2.h"
    #include "../../../../mingw/include/iphlpapi.h"
#endif
TapInfo::TapInfo(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::TapInfo)
{
    m_ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(refreshTraffic()));
}

void TapInfo::openDialog() {
    timer->start(1000);
    lastPc = 0;
    lastPcOut = 0;
    timerNewStarted = true;
    this->show();
}

TapInfo::~TapInfo()
{
    delete m_ui;
}

void TapInfo::changeEvent(QEvent *e)
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

void TapInfo::closeEvent(QCloseEvent *event) {
    timer->stop();
    hide ();
    event->ignore();
}

void TapInfo::refreshTraffic() {
    #ifdef Q_OS_WIN32
        DWORD dwSize = 0;
        DWORD dwRetVal = 0;

        int i;

        MIB_IFTABLE *pIfTable;
        MIB_IFROW *pIfRow;

        pIfTable = (MIB_IFTABLE *) malloc(sizeof (MIB_IFTABLE));
        if (pIfTable == NULL) {
            QMessageBox::critical(0, QString("Securepoint VPN Client") , QString ("Error allocating memory needed to call GetIfTable"));
            return;
        }

        dwSize = sizeof (MIB_IFTABLE);
        if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
            free(pIfTable);
            pIfTable = (MIB_IFTABLE *) malloc(dwSize);
            if (pIfTable == NULL) {
                QMessageBox::critical(0, QString("Securepoint VPN Client") , QString ("Error allocating memory needed to call GetIfTable"));
                return;
            }
        }

        if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
            for (i = 0; i < (int) pIfTable->dwNumEntries; i++) {
                pIfRow = (MIB_IFROW *) & pIfTable->table[i];
                if (QString::fromUtf8((const char*)pIfRow->bDescr).contains("TAP-Win32 Adapter")) {
                    m_ui->lblIncommingTotal->setText(QString::number(pIfRow->dwInUcastPkts));
                    m_ui->lblOutgoingTotal->setText(QString::number(pIfRow->dwOutUcastPkts));
                    if (!timerNewStarted) {
                        m_ui->lblIncommingPerSecond->setText(QString::number((pIfRow->dwInUcastPkts-lastPc)));
                        m_ui->lblOutgoingPerSecond->setText(QString::number((pIfRow->dwOutUcastPkts-lastPcOut)));
                    }
                    timerNewStarted=false;
                    lastPc = pIfRow->dwInUcastPkts;
                    lastPcOut = pIfRow->dwOutUcastPkts;
                }
            }
        } else {
            if (pIfTable != NULL) {
                free(pIfTable);
                pIfTable = NULL;
            }
            return;
        }
        if (pIfTable != NULL) {
            free(pIfTable);
            pIfTable = NULL;
        }
    #endif
}

void TapInfo::on_cmdClose_clicked()
{
    this->close();
}
