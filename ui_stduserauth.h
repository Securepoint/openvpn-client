/********************************************************************************
** Form generated from reading ui file 'stduserauth.ui'
**
** Created: Wed 4. Nov 17:16:48 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_STDUSERAUTH_H
#define UI_STDUSERAUTH_H

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

class Ui_StdUserAuth
{
public:
    QPushButton *cmdOK;
    QPushButton *cmdClose;
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *txtUser;
    QLineEdit *txtPwd;
    QCheckBox *cbSavePassword;
    QCheckBox *cbShowLog;

    void setupUi(QDialog *StdUserAuth)
    {
        if (StdUserAuth->objectName().isEmpty())
            StdUserAuth->setObjectName(QString::fromUtf8("StdUserAuth"));
        StdUserAuth->setWindowModality(Qt::WindowModal);
        StdUserAuth->resize(220, 168);
        StdUserAuth->setMinimumSize(QSize(220, 168));
        StdUserAuth->setMaximumSize(QSize(220, 168));
        StdUserAuth->setContextMenuPolicy(Qt::NoContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
        StdUserAuth->setWindowIcon(icon);
        StdUserAuth->setModal(true);
        cmdOK = new QPushButton(StdUserAuth);
        cmdOK->setObjectName(QString::fromUtf8("cmdOK"));
        cmdOK->setGeometry(QRect(54, 139, 75, 24));
        cmdOK->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/connectmen.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdOK->setIcon(icon1);
        cmdOK->setFlat(true);
        cmdClose = new QPushButton(StdUserAuth);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(135, 139, 75, 24));
        cmdClose->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon2);
        cmdClose->setIconSize(QSize(16, 16));
        cmdClose->setFlat(true);
        groupBox = new QGroupBox(StdUserAuth);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 201, 121));
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
        cbSavePassword = new QCheckBox(groupBox);
        cbSavePassword->setObjectName(QString::fromUtf8("cbSavePassword"));
        cbSavePassword->setGeometry(QRect(10, 69, 161, 19));
        cbSavePassword->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
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
        cbShowLog = new QCheckBox(groupBox);
        cbShowLog->setObjectName(QString::fromUtf8("cbShowLog"));
        cbShowLog->setGeometry(QRect(10, 90, 141, 19));
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

        retranslateUi(StdUserAuth);

        QMetaObject::connectSlotsByName(StdUserAuth);
    } // setupUi

    void retranslateUi(QDialog *StdUserAuth)
    {
        StdUserAuth->setWindowTitle(QApplication::translate("StdUserAuth", "User Authentication", 0, QApplication::UnicodeUTF8));
        cmdOK->setText(QApplication::translate("StdUserAuth", "C&onnect", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QApplication::translate("StdUserAuth", "&Close", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("StdUserAuth", "User data:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StdUserAuth", "Username:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("StdUserAuth", "Password:", 0, QApplication::UnicodeUTF8));
        cbSavePassword->setText(QApplication::translate("StdUserAuth", "Remember username", 0, QApplication::UnicodeUTF8));
        cbShowLog->setText(QApplication::translate("StdUserAuth", "Show log", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(StdUserAuth);
    } // retranslateUi

};

namespace Ui {
    class StdUserAuth: public Ui_StdUserAuth {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STDUSERAUTH_H
