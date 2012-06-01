/********************************************************************************
** Form generated from reading UI file 'servicelog.ui'
**
** Created: Tue 17. Jan 13:18:53 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVICELOG_H
#define UI_SERVICELOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ServiceLog
{
public:
    QPlainTextEdit *memLog;
    QPushButton *cmdClose;
    QPushButton *cmdSave;
    QPushButton *cmdStopLog;

    void setupUi(QDialog *ServiceLog)
    {
        if (ServiceLog->objectName().isEmpty())
            ServiceLog->setObjectName(QString::fromUtf8("ServiceLog"));
        ServiceLog->setWindowModality(Qt::ApplicationModal);
        ServiceLog->resize(600, 430);
        ServiceLog->setMinimumSize(QSize(600, 430));
        ServiceLog->setMaximumSize(QSize(600, 430));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        ServiceLog->setWindowIcon(icon);
        ServiceLog->setModal(true);
        memLog = new QPlainTextEdit(ServiceLog);
        memLog->setObjectName(QString::fromUtf8("memLog"));
        memLog->setGeometry(QRect(10, 10, 581, 371));
        memLog->setFrameShape(QFrame::Box);
        memLog->setFrameShadow(QFrame::Plain);
        memLog->setReadOnly(true);
        cmdClose = new QPushButton(ServiceLog);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(490, 390, 100, 33));
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
        cmdSave = new QPushButton(ServiceLog);
        cmdSave->setObjectName(QString::fromUtf8("cmdSave"));
        cmdSave->setGeometry(QRect(380, 390, 100, 33));
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
        cmdStopLog = new QPushButton(ServiceLog);
        cmdStopLog->setObjectName(QString::fromUtf8("cmdStopLog"));
        cmdStopLog->setGeometry(QRect(10, 390, 100, 33));
        cmdStopLog->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/logstartstop.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdStopLog->setIcon(icon3);
        cmdStopLog->setIconSize(QSize(22, 22));
        cmdStopLog->setFlat(true);

        retranslateUi(ServiceLog);

        QMetaObject::connectSlotsByName(ServiceLog);
    } // setupUi

    void retranslateUi(QDialog *ServiceLog)
    {
        ServiceLog->setWindowTitle(QApplication::translate("ServiceLog", "Securepoint SSL VPN Service Log", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QApplication::translate("ServiceLog", "     &Close", 0, QApplication::UnicodeUTF8));
        cmdSave->setText(QApplication::translate("ServiceLog", "     &Save", 0, QApplication::UnicodeUTF8));
        cmdStopLog->setText(QApplication::translate("ServiceLog", "     Stop &Log", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ServiceLog: public Ui_ServiceLog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICELOG_H
