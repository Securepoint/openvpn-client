/********************************************************************************
** Form generated from reading ui file 'vpnlog.ui'
**
** Created: Mon 2. Nov 14:42:19 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_VPNLOG_H
#define UI_VPNLOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_VpnLog
{
public:
    QGroupBox *groupBox;
    QPlainTextEdit *memLog;
    QPushButton *cmdClose;
    QPushButton *cmdSave;

    void setupUi(QDialog *VpnLog)
    {
        if (VpnLog->objectName().isEmpty())
            VpnLog->setObjectName(QString::fromUtf8("VpnLog"));
        VpnLog->resize(800, 400);
        VpnLog->setMinimumSize(QSize(800, 400));
        VpnLog->setMaximumSize(QSize(800, 400));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
        VpnLog->setWindowIcon(icon);
        VpnLog->setModal(true);
        groupBox = new QGroupBox(VpnLog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 781, 361));
        memLog = new QPlainTextEdit(groupBox);
        memLog->setObjectName(QString::fromUtf8("memLog"));
        memLog->setGeometry(QRect(10, 20, 761, 331));
        memLog->setFrameShape(QFrame::Box);
        memLog->setFrameShadow(QFrame::Plain);
        memLog->setUndoRedoEnabled(false);
        memLog->setReadOnly(true);
        cmdClose = new QPushButton(VpnLog);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(700, 369, 75, 24));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon1);
        cmdClose->setIconSize(QSize(30, 16));
        cmdClose->setFlat(true);
        cmdSave = new QPushButton(VpnLog);
        cmdSave->setObjectName(QString::fromUtf8("cmdSave"));
        cmdSave->setGeometry(QRect(590, 371, 75, 24));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdSave->setIcon(icon2);
        cmdSave->setIconSize(QSize(30, 16));
        cmdSave->setFlat(true);

        retranslateUi(VpnLog);

        QMetaObject::connectSlotsByName(VpnLog);
    } // setupUi

    void retranslateUi(QDialog *VpnLog)
    {
        VpnLog->setWindowTitle(QApplication::translate("VpnLog", "Securepoint VPN Log", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("VpnLog", "VPN Log:", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QString());
        cmdSave->setText(QString());
        Q_UNUSED(VpnLog);
    } // retranslateUi

};

namespace Ui {
    class VpnLog: public Ui_VpnLog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VPNLOG_H
