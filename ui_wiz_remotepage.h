/********************************************************************************
** Form generated from reading UI file 'wiz_remotepage.ui'
**
** Created: Thu 12. Jan 13:39:28 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIZ_REMOTEPAGE_H
#define UI_WIZ_REMOTEPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_RemotePage
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *txtRemoteIP;
    QLineEdit *txtRemotePort;
    QComboBox *cmbRemoteProtocol;

    void setupUi(QWizardPage *RemotePage)
    {
        if (RemotePage->objectName().isEmpty())
            RemotePage->setObjectName(QString::fromUtf8("RemotePage"));
        RemotePage->resize(400, 300);
        label = new QLabel(RemotePage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 371, 16));
        label_2 = new QLabel(RemotePage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 70, 51, 16));
        label_3 = new QLabel(RemotePage);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 95, 41, 16));
        label_4 = new QLabel(RemotePage);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 122, 46, 14));
        txtRemoteIP = new QLineEdit(RemotePage);
        txtRemoteIP->setObjectName(QString::fromUtf8("txtRemoteIP"));
        txtRemoteIP->setGeometry(QRect(80, 69, 171, 20));
        txtRemoteIP->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }"));
        txtRemotePort = new QLineEdit(RemotePage);
        txtRemotePort->setObjectName(QString::fromUtf8("txtRemotePort"));
        txtRemotePort->setGeometry(QRect(80, 93, 71, 20));
        txtRemotePort->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }"));
        cmbRemoteProtocol = new QComboBox(RemotePage);
        cmbRemoteProtocol->setObjectName(QString::fromUtf8("cmbRemoteProtocol"));
        cmbRemoteProtocol->setGeometry(QRect(80, 118, 71, 22));
        cmbRemoteProtocol->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }"));

        retranslateUi(RemotePage);

        QMetaObject::connectSlotsByName(RemotePage);
    } // setupUi

    void retranslateUi(QWizardPage *RemotePage)
    {
        RemotePage->setWindowTitle(QApplication::translate("RemotePage", "WizardPage", 0, QApplication::UnicodeUTF8));
        RemotePage->setTitle(QApplication::translate("RemotePage", "Remote Settings", 0, QApplication::UnicodeUTF8));
        RemotePage->setSubTitle(QApplication::translate("RemotePage", "Specify the remote settings of the connection.", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RemotePage", "Please enter the IP and Port of the remote OpenVPN server.", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RemotePage", "IP:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("RemotePage", "Port:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("RemotePage", "Protocol:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RemotePage: public Ui_RemotePage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZ_REMOTEPAGE_H
