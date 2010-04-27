#include "editconfig.h"
#include "ui_editconfig.h"

EditConfig *EditConfig::mInst = NULL;

EditConfig *EditConfig::getInstance() {
    if (!mInst)
        mInst = new EditConfig ();
    return mInst;
}

EditConfig::EditConfig() :
    QDialog(),
    m_ui(new Ui::EditConfig)
{
    m_ui->setupUi(this);
    this->path = "";
    this->setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowStaysOnTopHint);
}

void EditConfig::changeEvent(QEvent *e)
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

void EditConfig::showEvent(QShowEvent *e) {
    m_ui->memConfigPath->setPlainText(this->path.replace("\\", "/"));
    // Content holen und setzen
    m_ui->memConfigContent->setPlainText(this->getContent());
    // Dialog öffnen
    // Mittig ausrichten
    int screenH = qApp->desktop()->height();
    int screenW = qApp->desktop()->width();
    int winH = 420;
    int winW = 590;
    // Nun die neuen setzen
    this->setGeometry((screenW / 2) - (winW / 2), (screenH / 2) - (winH / 2), winW, winH);
    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
}

void EditConfig::setPath(QString path) {
    this->path = path;
}

void EditConfig::on_cmdSave_clicked()
{
    // Speichern
    QFile saveFile (this->path);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

     QTextStream out(&saveFile);
     out << m_ui->memConfigContent->toPlainText();
     saveFile.close();
     // Reload File
     m_ui->memConfigContent->setPlainText(this->getContent());
}

void EditConfig::on_cmdClose_clicked()
{
    this->close();
}

QString EditConfig::getContent() {
    QString fileContent = QString("");
    QFile cf (this->path);

    if (!cf.open(QIODevice::ReadOnly | QIODevice::Text))
         return "";

    QTextStream in(&cf);
    while (!in.atEnd()) {
        QString line = in.readLine();
        fileContent += line + QString("\n");
    }
    cf.close();
    return fileContent;
}
