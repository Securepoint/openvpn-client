/********************************************************************************
** Form generated from reading UI file 'stdserviceauth.ui'
**
** Created: Tue 23. Mar 14:00:30 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STDSERVICEAUTH_H
#define UI_STDSERVICEAUTH_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_StdServiceAuth
{
public:
    QPushButton *cmdClose;
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *txtUser;
    QLineEdit *txtPwd;
    QCheckBox *cbShowLog;
    QPushButton *cmdOK;

    void setupUi(QDialog *StdServiceAuth)
    {
        if (StdServiceAuth->objectName().isEmpty())
            StdServiceAuth->setObjectName(QString::fromUtf8("StdServiceAuth"));
        StdServiceAuth->setWindowModality(Qt::ApplicationModal);
        StdServiceAuth->resize(218, 147);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
        StdServiceAuth->setWindowIcon(icon);
        StdServiceAuth->setStyleSheet(QString::fromUtf8(""));
        cmdClose = new QPushButton(StdServiceAuth);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(131, 110, 75, 31));
        cmdClose->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon1);
        cmdClose->setIconSize(QSize(16, 16));
        cmdClose->setFlat(true);
        groupBox = new QGroupBox(StdServiceAuth);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 201, 101));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 21, 61, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 43, 61, 16));
        txtUser = new QLineEdit(groupBox);
        txtUser->setObjectName(QString::fromUtf8("txtUser"));
        txtUser->setGeometry(QRect(70, 19, 121, 20));
        txtUser->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":hover {border: 1px solid red; }"));
        txtPwd = new QLineEdit(groupBox);
        txtPwd->setObjectName(QString::fromUtf8("txtPwd"));
        txtPwd->setGeometry(QRect(70, 41, 121, 20));
        txtPwd->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":hover {border: 1px solid red; }"));
        cbShowLog = new QCheckBox(groupBox);
        cbShowLog->setObjectName(QString::fromUtf8("cbShowLog"));
        cbShowLog->setGeometry(QRect(10, 70, 141, 19));
        cbShowLog->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
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
        cmdOK = new QPushButton(StdServiceAuth);
        cmdOK->setObjectName(QString::fromUtf8("cmdOK"));
        cmdOK->setGeometry(QRect(50, 110, 75, 31));
        cmdOK->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/connectmen.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdOK->setIcon(icon2);
        cmdOK->setFlat(true);

        retranslateUi(StdServiceAuth);

        QMetaObject::connectSlotsByName(StdServiceAuth);
    } // setupUi

    void retranslateUi(QDialog *StdServiceAuth)
    {
        StdServiceAuth->setWindowTitle(QApplication::translate("StdServiceAuth", "Service Authentication", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QApplication::translate("StdServiceAuth", "&Close", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("StdServiceAuth", "User data:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StdServiceAuth", "Username:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("StdServiceAuth", "Password:", 0, QApplication::UnicodeUTF8));
        cbShowLog->setText(QApplication::translate("StdServiceAuth", "Show log", 0, QApplication::UnicodeUTF8));
        cmdOK->setText(QApplication::translate("StdServiceAuth", "C&onnect", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StdServiceAuth: public Ui_StdServiceAuth {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STDSERVICEAUTH_H
