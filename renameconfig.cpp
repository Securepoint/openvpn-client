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
    id(-1)
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

    int left (0);
    int top (0);
    if (Preferences::instance()->isVisible()) {
        // Wenn das Hauptfenster offen ist mittig über diesem plazieren
        left = Preferences::instance()->geometry().x();
        top = Preferences::instance()->geometry().y();
        left = left + (Preferences::instance()->geometry().width() - winW) / 2;
        //
        top  = top + (Preferences::instance()->geometry().height() - winH) / 2;
    } else {
        // Desktop auswerten
        top = qApp->desktop()->height();
        left = qApp->desktop()->width();
        // Die Breite bei virtuellen Desktops vierteln
        if (left > 2000 && qApp->desktop()->isVirtualDesktop()) {
            left /= 4;
        } else {
            // Normaler Desktop
            left = (left - winH) / 2;
        }
        // Height
        if (top > 2000 && qApp->desktop()->isVirtualDesktop()) {
            top /= 4;
        } else {
            top = (top - winH) / 2;
        }
    }
    // Nun die neuen setzen
    this->setGeometry(left, top, winW, winH);
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
    if (this->id == -1 || m_ui->txtNewName->text().isEmpty() || m_ui->lblOldName->text() == m_ui->txtNewName->text()) {
        return;
    }

    Preferences::instance()->renameConfig(this->id, m_ui->txtNewName->text().trimmed(), m_ui->lblOldName->text().trimmed());
    Preferences::instance()->refreshConfigList();
    Preferences::instance()->refreshDialog();
    Preferences::instance()->setConnectionStatus();
    Preferences::instance()->setIcon();
    this->close();
}

void RenameConfig::setOldName(const QString &oldname) {
    m_ui->lblOldName->setText(oldname);
}

void RenameConfig::setId(int id)
{
    this->id = id;
}
