/********************************************************************************
** Form generated from reading ui file 'renameconfig.ui'
**
** Created: Wed 4. Nov 17:16:48 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
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
    QPushButton *cmdRename;
    QPushButton *cmdClose;

    void setupUi(QDialog *RenameConfig)
    {
        if (RenameConfig->objectName().isEmpty())
            RenameConfig->setObjectName(QString::fromUtf8("RenameConfig"));
        RenameConfig->resize(368, 114);
        RenameConfig->setMinimumSize(QSize(368, 114));
        RenameConfig->setMaximumSize(QSize(368, 114));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
        RenameConfig->setWindowIcon(icon);
        RenameConfig->setModal(true);
        groupBox = new QGroupBox(RenameConfig);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 351, 71));
        lblOldName = new QLabel(groupBox);
        lblOldName->setObjectName(QString::fromUtf8("lblOldName"));
        lblOldName->setGeometry(QRect(70, 20, 91, 16));
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
        cmdRename = new QPushButton(groupBox);
        cmdRename->setObjectName(QString::fromUtf8("cmdRename"));
        cmdRename->setGeometry(QRect(259, 37, 75, 24));
        cmdRename->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/rename.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdRename->setIcon(icon1);
        cmdRename->setFlat(true);
        cmdClose = new QPushButton(RenameConfig);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(280, 80, 75, 24));
        cmdClose->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon2);
        cmdClose->setIconSize(QSize(16, 16));
        cmdClose->setFlat(true);

        retranslateUi(RenameConfig);

        QMetaObject::connectSlotsByName(RenameConfig);
    } // setupUi

    void retranslateUi(QDialog *RenameConfig)
    {
        RenameConfig->setWindowTitle(QApplication::translate("RenameConfig", "Securepoint VPN Client", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("RenameConfig", "Rename configuration:", 0, QApplication::UnicodeUTF8));
        lblOldName->setText(QApplication::translate("RenameConfig", "oldname", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RenameConfig", "Old name:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("RenameConfig", "New name:", 0, QApplication::UnicodeUTF8));
        cmdRename->setText(QApplication::translate("RenameConfig", "Rename", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QApplication::translate("RenameConfig", "&Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(RenameConfig);
    } // retranslateUi

};

namespace Ui {
    class RenameConfig: public Ui_RenameConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENAMECONFIG_H
