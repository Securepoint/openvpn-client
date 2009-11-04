/********************************************************************************
** Form generated from reading ui file 'tapinfo.ui'
**
** Created: Wed 4. Nov 17:16:48 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_TAPINFO_H
#define UI_TAPINFO_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_TapInfo
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QLabel *label_2;
    QLabel *lblIncommingTotal;
    QLabel *lblOutgoingTotal;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *lblIncommingPerSecond;
    QLabel *lblOutgoingPerSecond;
    QPushButton *cmdClose;

    void setupUi(QDialog *TapInfo)
    {
        if (TapInfo->objectName().isEmpty())
            TapInfo->setObjectName(QString::fromUtf8("TapInfo"));
        TapInfo->resize(279, 190);
        TapInfo->setMinimumSize(QSize(279, 190));
        TapInfo->setMaximumSize(QSize(279, 190));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
        TapInfo->setWindowIcon(icon);
        TapInfo->setModal(true);
        groupBox = new QGroupBox(TapInfo);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 261, 141));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 131, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(20, 51, 131, 16));
        lblIncommingTotal = new QLabel(groupBox);
        lblIncommingTotal->setObjectName(QString::fromUtf8("lblIncommingTotal"));
        lblIncommingTotal->setGeometry(QRect(160, 31, 91, 16));
        lblOutgoingTotal = new QLabel(groupBox);
        lblOutgoingTotal->setObjectName(QString::fromUtf8("lblOutgoingTotal"));
        lblOutgoingTotal->setGeometry(QRect(160, 50, 71, 20));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(20, 80, 161, 20));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(20, 100, 161, 20));
        lblIncommingPerSecond = new QLabel(groupBox);
        lblIncommingPerSecond->setObjectName(QString::fromUtf8("lblIncommingPerSecond"));
        lblIncommingPerSecond->setGeometry(QRect(180, 83, 61, 16));
        lblOutgoingPerSecond = new QLabel(groupBox);
        lblOutgoingPerSecond->setObjectName(QString::fromUtf8("lblOutgoingPerSecond"));
        lblOutgoingPerSecond->setGeometry(QRect(180, 101, 91, 20));
        cmdClose = new QPushButton(TapInfo);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(191, 159, 75, 24));
        cmdClose->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon1);
        cmdClose->setIconSize(QSize(16, 16));
        cmdClose->setFlat(true);

        retranslateUi(TapInfo);

        QMetaObject::connectSlotsByName(TapInfo);
    } // setupUi

    void retranslateUi(QDialog *TapInfo)
    {
        TapInfo->setWindowTitle(QApplication::translate("TapInfo", "Tap driver", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("TapInfo", "TAP-Win32 Adapter:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("TapInfo", "Incomming packets total:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("TapInfo", "Outgoing packets total:", 0, QApplication::UnicodeUTF8));
        lblIncommingTotal->setText(QApplication::translate("TapInfo", "0", 0, QApplication::UnicodeUTF8));
        lblOutgoingTotal->setText(QApplication::translate("TapInfo", "0", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("TapInfo", "Incomming packets per second:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("TapInfo", "Outgoing packets per second:", 0, QApplication::UnicodeUTF8));
        lblIncommingPerSecond->setText(QApplication::translate("TapInfo", "0", 0, QApplication::UnicodeUTF8));
        lblOutgoingPerSecond->setText(QApplication::translate("TapInfo", "0", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QApplication::translate("TapInfo", "&Close", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(TapInfo);
    } // retranslateUi

};

namespace Ui {
    class TapInfo: public Ui_TapInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TAPINFO_H
