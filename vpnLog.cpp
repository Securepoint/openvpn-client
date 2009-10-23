#include "vpnLog.h"
#include <QtGui>

VpnLog::VpnLog()
{
    logContent = new QTextEdit();
    cmdClose = new QPushButton ("&Close", this);
    connect(cmdClose, SIGNAL(clicked()), this, SLOT(closeDialog()));

    QVBoxLayout *messageLayout = new QVBoxLayout;
        messageLayout->addWidget(logContent);
        messageLayout->addWidget(cmdClose);
    setLayout(messageLayout);

    setWindowTitle(tr("VPN Log"));
    setWindowFlags(Qt::Dialog);
    setModal (true);
    resize(500, 500);
}

void VpnLog::closeDialog () {
    this->close();
}


