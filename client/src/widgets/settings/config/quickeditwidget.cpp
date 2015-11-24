#include "quickeditwidget.h"
#include "ui_quickeditwidget.h"

#include <QtGui>
#include <QtGui/qguiapplication.h>
#include <QtWidgets/QDesktopWidget>

float windowsDpiScale();

QuickEditWidget::QuickEditWidget(const QString &path, QWidget * parent) :
    FramelessDialog(parent),
    m_ui(new Ui::QuickEditWidget),
    path (path)
{
     
    this->setWindowFlags(Qt::WindowCloseButtonHint);

    m_ui->setupUi((QDialog*)this->getWrapperWidget());  

    auto geom = this->getWrapperWidget()->geometry();
    auto size = geom.size() * windowsDpiScale();
    geom = geometry();
    geom.setSize(size);
    this->getWrapperWidget()->setGeometry(geom);


    size = QSize(0, m_ui->memConfigContent->maximumSize().height() * windowsDpiScale());
    m_ui->memConfigContent->setMinimumSize(size);
    size.setWidth(16666667);
    m_ui->memConfigContent->setMaximumSize(size);

    size = QSize(0, m_ui->memConfigPath->maximumSize().height() * windowsDpiScale());
    m_ui->memConfigPath->setMinimumSize(size);
    size.setWidth(16666667);
    m_ui->memConfigPath->setMaximumSize(size);

    updateGeometry();
    m_ui->memConfigContent->updateGeometry();

    geom = m_ui->memConfigContent->geometry();
    geom.setHeight(size.height());
    m_ui->memConfigContent->setGeometry(geom);

    geom = m_ui->groupBox->geometry();
    geom.setHeight(size.height());
    m_ui->groupBox->setGeometry(geom);

    QMetaObject::connectSlotsByName(this);

    this->setupFrameless();
}

void QuickEditWidget::changeEvent(QEvent *e)
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

void QuickEditWidget::showEvent(QShowEvent *e) {
    m_ui->memConfigPath->setPlainText(this->path.replace("\\", "/"));
    // Content holen und setzen
    m_ui->memConfigContent->setPlainText(this->getContent());
    // Dialog öffnen
    // Mittig ausrichten
    int winW = this->width();
    int winH = this->height();

    int left (0);
    int top (0);

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

    // Nun die neuen setzen
    this->setGeometry(left, top, winW, winH);

    // Öffnen
    e->accept();
    this->setWindowState(Qt::WindowActive);
}

void QuickEditWidget::on_cmdSave_clicked()
{
    // Speichern
    QFile saveFile (this->path);
    if (!saveFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
         return;
    }

     QTextStream out(&saveFile);
     out << m_ui->memConfigContent->toPlainText();
     saveFile.close();
     // Reload File
     m_ui->memConfigContent->setPlainText(this->getContent());
     this->close();
}

void QuickEditWidget::on_cmdClose_clicked()
{
    this->close();
}

QString QuickEditWidget::getContent() {
    QString fileContent ("");
    QFile cf (this->path);

    if (!cf.open(QIODevice::ReadOnly | QIODevice::Text))
         return QLatin1String("");

    QTextStream in(&cf);
    while (!in.atEnd()) {
        QString line = in.readLine();
        fileContent += line + QLatin1String("\n");
    }
    cf.close();

    return fileContent;
}
