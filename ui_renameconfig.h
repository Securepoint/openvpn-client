/********************************************************************************
** Form generated from reading UI file 'renameconfig.ui'
**
** Created: Tue 16. Nov 12:52:14 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENAMECONFIG_H
#define UI_RENAMECONFIG_H

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

class Ui_RenameConfig
{
public:
    QGroupBox *groupBox;
    QLabel *lblOldName;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *txtNewName;
    QPushButton *cmdClose;
    QPushButton *cmdRename;

    void setupUi(QDialog *RenameConfig)
    {
        if (RenameConfig->objectName().isEmpty())
            RenameConfig->setObjectName(QString::fromUtf8("RenameConfig"));
        RenameConfig->setWindowModality(Qt::ApplicationModal);
        RenameConfig->resize(280, 120);
        RenameConfig->setMinimumSize(QSize(200, 120));
        RenameConfig->setMaximumSize(QSize(280, 120));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        RenameConfig->setWindowIcon(icon);
        RenameConfig->setModal(true);
        groupBox = new QGroupBox(RenameConfig);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 2, 261, 71));
        lblOldName = new QLabel(groupBox);
        lblOldName->setObjectName(QString::fromUtf8("lblOldName"));
        lblOldName->setGeometry(QRect(70, 20, 171, 16));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 61, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 40, 61, 16));
        txtNewName = new QLineEdit(groupBox);
        txtNewName->setObjectName(QString::fromUtf8("txtNewName"));
        txtNewName->setGeometry(QRect(70, 39, 181, 20));
        txtNewName->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":hover {border: 1px solid red; }"));
        cmdClose = new QPushButton(RenameConfig);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(171, 80, 100, 33));
        cmdClose->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/close_dialog.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon1);
        cmdClose->setIconSize(QSize(22, 22));
        cmdClose->setFlat(true);
        cmdRename = new QPushButton(RenameConfig);
        cmdRename->setObjectName(QString::fromUtf8("cmdRename"));
        cmdRename->setGeometry(QRect(60, 80, 100, 33));
        cmdRename->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/rename.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdRename->setIcon(icon2);
        cmdRename->setIconSize(QSize(22, 22));
        cmdRename->setDefault(true);
        cmdRename->setFlat(true);

        retranslateUi(RenameConfig);

        QMetaObject::connectSlotsByName(RenameConfig);
    } // setupUi

    void retranslateUi(QDialog *RenameConfig)
    {
        RenameConfig->setWindowTitle(QApplication::translate("RenameConfig", "Rename configuration", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("RenameConfig", "Rename configuration:", 0, QApplication::UnicodeUTF8));
        lblOldName->setText(QApplication::translate("RenameConfig", "oldname", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RenameConfig", "Old name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RenameConfig", "New name:", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QApplication::translate("RenameConfig", "     &Close", 0, QApplication::UnicodeUTF8));
        cmdRename->setText(QApplication::translate("RenameConfig", "     &Rename", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RenameConfig: public Ui_RenameConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENAMECONFIG_H
