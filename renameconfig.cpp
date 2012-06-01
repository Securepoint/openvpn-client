#include "renameconfig.h"
#include "ui_renameconfig.h"

#include "preferences.h"
#include "message.h"

RenameConfig *RenameConfig::mInst = NULL;

RenameConfig *RenameConfig::getInstance() {
    if (!mInst)
        mInst = new RenameConfig ();
    return mInst;
}

RenameConfig::RenameConfig() :
    QDialog(),
    m_ui(new Ui::RenameConfig),
    fullPath ("")
{
    m_ui->setupUi(this);
    this->setWindowFlags(Qt::WindowCloseButtonHint);
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
    int winW = this->width();
    int winH = this->height();

    int left = Preferences::instance()->geometry().x();
    left = left + (Preferences::instance()->geometry().width() - winW) / 2;

    // Nun die neuen setzen
    this->setGeometry(left, (qApp->desktop()->height() / 2) - (winH / 2), winW, winH);
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
    bool isLinked (false);
    bool fError (false);
    QString errMes ("");

    if (!m_ui->txtNewName->text().trimmed().isEmpty()) {
        QFile renFile (this->fullPath);
        QString oldPath (this->fullPath);
                oldPath = oldPath.replace("\\","/");
        if (Configs::getInstance()->isConfigLinked(oldPath)) {
            isLinked = true;
        }

        if (renFile.exists()) {
            // rename
            QString renPath (this->fullPath.left(this->fullPath.lastIndexOf("/")));
            if (!renFile.rename(renPath + QLatin1String("/") + m_ui->txtNewName->text().trimmed() + QLatin1String(".ovpn"))) {
                fError = true;
                errMes = QObject::tr("Rename failed!");
            }
        }

        if(!fError) {
            if (isLinked) {
                Configs::getInstance()->changeConfigNameInLinkedList(oldPath, oldPath.left(oldPath.lastIndexOf("/")) + QLatin1String("/") + m_ui->txtNewName->text().trimmed() + QLatin1String(".ovpn"));
            }
        }
    } else {
        fError = true;
        errMes = QObject::tr("New name is empty!");
    }

    if (fError) {
        Message::error(errMes, QObject::tr("Rename Configuration"));
    } else {
        Message::information(QObject::tr("Rename was successfully!"), QObject::tr("Rename Configuration"));

        Preferences::instance()->refreshConfigList();
        Preferences::instance()->refreshDialog();
        Preferences::instance()->setConnectionStatus();
        Preferences::instance()->setIcon();
        this->close();
    }
}

void RenameConfig::setOldName(const QString &oldname) {
    m_ui->lblOldName->setText(oldname);
}

void RenameConfig::setFullPath(const QString &path) {
    this->fullPath = path;
}
