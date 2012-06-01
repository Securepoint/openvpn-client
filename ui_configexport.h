/********************************************************************************
** Form generated from reading UI file 'configexport.ui'
**
** Created: Fri 13. Jan 11:16:40 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGEXPORT_H
#define UI_CONFIGEXPORT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ConfigExport
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *txtExportPwd;
    QLabel *label_2;
    QLineEdit *txtSaveTo;
    QPushButton *cmdOpenDir;
    QPlainTextEdit *plainTextEdit;
    QPushButton *cmdExport;
    QPushButton *cmdCancel;

    void setupUi(QDialog *ConfigExport)
    {
        if (ConfigExport->objectName().isEmpty())
            ConfigExport->setObjectName(QString::fromUtf8("ConfigExport"));
        ConfigExport->setWindowModality(Qt::ApplicationModal);
        ConfigExport->resize(270, 200);
        ConfigExport->setMinimumSize(QSize(270, 200));
        ConfigExport->setMaximumSize(QSize(272, 200));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        ConfigExport->setWindowIcon(icon);
        ConfigExport->setStyleSheet(QString::fromUtf8(""));
        ConfigExport->setModal(true);
        groupBox = new QGroupBox(ConfigExport);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 4, 251, 81));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(12, 48, 61, 16));
        txtExportPwd = new QLineEdit(groupBox);
        txtExportPwd->setObjectName(QString::fromUtf8("txtExportPwd"));
        txtExportPwd->setGeometry(QRect(76, 46, 161, 20));
        txtExportPwd->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(12, 22, 61, 16));
        txtSaveTo = new QLineEdit(groupBox);
        txtSaveTo->setObjectName(QString::fromUtf8("txtSaveTo"));
        txtSaveTo->setGeometry(QRect(77, 19, 141, 20));
        txtSaveTo->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }\n"
"QLineEdit {border-radius: 3px;}"));
        cmdOpenDir = new QPushButton(groupBox);
        cmdOpenDir->setObjectName(QString::fromUtf8("cmdOpenDir"));
        cmdOpenDir->setGeometry(QRect(222, 19, 20, 20));
        cmdOpenDir->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {border-radius: 3px;}"));
        plainTextEdit = new QPlainTextEdit(ConfigExport);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(10, 90, 251, 61));
        plainTextEdit->setContextMenuPolicy(Qt::NoContextMenu);
        plainTextEdit->setStyleSheet(QString::fromUtf8("\n"
"background-color: rgb(255, 170, 0);"));
        plainTextEdit->setFrameShape(QFrame::Box);
        plainTextEdit->setFrameShadow(QFrame::Plain);
        plainTextEdit->setReadOnly(true);
        cmdExport = new QPushButton(ConfigExport);
        cmdExport->setObjectName(QString::fromUtf8("cmdExport"));
        cmdExport->setGeometry(QRect(50, 159, 100, 33));
        cmdExport->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"border-radius: 3px;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}\n"
""));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/export.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdExport->setIcon(icon1);
        cmdExport->setIconSize(QSize(22, 22));
        cmdExport->setDefault(true);
        cmdExport->setFlat(true);
        cmdCancel = new QPushButton(ConfigExport);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));
        cmdCancel->setGeometry(QRect(160, 159, 100, 33));
        cmdCancel->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"border-radius: 3px;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/close_dialog.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdCancel->setIcon(icon2);
        cmdCancel->setIconSize(QSize(22, 22));
        cmdCancel->setFlat(true);

        retranslateUi(ConfigExport);

        QMetaObject::connectSlotsByName(ConfigExport);
    } // setupUi

    void retranslateUi(QDialog *ConfigExport)
    {
        ConfigExport->setWindowTitle(QApplication::translate("ConfigExport", "Export configuration", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ConfigExport", "Export:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ConfigExport", "Password:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ConfigExport", "Save to:", 0, QApplication::UnicodeUTF8));
        cmdOpenDir->setText(QApplication::translate("ConfigExport", "...", 0, QApplication::UnicodeUTF8));
        plainTextEdit->setPlainText(QApplication::translate("ConfigExport", "The exported config will be created into the [save to directory]/[configname].crypt.\n"
"", 0, QApplication::UnicodeUTF8));
        cmdExport->setText(QApplication::translate("ConfigExport", "     E&xport", 0, QApplication::UnicodeUTF8));
        cmdCancel->setText(QApplication::translate("ConfigExport", "     &Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ConfigExport: public Ui_ConfigExport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGEXPORT_H
