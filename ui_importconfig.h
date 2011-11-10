/********************************************************************************
** Form generated from reading UI file 'importconfig.ui'
**
** Created: Tue 16. Nov 12:52:15 2010
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPORTCONFIG_H
#define UI_IMPORTCONFIG_H

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

class Ui_ImportConfig
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *txtImportPath;
    QPushButton *cmdOpenCryptFile;
    QGroupBox *groupBox_2;
    QRadioButton *rbSaveAsFile;
    QRadioButton *rbSaveAsName;
    QLineEdit *txtNewName;
    QPushButton *cmdCancel;
    QPushButton *cmdImport;
    QGroupBox *groupBox_3;
    QLabel *label_2;
    QLineEdit *txtPassword;
    QGroupBox *groupBox_4;
    QLabel *label_3;
    QLineEdit *txtExistingOvpn;
    QPushButton *cmdOpenOvpnFile;

    void setupUi(QDialog *ImportConfig)
    {
        if (ImportConfig->objectName().isEmpty())
            ImportConfig->setObjectName(QString::fromUtf8("ImportConfig"));
        ImportConfig->setWindowModality(Qt::ApplicationModal);
        ImportConfig->resize(370, 350);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ImportConfig->sizePolicy().hasHeightForWidth());
        ImportConfig->setSizePolicy(sizePolicy);
        ImportConfig->setMinimumSize(QSize(370, 350));
        ImportConfig->setMaximumSize(QSize(370, 350));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        ImportConfig->setWindowIcon(icon);
        ImportConfig->setStyleSheet(QString::fromUtf8(""));
        ImportConfig->setModal(true);
        groupBox = new QGroupBox(ImportConfig);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 4, 351, 81));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 191, 16));
        txtImportPath = new QLineEdit(groupBox);
        txtImportPath->setObjectName(QString::fromUtf8("txtImportPath"));
        txtImportPath->setGeometry(QRect(10, 40, 310, 20));
        txtImportPath->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":hover {border: 1px solid red; }"));
        cmdOpenCryptFile = new QPushButton(groupBox);
        cmdOpenCryptFile->setObjectName(QString::fromUtf8("cmdOpenCryptFile"));
        cmdOpenCryptFile->setGeometry(QRect(323, 40, 20, 20));
        cmdOpenCryptFile->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        groupBox_2 = new QGroupBox(ImportConfig);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 90, 351, 81));
        rbSaveAsFile = new QRadioButton(groupBox_2);
        rbSaveAsFile->setObjectName(QString::fromUtf8("rbSaveAsFile"));
        rbSaveAsFile->setGeometry(QRect(11, 22, 151, 19));
        rbSaveAsFile->setStyleSheet(QString::fromUtf8("QRadioButton { \n"
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
        rbSaveAsFile->setChecked(true);
        rbSaveAsName = new QRadioButton(groupBox_2);
        rbSaveAsName->setObjectName(QString::fromUtf8("rbSaveAsName"));
        rbSaveAsName->setGeometry(QRect(11, 47, 41, 19));
        rbSaveAsName->setStyleSheet(QString::fromUtf8("QRadioButton { \n"
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
        txtNewName = new QLineEdit(groupBox_2);
        txtNewName->setObjectName(QString::fromUtf8("txtNewName"));
        txtNewName->setEnabled(false);
        txtNewName->setGeometry(QRect(50, 47, 151, 20));
        txtNewName->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }\n"
""));
        cmdCancel = new QPushButton(ImportConfig);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));
        cmdCancel->setGeometry(QRect(260, 313, 100, 33));
        cmdCancel->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/close_dialog.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdCancel->setIcon(icon1);
        cmdCancel->setIconSize(QSize(22, 22));
        cmdCancel->setFlat(true);
        cmdImport = new QPushButton(ImportConfig);
        cmdImport->setObjectName(QString::fromUtf8("cmdImport"));
        cmdImport->setGeometry(QRect(150, 313, 100, 33));
        cmdImport->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/import.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdImport->setIcon(icon2);
        cmdImport->setIconSize(QSize(22, 22));
        cmdImport->setFlat(true);
        groupBox_3 = new QGroupBox(ImportConfig);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 173, 351, 51));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 22, 51, 16));
        txtPassword = new QLineEdit(groupBox_3);
        txtPassword->setObjectName(QString::fromUtf8("txtPassword"));
        txtPassword->setGeometry(QRect(65, 21, 201, 20));
        txtPassword->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":hover {border: 1px solid red; }"));
        groupBox_4 = new QGroupBox(ImportConfig);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(10, 230, 351, 80));
        label_3 = new QLabel(groupBox_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(12, 20, 161, 16));
        txtExistingOvpn = new QLineEdit(groupBox_4);
        txtExistingOvpn->setObjectName(QString::fromUtf8("txtExistingOvpn"));
        txtExistingOvpn->setGeometry(QRect(10, 40, 310, 20));
        txtExistingOvpn->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":hover {border: 1px solid red; }"));
        cmdOpenOvpnFile = new QPushButton(groupBox_4);
        cmdOpenOvpnFile->setObjectName(QString::fromUtf8("cmdOpenOvpnFile"));
        cmdOpenOvpnFile->setGeometry(QRect(323, 40, 20, 20));
        cmdOpenOvpnFile->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));

        retranslateUi(ImportConfig);

        QMetaObject::connectSlotsByName(ImportConfig);
    } // setupUi

    void retranslateUi(QDialog *ImportConfig)
    {
        ImportConfig->setWindowTitle(QApplication::translate("ImportConfig", "Import config file", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("ImportConfig", "File path:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ImportConfig", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Path to the import file [*.crypt]:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        cmdOpenCryptFile->setText(QApplication::translate("ImportConfig", "...", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("ImportConfig", "Save as:", 0, QApplication::UnicodeUTF8));
        rbSaveAsFile->setText(QApplication::translate("ImportConfig", "as filename", 0, QApplication::UnicodeUTF8));
        rbSaveAsName->setText(QApplication::translate("ImportConfig", "as:", 0, QApplication::UnicodeUTF8));
        cmdCancel->setText(QApplication::translate("ImportConfig", "     &Close", 0, QApplication::UnicodeUTF8));
        cmdImport->setText(QApplication::translate("ImportConfig", "     &Import", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("ImportConfig", "Password:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ImportConfig", "Password:", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("ImportConfig", "Import existing configuration:", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ImportConfig", "Path to the *.ovpn file:", 0, QApplication::UnicodeUTF8));
        cmdOpenOvpnFile->setText(QApplication::translate("ImportConfig", "...", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImportConfig: public Ui_ImportConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORTCONFIG_H
