/********************************************************************************
** Form generated from reading UI file 'proxysettings.ui'
**
** Created: Thu 12. Jan 13:39:28 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
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
    QRadioButton *rbUseIE;
    QRadioButton *rbUseConfig;
    QRadioButton *rbUseManual;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *txtProxyIP;
    QLabel *label_2;
    QLineEdit *txtProxyPort;
    QRadioButton *rbHttpProxy;
    QRadioButton *rbSocksProxy;
    QLabel *label_3;
    QPushButton *cmdSave;
    QPushButton *cmdClose;

    void setupUi(QDialog *ProxySettings)
    {
        if (ProxySettings->objectName().isEmpty())
            ProxySettings->setObjectName(QString::fromUtf8("ProxySettings"));
        ProxySettings->setWindowModality(Qt::ApplicationModal);
        ProxySettings->resize(350, 300);
        ProxySettings->setMinimumSize(QSize(350, 300));
        ProxySettings->setMaximumSize(QSize(350, 300));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        ProxySettings->setWindowIcon(icon);
        ProxySettings->setModal(true);
        groupBox_2 = new QGroupBox(ProxySettings);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 4, 331, 251));
        rbUseIE = new QRadioButton(groupBox_2);
        rbUseIE->setObjectName(QString::fromUtf8("rbUseIE"));
        rbUseIE->setEnabled(false);
        rbUseIE->setGeometry(QRect(12, 38, 281, 19));
        rbUseIE->setStyleSheet(QString::fromUtf8("QRadioButton { \n"
"spacing: 5px;\n"
"}\n"
"\n"
"QRadioButton::indicator {\n"
"width: 13px;\n"
"height: 13px;\n"
"}\n"
"\n"
"QRadioButton::indicator::unchecked:enabled {\n"
"image: url(:/images/styles/radiobutton_unchecked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::checked:enabled {\n"
"image: url(:/images/styles/radiobutton_checked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::unchecked:disabled {\n"
"image: url(:/images/styles/radiobutton_unchecked_dis.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::checked:disabled {\n"
"image: url(:/images/styles/radiobutton_checked_dis.png);\n"
"}"));
        rbUseConfig = new QRadioButton(groupBox_2);
        rbUseConfig->setObjectName(QString::fromUtf8("rbUseConfig"));
        rbUseConfig->setGeometry(QRect(12, 18, 291, 19));
        rbUseConfig->setStyleSheet(QString::fromUtf8("QRadioButton { \n"
"spacing: 5px;\n"
"}\n"
"\n"
"QRadioButton::indicator {\n"
"width: 13px;\n"
"height: 13px;\n"
"}\n"
"\n"
"QRadioButton::indicator::unchecked:enabled {\n"
"image: url(:/images/styles/radiobutton_unchecked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::checked:enabled {\n"
"image: url(:/images/styles/radiobutton_checked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::unchecked:disabled {\n"
"image: url(:/images/styles/radiobutton_unchecked_dis.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::checked:disabled {\n"
"image: url(:/images/styles/radiobutton_checked_dis.png);\n"
"}"));
        rbUseConfig->setChecked(true);
        rbUseManual = new QRadioButton(groupBox_2);
        rbUseManual->setObjectName(QString::fromUtf8("rbUseManual"));
        rbUseManual->setGeometry(QRect(12, 59, 281, 19));
        rbUseManual->setStyleSheet(QString::fromUtf8("QRadioButton { \n"
"spacing: 5px;\n"
"}\n"
"\n"
"QRadioButton::indicator {\n"
"width: 13px;\n"
"height: 13px;\n"
"}\n"
"\n"
"QRadioButton::indicator::unchecked:enabled {\n"
"image: url(:/images/styles/radiobutton_unchecked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::checked:enabled {\n"
"image: url(:/images/styles/radiobutton_checked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::unchecked:disabled {\n"
"image: url(:/images/styles/radiobutton_unchecked_dis.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::checked:disabled {\n"
"image: url(:/images/styles/radiobutton_checked_dis.png);\n"
"}"));
        groupBox = new QGroupBox(groupBox_2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(29, 80, 281, 91));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 63, 21, 16));
        txtProxyIP = new QLineEdit(groupBox);
        txtProxyIP->setObjectName(QString::fromUtf8("txtProxyIP"));
        txtProxyIP->setGeometry(QRect(30, 61, 151, 20));
        txtProxyIP->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(188, 61, 31, 20));
        txtProxyPort = new QLineEdit(groupBox);
        txtProxyPort->setObjectName(QString::fromUtf8("txtProxyPort"));
        txtProxyPort->setGeometry(QRect(215, 61, 51, 20));
        txtProxyPort->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }"));
        rbHttpProxy = new QRadioButton(groupBox);
        rbHttpProxy->setObjectName(QString::fromUtf8("rbHttpProxy"));
        rbHttpProxy->setGeometry(QRect(10, 20, 82, 19));
        rbHttpProxy->setStyleSheet(QString::fromUtf8("QRadioButton { \n"
"spacing: 5px;\n"
"}\n"
"\n"
"QRadioButton::indicator {\n"
"width: 13px;\n"
"height: 13px;\n"
"}\n"
"\n"
"QRadioButton::indicator::unchecked:enabled {\n"
"image: url(:/images/styles/radiobutton_unchecked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::checked:enabled {\n"
"image: url(:/images/styles/radiobutton_checked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::unchecked:disabled {\n"
"image: url(:/images/styles/radiobutton_unchecked_dis.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::checked:disabled {\n"
"image: url(:/images/styles/radiobutton_checked_dis.png);\n"
"}"));
        rbHttpProxy->setChecked(true);
        rbSocksProxy = new QRadioButton(groupBox);
        rbSocksProxy->setObjectName(QString::fromUtf8("rbSocksProxy"));
        rbSocksProxy->setGeometry(QRect(10, 40, 82, 19));
        rbSocksProxy->setStyleSheet(QString::fromUtf8("QRadioButton { \n"
"spacing: 5px;\n"
"}\n"
"\n"
"QRadioButton::indicator {\n"
"width: 13px;\n"
"height: 13px;\n"
"}\n"
"\n"
"QRadioButton::indicator::unchecked:enabled {\n"
"image: url(:/images/styles/radiobutton_unchecked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::checked:enabled {\n"
"image: url(:/images/styles/radiobutton_checked.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::unchecked:disabled {\n"
"image: url(:/images/styles/radiobutton_unchecked_dis.png);\n"
"}\n"
"\n"
"QRadioButton::indicator::checked:disabled {\n"
"image: url(:/images/styles/radiobutton_checked_dis.png);\n"
"}"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 174, 311, 71));
        label_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);\n"
"border: 2px solid #FFFFFF;"));
        label_3->setWordWrap(true);
        cmdSave = new QPushButton(ProxySettings);
        cmdSave->setObjectName(QString::fromUtf8("cmdSave"));
        cmdSave->setGeometry(QRect(130, 260, 100, 33));
        cmdSave->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdSave->setIcon(icon1);
        cmdSave->setIconSize(QSize(22, 22));
        cmdSave->setDefault(true);
        cmdSave->setFlat(true);
        cmdClose = new QPushButton(ProxySettings);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(240, 260, 100, 33));
        cmdClose->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/close_dialog.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon2);
        cmdClose->setIconSize(QSize(22, 22));
        cmdClose->setFlat(true);

        retranslateUi(ProxySettings);

        QMetaObject::connectSlotsByName(ProxySettings);
    } // setupUi

    void retranslateUi(QDialog *ProxySettings)
    {
        ProxySettings->setWindowTitle(QApplication::translate("ProxySettings", "Proxy Settings", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("ProxySettings", "Proxy Settings:", 0, QApplication::UnicodeUTF8));
        rbUseIE->setText(QApplication::translate("ProxySettings", "Use Internet Explorer settings", 0, QApplication::UnicodeUTF8));
        rbUseConfig->setText(QApplication::translate("ProxySettings", "Use OpenVPN config settings", 0, QApplication::UnicodeUTF8));
        rbUseManual->setText(QApplication::translate("ProxySettings", "Use manual settings", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ProxySettings", "Manual Settings:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ProxySettings", "IP:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ProxySettings", "Port:", 0, QApplication::UnicodeUTF8));
        txtProxyPort->setText(QApplication::translate("ProxySettings", "8080", 0, QApplication::UnicodeUTF8));
        rbHttpProxy->setText(QApplication::translate("ProxySettings", "HTTP Proxy", 0, QApplication::UnicodeUTF8));
        rbSocksProxy->setText(QApplication::translate("ProxySettings", "SOCKS Proxy", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ProxySettings", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">For a connection to a HTTP-Proxy with authentication, please use the config settings.</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">It is possible to configure at manage settings, tab advanced.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        cmdSave->setText(QApplication::translate("ProxySettings", "     &Save", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QApplication::translate("ProxySettings", "     &Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ProxySettings: public Ui_ProxySettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROXYSETTINGS_H
