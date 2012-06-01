/********************************************************************************
** Form generated from reading UI file 'wiz_endpage.ui'
**
** Created: Thu 12. Jan 13:39:28 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WIZ_ENDPAGE_H
#define UI_WIZ_ENDPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_EndPage
{
public:
    QGroupBox *gbEndPage;
    QLabel *label;
    QLabel *lblRemoteIP;
    QLabel *label_2;
    QLabel *lblRemotePort;
    QLabel *label_3;
    QLabel *lblRemoteProtocol;
    QLabel *label_4;
    QLineEdit *txtPathToCA;
    QLabel *label_5;
    QLineEdit *txtPathToCert;
    QLabel *label_6;
    QLineEdit *txtPathToKey;
    QLabel *label_7;
    QLabel *lblCerttype;
    QGroupBox *groupBox;
    QLabel *label_8;
    QLabel *lblMssfix;
    QLabel *label_9;
    QLabel *lblRouteMethod;
    QLabel *label_18;
    QLabel *label_17;
    QLabel *label_22;
    QLabel *lblRouteDelay;
    QLabel *lblMute;
    QLabel *lblVerbose;
    QLabel *label_20;
    QLabel *label_16;
    QLabel *label_10;
    QLabel *label_11;
    QLabel *lblUserPass;
    QLabel *lblNobind;
    QLabel *lblFloat;
    QLabel *lblCompLzo;
    QLabel *label_12;
    QLabel *lblWindir;
    QLabel *lblInfo;

    void setupUi(QWizardPage *EndPage)
    {
        if (EndPage->objectName().isEmpty())
            EndPage->setObjectName(QString::fromUtf8("EndPage"));
        EndPage->resize(400, 300);
        gbEndPage = new QGroupBox(EndPage);
        gbEndPage->setObjectName(QString::fromUtf8("gbEndPage"));
        gbEndPage->setGeometry(QRect(10, 7, 381, 291));
        label = new QLabel(gbEndPage);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 61, 16));
        lblRemoteIP = new QLabel(gbEndPage);
        lblRemoteIP->setObjectName(QString::fromUtf8("lblRemoteIP"));
        lblRemoteIP->setGeometry(QRect(91, 20, 151, 16));
        lblRemoteIP->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        label_2 = new QLabel(gbEndPage);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(260, 21, 31, 16));
        lblRemotePort = new QLabel(gbEndPage);
        lblRemotePort->setObjectName(QString::fromUtf8("lblRemotePort"));
        lblRemotePort->setGeometry(QRect(290, 22, 46, 14));
        lblRemotePort->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        label_3 = new QLabel(gbEndPage);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 40, 46, 14));
        lblRemoteProtocol = new QLabel(gbEndPage);
        lblRemoteProtocol->setObjectName(QString::fromUtf8("lblRemoteProtocol"));
        lblRemoteProtocol->setGeometry(QRect(91, 40, 46, 14));
        lblRemoteProtocol->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        label_4 = new QLabel(gbEndPage);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 58, 61, 16));
        txtPathToCA = new QLineEdit(gbEndPage);
        txtPathToCA->setObjectName(QString::fromUtf8("txtPathToCA"));
        txtPathToCA->setEnabled(false);
        txtPathToCA->setGeometry(QRect(70, 56, 301, 20));
        txtPathToCA->setContextMenuPolicy(Qt::NoContextMenu);
        txtPathToCA->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black;background-color: rgb(195, 195, 195);color: rgb(0, 0, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);color: rgb(0, 0, 255);}"));
        txtPathToCA->setReadOnly(true);
        label_5 = new QLabel(gbEndPage);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 80, 61, 16));
        txtPathToCert = new QLineEdit(gbEndPage);
        txtPathToCert->setObjectName(QString::fromUtf8("txtPathToCert"));
        txtPathToCert->setEnabled(false);
        txtPathToCert->setGeometry(QRect(70, 78, 301, 20));
        txtPathToCert->setContextMenuPolicy(Qt::NoContextMenu);
        txtPathToCert->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black;background-color: rgb(195, 195, 195);color: rgb(0, 0, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);color: rgb(0, 0, 255);}"));
        txtPathToCert->setReadOnly(true);
        label_6 = new QLabel(gbEndPage);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(10, 102, 61, 16));
        txtPathToKey = new QLineEdit(gbEndPage);
        txtPathToKey->setObjectName(QString::fromUtf8("txtPathToKey"));
        txtPathToKey->setEnabled(false);
        txtPathToKey->setGeometry(QRect(70, 100, 301, 20));
        txtPathToKey->setContextMenuPolicy(Qt::NoContextMenu);
        txtPathToKey->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black;background-color: rgb(195, 195, 195);color: rgb(0, 0, 255);}\n"
":disabled {border: 1px solid black; background-color: rgb(195, 195, 195);color: rgb(0, 0, 255);}\n"
""));
        txtPathToKey->setReadOnly(true);
        label_7 = new QLabel(gbEndPage);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(10, 122, 81, 16));
        lblCerttype = new QLabel(gbEndPage);
        lblCerttype->setObjectName(QString::fromUtf8("lblCerttype"));
        lblCerttype->setGeometry(QRect(100, 122, 101, 16));
        lblCerttype->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        groupBox = new QGroupBox(gbEndPage);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 138, 361, 121));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(10, 39, 46, 14));
        lblMssfix = new QLabel(groupBox);
        lblMssfix->setObjectName(QString::fromUtf8("lblMssfix"));
        lblMssfix->setGeometry(QRect(80, 40, 31, 16));
        lblMssfix->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(140, 38, 81, 16));
        lblRouteMethod = new QLabel(groupBox);
        lblRouteMethod->setObjectName(QString::fromUtf8("lblRouteMethod"));
        lblRouteMethod->setGeometry(QRect(220, 38, 101, 16));
        lblRouteMethod->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        label_18 = new QLabel(groupBox);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 96, 61, 20));
        label_17 = new QLabel(groupBox);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(10, 60, 61, 16));
        label_22 = new QLabel(groupBox);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setGeometry(QRect(10, 77, 71, 20));
        lblRouteDelay = new QLabel(groupBox);
        lblRouteDelay->setObjectName(QString::fromUtf8("lblRouteDelay"));
        lblRouteDelay->setGeometry(QRect(80, 78, 51, 20));
        lblRouteDelay->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        lblMute = new QLabel(groupBox);
        lblMute->setObjectName(QString::fromUtf8("lblMute"));
        lblMute->setGeometry(QRect(80, 100, 46, 14));
        lblMute->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        lblVerbose = new QLabel(groupBox);
        lblVerbose->setObjectName(QString::fromUtf8("lblVerbose"));
        lblVerbose->setGeometry(QRect(80, 61, 46, 16));
        lblVerbose->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        label_20 = new QLabel(groupBox);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setGeometry(QRect(240, 58, 81, 20));
        label_16 = new QLabel(groupBox);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(240, 80, 61, 16));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(140, 61, 61, 16));
        label_11 = new QLabel(groupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(140, 80, 61, 16));
        lblUserPass = new QLabel(groupBox);
        lblUserPass->setObjectName(QString::fromUtf8("lblUserPass"));
        lblUserPass->setGeometry(QRect(320, 60, 31, 16));
        lblUserPass->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        lblNobind = new QLabel(groupBox);
        lblNobind->setObjectName(QString::fromUtf8("lblNobind"));
        lblNobind->setGeometry(QRect(188, 80, 31, 16));
        lblNobind->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        lblFloat = new QLabel(groupBox);
        lblFloat->setObjectName(QString::fromUtf8("lblFloat"));
        lblFloat->setGeometry(QRect(187, 61, 31, 16));
        lblFloat->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        lblCompLzo = new QLabel(groupBox);
        lblCompLzo->setObjectName(QString::fromUtf8("lblCompLzo"));
        lblCompLzo->setGeometry(QRect(320, 81, 31, 16));
        lblCompLzo->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        label_12 = new QLabel(groupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 20, 61, 16));
        lblWindir = new QLabel(groupBox);
        lblWindir->setObjectName(QString::fromUtf8("lblWindir"));
        lblWindir->setGeometry(QRect(80, 20, 251, 16));
        lblWindir->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 255);"));
        lblInfo = new QLabel(gbEndPage);
        lblInfo->setObjectName(QString::fromUtf8("lblInfo"));
        lblInfo->setGeometry(QRect(9, 265, 370, 20));

        retranslateUi(EndPage);

        QMetaObject::connectSlotsByName(EndPage);
    } // setupUi

    void retranslateUi(QWizardPage *EndPage)
    {
        EndPage->setWindowTitle(QApplication::translate("EndPage", "WizardPage", 0, QApplication::UnicodeUTF8));
        EndPage->setTitle(QApplication::translate("EndPage", "Conclusion", 0, QApplication::UnicodeUTF8));
        EndPage->setSubTitle(QApplication::translate("EndPage", "Please check your defined settings.", 0, QApplication::UnicodeUTF8));
        gbEndPage->setTitle(QApplication::translate("EndPage", "Overview:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("EndPage", "Remote IP:", 0, QApplication::UnicodeUTF8));
        lblRemoteIP->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("EndPage", "Port:", 0, QApplication::UnicodeUTF8));
        lblRemotePort->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("EndPage", "Protocol:", 0, QApplication::UnicodeUTF8));
        lblRemoteProtocol->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("EndPage", "CA:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("EndPage", "Certificate:", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("EndPage", "Key", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("EndPage", "Certificate type:", 0, QApplication::UnicodeUTF8));
        lblCerttype->setText(QApplication::translate("EndPage", "Text", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("EndPage", "Advanced:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("EndPage", "MSSFIX:", 0, QApplication::UnicodeUTF8));
        lblMssfix->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("EndPage", "Route method:", 0, QApplication::UnicodeUTF8));
        lblRouteMethod->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_18->setText(QApplication::translate("EndPage", "Mute:", 0, QApplication::UnicodeUTF8));
        label_17->setText(QApplication::translate("EndPage", "Verbosity:", 0, QApplication::UnicodeUTF8));
        label_22->setText(QApplication::translate("EndPage", "Route delay:", 0, QApplication::UnicodeUTF8));
        lblRouteDelay->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        lblMute->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        lblVerbose->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_20->setText(QApplication::translate("EndPage", "Auth user/pass:", 0, QApplication::UnicodeUTF8));
        label_16->setText(QApplication::translate("EndPage", "Comp-LZO:", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("EndPage", "Float:", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("EndPage", "No bind:", 0, QApplication::UnicodeUTF8));
        lblUserPass->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        lblNobind->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        lblFloat->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        lblCompLzo->setText(QApplication::translate("EndPage", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("EndPage", "Windir:", 0, QApplication::UnicodeUTF8));
        lblWindir->setText(QApplication::translate("EndPage", "default", 0, QApplication::UnicodeUTF8));
        lblInfo->setText(QApplication::translate("EndPage", "MTU [1500], dev tun, persist key and persist tun will be added automatically.", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EndPage: public Ui_EndPage {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WIZ_ENDPAGE_H
