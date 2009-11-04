#include "renameconfig.h"
#include "ui_renameconfig.h"

RenameConfig::RenameConfig(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::RenameConfig)
{
    m_ui->setupUi(this);
}

RenameConfig::~RenameConfig()
{
    delete m_ui;
}

void RenameConfig::changeEvent(QEvent *e)
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

void RenameConfig::on_cmdClose_clicked()
{
    this->close();
}

void RenameConfig::on_cmdRename_clicked()
{
    bool isLinked = false;
    Configs myConfig;
    if (m_ui->txtNewName->text().trimmed() != "") {
        QFile renFile (this->fullPath);
        QString oldPath (this->fullPath);
                oldPath = oldPath.replace("\\","/");
        if (myConfig.isConfigLinked(oldPath))
            isLinked = true;
        if (renFile.exists()) {
            // rename
            QString renPath (this->fullPath.left(this->fullPath.lastIndexOf("/")));
            if (!renFile.rename(renPath + QString("/") + m_ui->txtNewName->text().trimmed() + QString(".ovpn"))) {
                QMessageBox::critical(0, QString("Securepoint VPN Client"), QString ("Rename failed!"));
                return;
            }
            QMessageBox::information(0, QString("Securepoint VPN Client"), QString("Rename was successfully!"));
        }
        if (isLinked) {
            myConfig.changeConfigNameInLinkedList(oldPath, oldPath.left(oldPath.lastIndexOf("/")) + QString("/") + m_ui->txtNewName->text().trimmed() + QString(".ovpn"));
        }
        Preferences *prefDialog = dynamic_cast<Preferences*> (this->parent());
        prefDialog->refreshConfigList();
        this->close();
    } else {
        QMessageBox::critical(0, QString("Securepoint VPN Client"), QString ("New name is empty!"));
    }
}

void RenameConfig::setOldName(QString oldname) {
    m_ui->lblOldName->setText(oldname);
}

void RenameConfig::setFullPath(QString path) {
    this->fullPath = path;
}
