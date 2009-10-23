/********************************************************************************
** Form generated from reading ui file 'wiz_generalpage.ui'
**
** Created: Thu 22. Oct 16:13:37 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WIZ_GENERALPAGE_H
#define UI_WIZ_GENERALPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_GeneralPage
{
public:
    QLabel *lblConfigName;
    QLabel *label;
    QLineEdit *txtConfigName;
    QPushButton *cmdInfoName;
    QTextEdit *memHelp;

    void setupUi(QWizardPage *GeneralPage)
    {
        if (GeneralPage->objectName().isEmpty())
            GeneralPage->setObjectName(QString::fromUtf8("GeneralPage"));
        GeneralPage->resize(400, 300);
        lblConfigName = new QLabel(GeneralPage);
        lblConfigName->setObjectName(QString::fromUtf8("lblConfigName"));
        lblConfigName->setGeometry(QRect(20, 20, 191, 20));
        label = new QLabel(GeneralPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 50, 46, 14));
        txtConfigName = new QLineEdit(GeneralPage);
        txtConfigName->setObjectName(QString::fromUtf8("txtConfigName"));
        txtConfigName->setGeometry(QRect(70, 48, 291, 20));
        cmdInfoName = new QPushButton(GeneralPage);
        cmdInfoName->setObjectName(QString::fromUtf8("cmdInfoName"));
        cmdInfoName->setGeometry(QRect(364, 50, 16, 16));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/info.svg"), QSize(), QIcon::Normal, QIcon::Off);
        cmdInfoName->setIcon(icon);
        cmdInfoName->setFlat(true);
        memHelp = new QTextEdit(GeneralPage);
        memHelp->setObjectName(QString::fromUtf8("memHelp"));
        memHelp->setGeometry(QRect(15, 220, 371, 71));
        memHelp->setContextMenuPolicy(Qt::NoContextMenu);
        memHelp->setStyleSheet(QString::fromUtf8("background-color: rgb(241, 161, 80);"));
        memHelp->setFrameShape(QFrame::Box);
        memHelp->setFrameShadow(QFrame::Plain);
        memHelp->setReadOnly(true);

        retranslateUi(GeneralPage);

        QMetaObject::connectSlotsByName(GeneralPage);
    } // setupUi

    void retranslateUi(QWizardPage *GeneralPage)
    {
        GeneralPage->setWindowTitle(QApplication::translate("GeneralPage", "WizardPage", 0, QApplication::UnicodeUTF8));
        GeneralPage->setTitle(QApplication::translate("GeneralPage", "General Settings", 0, QApplication::UnicodeUTF8));
        GeneralPage->setSubTitle(QApplication::translate("GeneralPage", "Specify the name of the config file which should create.", 0, QApplication::UnicodeUTF8));
        lblConfigName->setText(QApplication::translate("GeneralPage", "Please enter the name of the config.", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GeneralPage", "Name:", 0, QApplication::UnicodeUTF8));
        cmdInfoName->setText(QString());
        memHelp->setHtml(QApplication::translate("GeneralPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Please click the info symbol of the item you want to see the help. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">For further informations please visit: http://www.openvpn.net</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(GeneralPage);
    } // retranslateUi

};

namespace Ui {
    class GeneralPage: public Ui_GeneralPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZ_GENERALPAGE_H
