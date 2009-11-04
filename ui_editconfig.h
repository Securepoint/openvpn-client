/********************************************************************************
** Form generated from reading ui file 'editconfig.ui'
**
** Created: Wed 4. Nov 17:16:48 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EDITCONFIG_H
#define UI_EDITCONFIG_H

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

class Ui_EditConfig
{
public:
    QGroupBox *groupBox;
    QPlainTextEdit *memConfigPath;
    QPlainTextEdit *memConfigContent;
    QPushButton *cmdClose;
    QPushButton *cmdSave;

    void setupUi(QDialog *EditConfig)
    {
        if (EditConfig->objectName().isEmpty())
            EditConfig->setObjectName(QString::fromUtf8("EditConfig"));
        EditConfig->resize(592, 419);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
        EditConfig->setWindowIcon(icon);
        EditConfig->setModal(true);
        groupBox = new QGroupBox(EditConfig);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 571, 361));
        memConfigPath = new QPlainTextEdit(groupBox);
        memConfigPath->setObjectName(QString::fromUtf8("memConfigPath"));
        memConfigPath->setEnabled(false);
        memConfigPath->setGeometry(QRect(10, 20, 551, 41));
        memConfigPath->setFrameShape(QFrame::Box);
        memConfigPath->setFrameShadow(QFrame::Plain);
        memConfigPath->setReadOnly(true);
        memConfigContent = new QPlainTextEdit(groupBox);
        memConfigContent->setObjectName(QString::fromUtf8("memConfigContent"));
        memConfigContent->setGeometry(QRect(10, 70, 551, 281));
        memConfigContent->setFrameShape(QFrame::Box);
        memConfigContent->setFrameShadow(QFrame::Plain);
        cmdClose = new QPushButton(EditConfig);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setGeometry(QRect(500, 384, 75, 24));
        cmdClose->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/close.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon1);
        cmdClose->setIconSize(QSize(16, 16));
        cmdClose->setFlat(true);
        cmdSave = new QPushButton(EditConfig);
        cmdSave->setObjectName(QString::fromUtf8("cmdSave"));
        cmdSave->setGeometry(QRect(390, 384, 75, 24));
        cmdSave->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdSave->setIcon(icon2);
        cmdSave->setIconSize(QSize(30, 16));
        cmdSave->setFlat(true);

        retranslateUi(EditConfig);

        QMetaObject::connectSlotsByName(EditConfig);
    } // setupUi

    void retranslateUi(QDialog *EditConfig)
    {
        EditConfig->setWindowTitle(QApplication::translate("EditConfig", "Edit Config", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("EditConfig", "Edit configuration:", 0, QApplication::UnicodeUTF8));
        cmdClose->setText(QApplication::translate("EditConfig", "&Close", 0, QApplication::UnicodeUTF8));
        cmdSave->setText(QApplication::translate("EditConfig", "&Save", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(EditConfig);
    } // retranslateUi

};

namespace Ui {
    class EditConfig: public Ui_EditConfig {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITCONFIG_H
