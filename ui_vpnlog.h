/********************************************************************************
** Form generated from reading UI file 'vpnlog.ui'
**
** Created: Thu 12. Jan 13:39:27 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VPNLOG_H
#define UI_VPNLOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_VpnLog
{
public:
    QPushButton *cmdClose;
    QPushButton *cmdSave;
    QPushButton *cmdStartStopLog;
    QPlainTextEdit *memLog;

    void setupUi(QDialog *VpnLog)
    {
        if (VpnLog->objectName().isEmpty())
            VpnLog->setObjectName(QString::fromUtf8("VpnLog"));
        VpnLog->setWindowModality(Qt::ApplicationModal);
        VpnLog->resize(630, 420);
        VpnLog->setMinimumSize(QSize(0, 400));
        VpnLog->setMaximumSize(QSize(1000, 1000));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        VpnLog->setWindowIcon(icon);
        VpnLog->setModal(true);
        cmdClose = new QPushButton(VpnLog);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(520, 380, 100, 33));
        cmdClose->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/close_dialog.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon1);
        cmdClose->setIconSize(QSize(22, 22));
        cmdClose->setDefault(true);
        cmdClose->setFlat(true);
        cmdSave = new QPushButton(VpnLog);
        cmdSave->setObjectName(QString::fromUtf8("cmdSave"));
        cmdSave->setGeometry(QRect(410, 380, 100, 33));
        cmdSave->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdSave->setIcon(icon2);
        cmdSave->setIconSize(QSize(22, 22));
        cmdSave->setFlat(true);
        cmdStartStopLog = new QPushButton(VpnLog);
        cmdStartStopLog->setObjectName(QString::fromUtf8("cmdStartStopLog"));
        cmdStartStopLog->setGeometry(QRect(10, 380, 100, 33));
        cmdStartStopLog->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/logstartstop.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdStartStopLog->setIcon(icon3);
        cmdStartStopLog->setIconSize(QSize(22, 22));
        memLog = new QPlainTextEdit(VpnLog);
        memLog->setObjectName(QString::fromUtf8("memLog"));
        memLog->setGeometry(QRect(10, 10, 611, 361));
        memLog->setFrameShape(QFrame::Box);
        memLog->setFrameShadow(QFrame::Plain);
        memLog->setUndoRedoEnabled(false);
        memLog->setReadOnly(true);

        retranslateUi(VpnLog);

        QMetaObject::connectSlotsByName(VpnLog);
    } // setupUi

    void retranslateUi(QDialog *VpnLog)
    {
        VpnLog->setWindowTitle(QApplication::translate("VpnLog", "Securepoint SSL VPN Log", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QApplication::translate("VpnLog", "     &Close", 0, QApplication::UnicodeUTF8));
        cmdSave->setText(QApplication::translate("VpnLog", "     &Save", 0, QApplication::UnicodeUTF8));
        cmdStartStopLog->setText(QApplication::translate("VpnLog", "     Stop Log", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class VpnLog: public Ui_VpnLog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VPNLOG_H
