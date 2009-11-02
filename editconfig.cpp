#include "editconfig.h"
#include "ui_editconfig.h"

EditConfig::EditConfig(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::EditConfig)
{
    m_ui->setupUi(this);
}

EditConfig::~EditConfig()
{
    delete m_ui;
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

void EditConfig::setPath(QString path) {
    m_ui->memConfigPath->setPlainText(path);
}

void EditConfig::setContent(QString con) {
    m_ui->memConfigContent->setPlainText(con);
}

void EditConfig::on_cmdSave_clicked()
{
    // Speichern
    QString sPath = m_ui->memConfigPath->toPlainText();
    QFile saveFile (sPath);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

     QTextStream out(&saveFile);
     out << m_ui->memConfigContent->toPlainText();
     saveFile.close();
     // Reload File
     QString fileReleoadContent = QString("");
     QFile cf (sPath);

     if (!cf.open(QIODevice::ReadOnly | QIODevice::Text))
          return;

     QTextStream in(&cf);
     while (!in.atEnd()) {
         QString line = in.readLine();
         fileReleoadContent += line + QString("\n");
     }
     cf.close();
     m_ui->memConfigContent->setPlainText(fileReleoadContent);
}

void EditConfig::on_cmdClose_clicked()
{
    this->close();
}
