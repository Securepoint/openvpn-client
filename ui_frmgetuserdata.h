/********************************************************************************
** Form generated from reading UI file 'frmgetuserdata.ui'
**
** Created: Thu 15. Apr 13:48:01 2010
**      by: Qt User Interface Compiler version 4.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMGETUSERDATA_H
#define UI_FRMGETUSERDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FrmGetUserData
{
public:
    QGroupBox *groupBox;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *lblDescription;
    QLineEdit *txtDataField;
    QCheckBox *cbSaveData;
    QPushButton *cmdClose;
    QPushButton *cmdOK;

    void setupUi(QDialog *FrmGetUserData)
    {
        if (FrmGetUserData->objectName().isEmpty())
            FrmGetUserData->setObjectName(QString::fromUtf8("FrmGetUserData"));
        FrmGetUserData->setWindowModality(Qt::ApplicationModal);
        FrmGetUserData->resize(330, 140);
        FrmGetUserData->setMinimumSize(QSize(330, 140));
        FrmGetUserData->setMaximumSize(QSize(330, 140));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        FrmGetUserData->setWindowIcon(icon);
        FrmGetUserData->setModal(true);
        groupBox = new QGroupBox(FrmGetUserData);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 311, 81));
        horizontalLayoutWidget = new QWidget(groupBox);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 20, 291, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        lblDescription = new QLabel(horizontalLayoutWidget);
        lblDescription->setObjectName(QString::fromUtf8("lblDescription"));

        horizontalLayout->addWidget(lblDescription);

        txtDataField = new QLineEdit(horizontalLayoutWidget);
        txtDataField->setObjectName(QString::fromUtf8("txtDataField"));
        txtDataField->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black; background-color: rgb(255, 255, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);}\n"
":hover {border: 1px solid red; }"));

        horizontalLayout->addWidget(txtDataField);

        cbSaveData = new QCheckBox(groupBox);
        cbSaveData->setObjectName(QString::fromUtf8("cbSaveData"));
        cbSaveData->setGeometry(QRect(11, 57, 291, 18));
        cbSaveData->setStyleSheet(QString::fromUtf8("QCheckBox { \n"
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
        cmdClose = new QPushButton(FrmGetUserData);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(220, 100, 100, 33));
        cmdClose->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        cmdClose->setFlat(true);
        cmdOK = new QPushButton(FrmGetUserData);
        cmdOK->setObjectName(QString::fromUtf8("cmdOK"));
        cmdOK->setGeometry(QRect(110, 100, 100, 33));
        cmdOK->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        cmdOK->setDefault(true);
        cmdOK->setFlat(true);

        retranslateUi(FrmGetUserData);

        QMetaObject::connectSlotsByName(FrmGetUserData);
    } // setupUi

    void retranslateUi(QDialog *FrmGetUserData)
    {
        FrmGetUserData->setWindowTitle(QApplication::translate("FrmGetUserData", "User data requested", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("FrmGetUserData", "Please insert:", 0, QApplication::UnicodeUTF8));
        lblDescription->setText(QApplication::translate("FrmGetUserData", "Username:", 0, QApplication::UnicodeUTF8));
        cbSaveData->setText(QApplication::translate("FrmGetUserData", "Save data [only possible with -useCryptedData ]", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QApplication::translate("FrmGetUserData", "Cancel", 0, QApplication::UnicodeUTF8));
        cmdOK->setText(QApplication::translate("FrmGetUserData", "OK", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FrmGetUserData: public Ui_FrmGetUserData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMGETUSERDATA_H
