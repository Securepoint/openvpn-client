#include "renameconfig.h"
#include "ui_renameconfig.h"

RenameConfig *RenameConfig::mInst = NULL;

RenameConfig *RenameConfig::getInstance() {
    if (!mInst)
        mInst = new RenameConfig ();
    return mInst;
}

RenameConfig::RenameConfig() :
    QDialog(),
    m_ui(new Ui::RenameConfig)
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
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

void RenameConfig::showEvent(QShowEvent *e) {
    m_ui->txtNewName->setText("");
    // Mittig ausrichten
    int screenH = qApp->desktop()->height();
    int screenW = qApp->desktop()->width();
    int winH = 120;
    int winW = 280;
    // Nun die neuen setzen
    this->setGeometry((screenW / 2) - (winW / 2), (screenH / 2) - (winH / 2), winW, winH);
    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
}

void RenameConfig::on_cmdClose_clicked()
{
    this->close();
}

void RenameConfig::on_cmdRename_clicked()
{
    bool isLinked = false;
    bool fError = false;
    QString errMes = "";

    if (m_ui->txtNewName->text().trimmed() != "") {
        QFile renFile (this->fullPath);
        QString oldPath (this->fullPath);
                oldPath = oldPath.replace("\\","/");
        if (Configs::getInstance()->isConfigLinked(oldPath))
            isLinked = true;
        if (renFile.exists()) {
            // rename
            QString renPath (this->fullPath.left(this->fullPath.lastIndexOf("/")));
            if (!renFile.rename(renPath + QString("/") + m_ui->txtNewName->text().trimmed() + QString(".ovpn"))) {
                fError = true;
                errMes = QString (tr("Rename failed!"));
            }
        }
        if(!fError)
            if (isLinked) {
                Configs::getInstance()->changeConfigNameInLinkedList(oldPath, oldPath.left(oldPath.lastIndexOf("/")) + QString("/") + m_ui->txtNewName->text().trimmed() + QString(".ovpn"));
            }
    } else {
        fError = true;
        errMes = QString (tr("New name is empty!"));
    }

    if (fError) {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("An error has occurred!"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(errMes);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
    } else {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Securepoint SSL VPN"));
        msgBox.setText(tr("Rename Configuration"));
        msgBox.setWindowIcon(QIcon(":/images/logo.png"));
        msgBox.setInformativeText(tr("Rename was successfully!"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        msgBox.exec();
        MainWindowControll::getInstance()->refreshConfigs();
        MainWindowControll::getInstance()->refreshDialog();
        MainWindowControll::getInstance()->setConnectionStatus();
        this->close();
    }
}

void RenameConfig::setOldName(QString oldname) {
    m_ui->lblOldName->setText(oldname);
}

void RenameConfig::setFullPath(QString path) {
    this->fullPath = path;
}
