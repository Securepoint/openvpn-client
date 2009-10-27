/********************************************************************************
** Form generated from reading ui file 'importconfig.ui'
**
** Created: Tue 27. Oct 16:47:15 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
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
#include <QtGui/QPlainTextEdit>
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
    QPlainTextEdit *plainTextEdit_2;
    QPushButton *cmdCancel;
    QPushButton *cmdImport;
    QGroupBox *groupBox_3;
    QLabel *label_2;
    QLineEdit *txtPassword;

    void setupUi(QDialog *ImportConfig)
    {
        if (ImportConfig->objectName().isEmpty())
            ImportConfig->setObjectName(QString::fromUtf8("ImportConfig"));
        ImportConfig->resize(370, 355);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
        ImportConfig->setWindowIcon(icon);
        groupBox = new QGroupBox(ImportConfig);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 4, 351, 81));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 141, 16));
        txtImportPath = new QLineEdit(groupBox);
        txtImportPath->setObjectName(QString::fromUtf8("txtImportPath"));
        txtImportPath->setGeometry(QRect(10, 40, 311, 20));
        cmdOpenCryptFile = new QPushButton(groupBox);
        cmdOpenCryptFile->setObjectName(QString::fromUtf8("cmdOpenCryptFile"));
        cmdOpenCryptFile->setGeometry(QRect(321, 40, 21, 21));
        groupBox_2 = new QGroupBox(ImportConfig);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 90, 351, 81));
        rbSaveAsFile = new QRadioButton(groupBox_2);
        rbSaveAsFile->setObjectName(QString::fromUtf8("rbSaveAsFile"));
        rbSaveAsFile->setGeometry(QRect(11, 22, 82, 19));
        rbSaveAsFile->setChecked(true);
        rbSaveAsName = new QRadioButton(groupBox_2);
        rbSaveAsName->setObjectName(QString::fromUtf8("rbSaveAsName"));
        rbSaveAsName->setGeometry(QRect(11, 47, 41, 19));
        txtNewName = new QLineEdit(groupBox_2);
        txtNewName->setObjectName(QString::fromUtf8("txtNewName"));
        txtNewName->setEnabled(false);
        txtNewName->setGeometry(QRect(50, 47, 151, 20));
        plainTextEdit_2 = new QPlainTextEdit(ImportConfig);
        plainTextEdit_2->setObjectName(QString::fromUtf8("plainTextEdit_2"));
        plainTextEdit_2->setEnabled(true);
        plainTextEdit_2->setGeometry(QRect(10, 231, 351, 81));
        plainTextEdit_2->setContextMenuPolicy(Qt::NoContextMenu);
        plainTextEdit_2->setStyleSheet(QString::fromUtf8("background-color: rgb(197, 197, 197);"));
        plainTextEdit_2->setFrameShape(QFrame::Box);
        plainTextEdit_2->setFrameShadow(QFrame::Plain);
        plainTextEdit_2->setReadOnly(true);
        cmdCancel = new QPushButton(ImportConfig);
        cmdCancel->setObjectName(QString::fromUtf8("cmdCancel"));
        cmdCancel->setGeometry(QRect(280, 323, 75, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/close.svg"), QSize(), QIcon::Normal, QIcon::Off);
        cmdCancel->setIcon(icon1);
        cmdCancel->setFlat(true);
        cmdImport = new QPushButton(ImportConfig);
        cmdImport->setObjectName(QString::fromUtf8("cmdImport"));
        cmdImport->setGeometry(QRect(180, 322, 75, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/import.svg"), QSize(), QIcon::Normal, QIcon::Off);
        cmdImport->setIcon(icon2);
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
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">Path to import file [*.crypt]:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        cmdOpenCryptFile->setText(QApplication::translate("ImportConfig", "...", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("ImportConfig", "Save as:", 0, QApplication::UnicodeUTF8));
        rbSaveAsFile->setText(QApplication::translate("ImportConfig", "as filename", 0, QApplication::UnicodeUTF8));
        rbSaveAsName->setText(QApplication::translate("ImportConfig", "as:", 0, QApplication::UnicodeUTF8));
        plainTextEdit_2->setPlainText(QApplication::translate("ImportConfig", "Please notice. If you import a OpenVPN config in portable mode, the new config will be automatically imported in the application directory. In the installation mode the default is to import the new config in the home directory of the user. \n"
"", 0, QApplication::UnicodeUTF8));
        cmdCancel->setText(QApplication::translate("ImportConfig", "&Cancel", 0, QApplication::UnicodeUTF8));
        cmdImport->setText(QApplication::translate("ImportConfig", "&Import", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("ImportConfig", "Password:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ImportConfig", "Password:", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ImportConfig);
    } // retranslateUi

};

namespace Ui {
    class ImportConfig: public Ui_ImportConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORTCONFIG_H
