#include "deleteconfig.h"
#include "ui_deleteconfig.h"

DeleteConfig::DeleteConfig(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::DeleteConfig)
{
    m_ui->setupUi(this);
}

DeleteConfig::~DeleteConfig()
{
    delete m_ui;
}

void DeleteConfig::changeEvent(QEvent *e)
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

void DeleteConfig::on_cmdCancel_clicked()
{
    this->close();
}

void DeleteConfig::on_cmdDelete_clicked()
{
    Preferences *prefDialog = dynamic_cast<Preferences*> (this->parent());
    prefDialog->deleteConfigFromList(m_ui->cbConfigFile->isChecked(),
                                     m_ui->cbCA->isChecked(),
                                     m_ui->cbCert->isChecked(),
                                     m_ui->cbKey->isChecked(),
                                     m_ui->cbUserdata->isChecked(),
                                     m_ui->cbScript->isChecked(),
                                     m_ui->cbDir->isChecked());
    this->close();
}

void DeleteConfig::openDialog() {
    this->show();
    m_ui->cbCA->setChecked(true);
    m_ui->cbCert->setChecked(true);
    m_ui->cbConfigFile->setChecked(true);
    m_ui->cbDir->setChecked(true);
    m_ui->cbKey->setChecked(true);
    m_ui->cbUserdata->setChecked(true);
    m_ui->cbScript->setChecked(true);
}

