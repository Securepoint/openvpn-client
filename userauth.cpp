#include "userauth.h"
#include <QtGui>

UserAuth::UserAuth()
{
    // Building authentication window
    setWindowTitle(tr("VPN Authentication"));
    setWindowFlags(Qt::Dialog);
    setModal (true);
    // Adding widgets
    authGroupBox = new QGroupBox(tr("Authentication:"));
    // Labels
    labelUser = new QLabel(tr("Username:"));
    labelPwd = new QLabel(tr("Password:"));
    // LineEdits
    txtPwd = new QLineEdit();
    txtUser = new QLineEdit();
    // Buttons
    cmdOK = new QPushButton ("&OK", this);
    cmdCancel = new QPushButton ("&Cancel", this);
    // Create new layout
    QGridLayout *authGrid = new QGridLayout();
    // Adding widgets
    authGrid->addWidget(labelUser,0,0);
    authGrid->addWidget(labelPwd,1,0);
    authGrid->addWidget(txtUser,0,1);
    authGrid->addWidget(txtPwd,1,1);
    authGrid->addWidget(cmdOK,2,0);
    authGrid->addWidget(cmdCancel,2,1);

    authGroupBox->setLayout(authGrid);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(authGroupBox);

    
    // Set layout to window
    this->setLayout(mainLayout);
    // Set slots
    connect(cmdOK, SIGNAL(clicked()), this, SLOT(getAuth()));
    connect(cmdCancel, SIGNAL(clicked()), this, SLOT(closeDialog()));
    // Resize
    resize(200, 100);
}

void UserAuth::clearFields() {
    this->txtPwd->setText("");
    this->txtUser->setText("");
}


void UserAuth::closeDialog () {
    this->close();
}

void UserAuth::getAuth() {
    this->parentVpn->configUser = this->txtUser->text();
    this->parentVpn->configPwd = this->txtPwd->text();
    this->parentVpn->connectToVpn();
    this->closeDialog();
}



