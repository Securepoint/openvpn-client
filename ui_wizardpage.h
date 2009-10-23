/********************************************************************************
** Form generated from reading ui file 'wizardpage.ui'
**
** Created: Thu 22. Oct 14:04:42 2009
**      by: Qt User Interface Compiler version 4.5.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WIZARDPAGE_H
#define UI_WIZARDPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_WizardPage
{
public:

    void setupUi(QWizardPage *WizardPage)
    {
        if (WizardPage->objectName().isEmpty())
            WizardPage->setObjectName(QString::fromUtf8("WizardPage"));
        WizardPage->resize(400, 300);

        retranslateUi(WizardPage);

        QMetaObject::connectSlotsByName(WizardPage);
    } // setupUi

    void retranslateUi(QWizardPage *WizardPage)
    {
        WizardPage->setWindowTitle(QApplication::translate("WizardPage", "WizardPage", 0, QApplication::UnicodeUTF8));
        WizardPage->setTitle(QApplication::translate("WizardPage", "asd", 0, QApplication::UnicodeUTF8));
        WizardPage->setSubTitle(QApplication::translate("WizardPage", "ssad", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(WizardPage);
    } // retranslateUi

};

namespace Ui {
    class WizardPage: public Ui_WizardPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZARDPAGE_H
