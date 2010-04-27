/********************************************************************************
** Form generated from reading UI file 'stduserauth.ui'
**
** Created: Tue 23. Mar 14:00:30 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STDUSERAUTH_H
#define UI_STDUSERAUTH_H

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

QT_BEGIN_NAMESPACE

class Ui_StdUserAuth
{
public:
    QPushButton *cmdOK;
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *txtUser;
    QLineEdit *txtPwd;

    void setupUi(QDialog *StdUserAuth)
    {
        if (StdUserAuth->objectName().isEmpty())
            StdUserAuth->setObjectName(QString::fromUtf8("StdUserAuth"));
        StdUserAuth->setWindowModality(Qt::WindowModal);
        StdUserAuth->resize(220, 118);
        StdUserAuth->setMinimumSize(QSize(0, 0));
        StdUserAuth->setMaximumSize(QSize(220, 168));
        StdUserAuth->setContextMenuPolicy(Qt::NoContextMenu);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
        StdUserAuth->setWindowIcon(icon);
        StdUserAuth->setModal(true);
        cmdOK = new QPushButton(StdUserAuth);
        cmdOK->setObjectName(QString::fromUtf8("cmdOK"));
        cmdOK->setGeometry(QRect(140, 80, 75, 31));
        cmdOK->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/connectmen.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdOK->setIcon(icon1);
        cmdOK->setFlat(true);
        groupBox = new QGroupBox(StdUserAuth);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 201, 71));
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

        retranslateUi(StdUserAuth);

        QMetaObject::connectSlotsByName(StdUserAuth);
    } // setupUi

    void retranslateUi(QDialog *StdUserAuth)
    {
        StdUserAuth->setWindowTitle(QApplication::translate("StdUserAuth", "User Authentication", 0, QApplication::UnicodeUTF8));
        cmdOK->setText(QApplication::translate("StdUserAuth", "C&onnect", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("StdUserAuth", "User data:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("StdUserAuth", "Username:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("StdUserAuth", "Password:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class StdUserAuth: public Ui_StdUserAuth {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STDUSERAUTH_H
