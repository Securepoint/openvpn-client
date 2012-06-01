/********************************************************************************
** Form generated from reading UI file 'frmsaveddata.ui'
**
** Created: Thu 12. Jan 13:39:29 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRMSAVEDDATA_H
#define UI_FRMSAVEDDATA_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTreeWidget>

QT_BEGIN_NAMESPACE

class Ui_FrmSavedData
{
public:
    QGroupBox *groupBox;
    QTreeWidget *trvSavedData;
    QPushButton *pushButton;

    void setupUi(QDialog *FrmSavedData)
    {
        if (FrmSavedData->objectName().isEmpty())
            FrmSavedData->setObjectName(QString::fromUtf8("FrmSavedData"));
        FrmSavedData->resize(454, 269);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        FrmSavedData->setWindowIcon(icon);
        groupBox = new QGroupBox(FrmSavedData);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(6, 4, 441, 221));
        trvSavedData = new QTreeWidget(groupBox);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        trvSavedData->setHeaderItem(__qtreewidgetitem);
        trvSavedData->setObjectName(QString::fromUtf8("trvSavedData"));
        trvSavedData->setGeometry(QRect(10, 20, 421, 191));
        pushButton = new QPushButton(FrmSavedData);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(347, 230, 100, 33));
        pushButton->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/close_dialog.png"), QSize(), QIcon::Normal, QIcon::Off);
        pushButton->setIcon(icon1);

        retranslateUi(FrmSavedData);

        QMetaObject::connectSlotsByName(FrmSavedData);
    } // setupUi

    void retranslateUi(QDialog *FrmSavedData)
    {
        FrmSavedData->setWindowTitle(QApplication::translate("FrmSavedData", "Saved Data", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("FrmSavedData", "Saved configuration data:", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("FrmSavedData", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FrmSavedData: public Ui_FrmSavedData {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRMSAVEDDATA_H
