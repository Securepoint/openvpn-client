/********************************************************************************
** Form generated from reading ui file 'proxysettings.ui'
**
** Created: Tue 3. Nov 16:14:15 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_PROXYSETTINGS_H
#define UI_PROXYSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_ProxySettings
{
public:
    QGroupBox *groupBox_2;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *txtProxyIP;
    QLabel *label_2;
    QLineEdit *txtProxyPort;
    QRadioButton *rbHttpProxy;
    QRadioButton *rbSocksProxy;
    QGroupBox *groupBox_3;
    QRadioButton *rbUseConfig;
    QRadioButton *rbUseIE;
    QRadioButton *rbUseManual;
    QPushButton *cmdSave;
    QPushButton *cmdClose;

    void setupUi(QDialog *ProxySettings)
    {
        if (ProxySettings->objectName().isEmpty())
            ProxySettings->setObjectName(QString::fromUtf8("ProxySettings"));
        ProxySettings->resize(400, 300);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ProxySettings->setWindowIcon(icon);
        ProxySettings->setModal(true);
        groupBox_2 = new QGroupBox(ProxySettings);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 10, 381, 241));
        groupBox = new QGroupBox(groupBox_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(9, 118, 361, 111));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 69, 21, 16));
        txtProxyIP = new QLineEdit(groupBox);
        txtProxyIP->setObjectName(QString::fromUtf8("txtProxyIP"));
        txtProxyIP->setGeometry(QRect(30, 67, 151, 20));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(203, 67, 31, 20));
        txtProxyPort = new QLineEdit(groupBox);
        txtProxyPort->setObjectName(QString::fromUtf8("txtProxyPort"));
        txtProxyPort->setGeometry(QRect(230, 67, 61, 20));
        rbHttpProxy = new QRadioButton(groupBox);
        rbHttpProxy->setObjectName(QString::fromUtf8("rbHttpProxy"));
        rbHttpProxy->setGeometry(QRect(10, 20, 82, 19));
        rbHttpProxy->setChecked(true);
        rbSocksProxy = new QRadioButton(groupBox);
        rbSocksProxy->setObjectName(QString::fromUtf8("rbSocksProxy"));
        rbSocksProxy->setGeometry(QRect(10, 40, 82, 19));
        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 18, 361, 101));
        rbUseConfig = new QRadioButton(groupBox_3);
        rbUseConfig->setObjectName(QString::fromUtf8("rbUseConfig"));
        rbUseConfig->setGeometry(QRect(10, 10, 191, 19));
        rbUseConfig->setChecked(true);
        rbUseIE = new QRadioButton(groupBox_3);
        rbUseIE->setObjectName(QString::fromUtf8("rbUseIE"));
        rbUseIE->setEnabled(false);
        rbUseIE->setGeometry(QRect(10, 40, 181, 19));
        rbUseManual = new QRadioButton(groupBox_3);
        rbUseManual->setObjectName(QString::fromUtf8("rbUseManual"));
        rbUseManual->setGeometry(QRect(10, 70, 131, 19));
        cmdSave = new QPushButton(ProxySettings);
        cmdSave->setObjectName(QString::fromUtf8("cmdSave"));
        cmdSave->setGeometry(QRect(200, 260, 75, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdSave->setIcon(icon1);
        cmdSave->setIconSize(QSize(30, 16));
        cmdSave->setFlat(true);
        cmdClose = new QPushButton(ProxySettings);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(310, 260, 75, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon2);
        cmdClose->setIconSize(QSize(30, 16));
        cmdClose->setFlat(true);

        retranslateUi(ProxySettings);

        QMetaObject::connectSlotsByName(ProxySettings);
    } // setupUi

    void retranslateUi(QDialog *ProxySettings)
    {
        ProxySettings->setWindowTitle(QApplication::translate("ProxySettings", "Proxy Settings", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("ProxySettings", "Proxy Settings:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ProxySettings", "Manual Settings:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ProxySettings", "IP:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ProxySettings", "Port:", 0, QApplication::UnicodeUTF8));
        txtProxyPort->setText(QApplication::translate("ProxySettings", "8080", 0, QApplication::UnicodeUTF8));
        rbHttpProxy->setText(QApplication::translate("ProxySettings", "HTTP Proxy", 0, QApplication::UnicodeUTF8));
        rbSocksProxy->setText(QApplication::translate("ProxySettings", "SOCKS Proxy", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QString());
        rbUseConfig->setText(QApplication::translate("ProxySettings", "Use OpenVPN config settings", 0, QApplication::UnicodeUTF8));
        rbUseIE->setText(QApplication::translate("ProxySettings", "Use Internet Explorer settings", 0, QApplication::UnicodeUTF8));
        rbUseManual->setText(QApplication::translate("ProxySettings", "Use manual settings", 0, QApplication::UnicodeUTF8));
        cmdSave->setText(QString());
        cmdClose->setText(QString());
        Q_UNUSED(ProxySettings);
    } // retranslateUi

};

namespace Ui {
    class ProxySettings: public Ui_ProxySettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROXYSETTINGS_H
