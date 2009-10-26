/********************************************************************************
** Form generated from reading ui file 'wiz_certpage.ui'
**
** Created: Fri 23. Oct 14:51:25 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WIZ_CERTPAGE_H
#define UI_WIZ_CERTPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_CertPage
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QCheckBox *cbServerCert;
    QLineEdit *txtKeyPath;
    QLineEdit *txtCertPath;
    QLineEdit *txtCAPath;
    QTextEdit *memHelp;
    QPushButton *cmdInfoServerCert;
    QPushButton *cmdOpenCA;
    QPushButton *cmdInfoCA;
    QPushButton *cmdOpenCert;
    QPushButton *cmdInfoCert;
    QPushButton *cmdOpenKey;
    QPushButton *cmdInfoKey;

    void setupUi(QWizardPage *CertPage)
    {
        if (CertPage->objectName().isEmpty())
            CertPage->setObjectName(QString::fromUtf8("CertPage"));
        CertPage->resize(400, 300);
        label = new QLabel(CertPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 18, 91, 16));
        label_2 = new QLabel(CertPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(30, 70, 91, 16));
        label_3 = new QLabel(CertPage);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(31, 116, 91, 16));
        label_4 = new QLabel(CertPage);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 162, 101, 16));
        cbServerCert = new QCheckBox(CertPage);
        cbServerCert->setObjectName(QString::fromUtf8("cbServerCert"));
        cbServerCert->setGeometry(QRect(121, 161, 21, 19));
        txtKeyPath = new QLineEdit(CertPage);
        txtKeyPath->setObjectName(QString::fromUtf8("txtKeyPath"));
        txtKeyPath->setGeometry(QRect(30, 134, 340, 20));
        txtKeyPath->setReadOnly(true);
        txtCertPath = new QLineEdit(CertPage);
        txtCertPath->setObjectName(QString::fromUtf8("txtCertPath"));
        txtCertPath->setGeometry(QRect(30, 90, 341, 20));
        txtCertPath->setReadOnly(true);
        txtCAPath = new QLineEdit(CertPage);
        txtCAPath->setObjectName(QString::fromUtf8("txtCAPath"));
        txtCAPath->setGeometry(QRect(30, 36, 341, 20));
        txtCAPath->setReadOnly(true);
        memHelp = new QTextEdit(CertPage);
        memHelp->setObjectName(QString::fromUtf8("memHelp"));
        memHelp->setGeometry(QRect(11, 220, 371, 71));
        memHelp->setContextMenuPolicy(Qt::NoContextMenu);
        memHelp->setStyleSheet(QString::fromUtf8("background-color: rgb(241, 161, 80);"));
        memHelp->setFrameShape(QFrame::Box);
        memHelp->setFrameShadow(QFrame::Plain);
        memHelp->setReadOnly(true);
        cmdInfoServerCert = new QPushButton(CertPage);
        cmdInfoServerCert->setObjectName(QString::fromUtf8("cmdInfoServerCert"));
        cmdInfoServerCert->setGeometry(QRect(138, 163, 16, 16));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/info.svg"), QSize(), QIcon::Normal, QIcon::Off);
        cmdInfoServerCert->setIcon(icon);
        cmdInfoServerCert->setFlat(true);
        cmdOpenCA = new QPushButton(CertPage);
        cmdOpenCA->setObjectName(QString::fromUtf8("cmdOpenCA"));
        cmdOpenCA->setGeometry(QRect(375, 35, 21, 21));
        cmdInfoCA = new QPushButton(CertPage);
        cmdInfoCA->setObjectName(QString::fromUtf8("cmdInfoCA"));
        cmdInfoCA->setGeometry(QRect(76, 16, 16, 20));
        cmdInfoCA->setIcon(icon);
        cmdInfoCA->setFlat(true);
        cmdOpenCert = new QPushButton(CertPage);
        cmdOpenCert->setObjectName(QString::fromUtf8("cmdOpenCert"));
        cmdOpenCert->setGeometry(QRect(376, 90, 21, 21));
        cmdInfoCert = new QPushButton(CertPage);
        cmdInfoCert->setObjectName(QString::fromUtf8("cmdInfoCert"));
        cmdInfoCert->setGeometry(QRect(85, 69, 20, 20));
        cmdInfoCert->setIcon(icon);
        cmdInfoCert->setFlat(true);
        cmdOpenKey = new QPushButton(CertPage);
        cmdOpenKey->setObjectName(QString::fromUtf8("cmdOpenKey"));
        cmdOpenKey->setGeometry(QRect(377, 133, 20, 21));
        cmdInfoKey = new QPushButton(CertPage);
        cmdInfoKey->setObjectName(QString::fromUtf8("cmdInfoKey"));
        cmdInfoKey->setGeometry(QRect(54, 115, 16, 20));
        cmdInfoKey->setIcon(icon);
        cmdInfoKey->setFlat(true);

        retranslateUi(CertPage);

        QMetaObject::connectSlotsByName(CertPage);
    } // setupUi

    void retranslateUi(QWizardPage *CertPage)
    {
        CertPage->setWindowTitle(QApplication::translate("CertPage", "WizardPage", 0, QApplication::UnicodeUTF8));
        CertPage->setTitle(QApplication::translate("CertPage", "Certificates", 0, QApplication::UnicodeUTF8));
        CertPage->setSubTitle(QApplication::translate("CertPage", "Please specify the certifcates for the connection.", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CertPage", "Root CA:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CertPage", "Certificate:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CertPage", "Key:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CertPage", "Server certificate:", 0, QApplication::UnicodeUTF8));
        cbServerCert->setText(QString());
        memHelp->setHtml(QApplication::translate("CertPage", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Please click the info symbol of the item you want to see the help. </span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">For further informations please visit: http://www.openvpn.net</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        cmdInfoServerCert->setText(QString());
        cmdOpenCA->setText(QApplication::translate("CertPage", "...", 0, QApplication::UnicodeUTF8));
        cmdInfoCA->setText(QString());
        cmdOpenCert->setText(QApplication::translate("CertPage", "...", 0, QApplication::UnicodeUTF8));
        cmdInfoCert->setText(QString());
        cmdOpenKey->setText(QApplication::translate("CertPage", "...", 0, QApplication::UnicodeUTF8));
        cmdInfoKey->setText(QString());
        Q_UNUSED(CertPage);
    } // retranslateUi

};

namespace Ui {
    class CertPage: public Ui_CertPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZ_CERTPAGE_H
