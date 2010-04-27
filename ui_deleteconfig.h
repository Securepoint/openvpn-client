/********************************************************************************
** Form generated from reading UI file 'deleteconfig.ui'
**
** Created: Thu 15. Apr 13:48:01 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DELETECONFIG_H
#define UI_DELETECONFIG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DeleteConfig
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QCheckBox *cbConfigFile;
    QCheckBox *cbCA;
    QCheckBox *cbCert;
    QCheckBox *cbKey;
    QCheckBox *cbUserdata;
    QCheckBox *cbScript;
    QCheckBox *cbDir;
    QPushButton *cmdDelete;
    QPushButton *cmdCancel;

    void setupUi(QDialog *DeleteConfig)
    {
        if (DeleteConfig->objectName().isEmpty())
            DeleteConfig->setObjectName(QString::fromUtf8("DeleteConfig"));
        DeleteConfig->setWindowModality(Qt::ApplicationModal);
        DeleteConfig->resize(230, 250);
        DeleteConfig->setMinimumSize(QSize(230, 250));
        DeleteConfig->setMaximumSize(QSize(230, 250));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        DeleteConfig->setWindowIcon(icon);
        DeleteConfig->setModal(true);
        groupBox = new QGroupBox(DeleteConfig);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(7, 7, 207, 191));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 41, 16));
        cbConfigFile = new QCheckBox(groupBox);
        cbConfigFile->setObjectName(QString::fromUtf8("cbConfigFile"));
        cbConfigFile->setGeometry(QRect(38, 40, 161, 19));
        cbConfigFile->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
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
        cbCA = new QCheckBox(groupBox);
        cbCA->setObjectName(QString::fromUtf8("cbCA"));
        cbCA->setGeometry(QRect(38, 60, 161, 19));
        cbCA->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
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
        cbCert = new QCheckBox(groupBox);
        cbCert->setObjectName(QString::fromUtf8("cbCert"));
        cbCert->setGeometry(QRect(38, 80, 161, 19));
        cbCert->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
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
        cbKey = new QCheckBox(groupBox);
        cbKey->setObjectName(QString::fromUtf8("cbKey"));
        cbKey->setGeometry(QRect(38, 100, 151, 19));
        cbKey->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
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
        cbUserdata = new QCheckBox(groupBox);
        cbUserdata->setObjectName(QString::fromUtf8("cbUserdata"));
        cbUserdata->setGeometry(QRect(38, 120, 161, 19));
        cbUserdata->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
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
        cbScript = new QCheckBox(groupBox);
        cbScript->setObjectName(QString::fromUtf8("cbScript"));
        cbScript->setGeometry(QRect(38, 140, 161, 19));
        cbScript->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
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
        cbDir = new QCheckBox(groupBox);
        cbDir->setObjectName(QString::fromUtf8("cbDir"));
        cbDir->setGeometry(QRect(38, 160, 161, 19));
        cbDir->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
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
        cmdDelete = new QPushButton(DeleteConfig);
        cmdDelete->setObjectName(QString::fromUtf8("cmdDelete"));
        cmdDelete->setGeometry(QRect(7, 208, 100, 33));
        cmdDelete->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdDelete->setIcon(icon1);
        cmdDelete->setIconSize(QSize(22, 22));
        cmdDelete->setDefault(true);
        cmdDelete->setFlat(true);
        cmdCancel = new QPushButton(DeleteConfig);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));
        cmdCancel->setGeometry(QRect(113, 208, 100, 33));
        cmdCancel->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/close_dialog.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdCancel->setIcon(icon2);
        cmdCancel->setIconSize(QSize(16, 16));
        cmdCancel->setFlat(true);

        retranslateUi(DeleteConfig);

        QMetaObject::connectSlotsByName(DeleteConfig);
    } // setupUi

    void retranslateUi(QDialog *DeleteConfig)
    {
        DeleteConfig->setWindowTitle(QApplication::translate("DeleteConfig", "Delete config", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("DeleteConfig", "Delete settings:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("DeleteConfig", "Delete:", 0, QApplication::UnicodeUTF8));
        cbConfigFile->setText(QApplication::translate("DeleteConfig", "Config", 0, QApplication::UnicodeUTF8));
        cbCA->setText(QApplication::translate("DeleteConfig", "CA [.pem]", 0, QApplication::UnicodeUTF8));
        cbCert->setText(QApplication::translate("DeleteConfig", "Certificate [.pem]", 0, QApplication::UnicodeUTF8));
        cbKey->setText(QApplication::translate("DeleteConfig", "Key [.key]", 0, QApplication::UnicodeUTF8));
        cbUserdata->setText(QApplication::translate("DeleteConfig", "Userdata [auth.dat]", 0, QApplication::UnicodeUTF8));
        cbScript->setText(QApplication::translate("DeleteConfig", "Scripts [scripts.conf]", 0, QApplication::UnicodeUTF8));
        cbDir->setText(QApplication::translate("DeleteConfig", "Directory if empty", 0, QApplication::UnicodeUTF8));
        cmdDelete->setText(QApplication::translate("DeleteConfig", "     &Delete", 0, QApplication::UnicodeUTF8));
        cmdCancel->setText(QApplication::translate("DeleteConfig", "     &Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class DeleteConfig: public Ui_DeleteConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELETECONFIG_H
