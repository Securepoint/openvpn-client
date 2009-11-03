/********************************************************************************
** Form generated from reading ui file 'configexport.ui'
**
** Created: Tue 3. Nov 16:14:15 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
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
        ConfigExport->resize(272, 211);
        ConfigExport->setMinimumSize(QSize(272, 211));
        ConfigExport->setMaximumSize(QSize(272, 211));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ConfigExport->setWindowIcon(icon);
        ConfigExport->setModal(true);
        groupBox = new QGroupBox(ConfigExport);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(14, 10, 251, 81));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(12, 48, 61, 16));
        txtExportPwd = new QLineEdit(groupBox);
        txtExportPwd->setObjectName(QString::fromUtf8("txtExportPwd"));
        txtExportPwd->setGeometry(QRect(66, 46, 171, 20));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(12, 24, 46, 14));
        txtSaveTo = new QLineEdit(groupBox);
        txtSaveTo->setObjectName(QString::fromUtf8("txtSaveTo"));
        txtSaveTo->setGeometry(QRect(67, 19, 151, 20));
        cmdOpenDir = new QPushButton(groupBox);
        cmdOpenDir->setObjectName(QString::fromUtf8("cmdOpenDir"));
        cmdOpenDir->setGeometry(QRect(225, 18, 20, 21));
        plainTextEdit = new QPlainTextEdit(ConfigExport);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));
        plainTextEdit->setGeometry(QRect(14, 99, 251, 71));
        plainTextEdit->setContextMenuPolicy(Qt::NoContextMenu);
        plainTextEdit->setStyleSheet(QString::fromUtf8("background-color: rgb(197, 197, 197);"));
        plainTextEdit->setFrameShape(QFrame::Box);
        plainTextEdit->setFrameShadow(QFrame::Plain);
        plainTextEdit->setReadOnly(true);
        cmdExport = new QPushButton(ConfigExport);
        cmdExport->setObjectName(QString::fromUtf8("cmdExport"));
        cmdExport->setGeometry(QRect(94, 179, 75, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/export.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdExport->setIcon(icon1);
        cmdExport->setFlat(true);
        cmdCancel = new QPushButton(ConfigExport);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));
        cmdCancel->setGeometry(QRect(190, 179, 75, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdCancel->setIcon(icon2);
        cmdCancel->setIconSize(QSize(30, 16));
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
        cmdExport->setText(QApplication::translate("ConfigExport", "E&xport", 0, QApplication::UnicodeUTF8));
        cmdCancel->setText(QString());
        Q_UNUSED(ConfigExport);
    } // retranslateUi

};

namespace Ui {
    class ConfigExport: public Ui_ConfigExport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGEXPORT_H
