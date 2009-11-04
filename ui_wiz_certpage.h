/********************************************************************************
** Form generated from reading ui file 'wiz_certpage.ui'
**
** Created: Wed 4. Nov 17:16:47 2009
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
    QPushButton *cmdOpenCA;
    QPushButton *cmdOpenCert;
    QPushButton *cmdOpenKey;

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
        cbServerCert->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
"spacing: 5px;\n"
"}\n"
"\n"
"QCheckBox::indicator {\n"
"width: 13px;\n"
"height: 13px;\n"
"}\n"
"\n"
"QCheckBox::indicator:unchecked:disabled {\n"
"image: url(:/images/styles/checkbox_unchecked_dis.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:checked:disabled {\n"
"image: url(:/images/styles/checkbox_checked_dis.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:unchecked:enabled {\n"
"image: url(:/images/styles/checkbox_unchecked.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:checked:enabled {\n"
"image: url(:/images/styles/checkbox_checked.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:unchecked:enabled:hover {\n"
"image: url(:/images/styles/checkbox_unchecked_hover.png);\n"
"}\n"
"\n"
"QCheckBox::indicator:checked:enabled:hover {\n"
"image: url(:/images/styles/checkbox_checked_hover.png);\n"
"}\n"
""));
        txtKeyPath = new QLineEdit(CertPage);
        txtKeyPath->setObjectName(QString::fromUtf8("txtKeyPath"));
        txtKeyPath->setGeometry(QRect(30, 134, 340, 20));
        txtKeyPath->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }"));
        txtKeyPath->setReadOnly(true);
        txtCertPath = new QLineEdit(CertPage);
        txtCertPath->setObjectName(QString::fromUtf8("txtCertPath"));
        txtCertPath->setGeometry(QRect(30, 90, 341, 20));
        txtCertPath->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }"));
        txtCertPath->setReadOnly(true);
        txtCAPath = new QLineEdit(CertPage);
        txtCAPath->setObjectName(QString::fromUtf8("txtCAPath"));
        txtCAPath->setGeometry(QRect(30, 36, 341, 20));
        txtCAPath->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }"));
        txtCAPath->setReadOnly(true);
        cmdOpenCA = new QPushButton(CertPage);
        cmdOpenCA->setObjectName(QString::fromUtf8("cmdOpenCA"));
        cmdOpenCA->setGeometry(QRect(375, 36, 20, 20));
        cmdOpenCA->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        cmdOpenCert = new QPushButton(CertPage);
        cmdOpenCert->setObjectName(QString::fromUtf8("cmdOpenCert"));
        cmdOpenCert->setGeometry(QRect(376, 90, 20, 20));
        cmdOpenCert->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        cmdOpenKey = new QPushButton(CertPage);
        cmdOpenKey->setObjectName(QString::fromUtf8("cmdOpenKey"));
        cmdOpenKey->setGeometry(QRect(376, 134, 20, 20));
        cmdOpenKey->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));

        retranslateUi(CertPage);

        QMetaObject::connectSlotsByName(CertPage);
    } // setupUi

    void retranslateUi(QWizardPage *CertPage)
    {
        CertPage->setWindowTitle(QApplication::translate("CertPage", "WizardPage", 0, QApplication::UnicodeUTF8));
        CertPage->setTitle(QApplication::translate("CertPage", "Certificates", 0, QApplication::UnicodeUTF8));
        CertPage->setSubTitle(QApplication::translate("CertPage", "Please specify the certificates for the connection.", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("CertPage", "Root CA:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("CertPage", "Certificate:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("CertPage", "Key:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("CertPage", "Server certificate:", 0, QApplication::UnicodeUTF8));
        cbServerCert->setText(QString());
        cmdOpenCA->setText(QApplication::translate("CertPage", "...", 0, QApplication::UnicodeUTF8));
        cmdOpenCert->setText(QApplication::translate("CertPage", "...", 0, QApplication::UnicodeUTF8));
        cmdOpenKey->setText(QApplication::translate("CertPage", "...", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(CertPage);
    } // retranslateUi

};

namespace Ui {
    class CertPage: public Ui_CertPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZ_CERTPAGE_H
