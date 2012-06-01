/********************************************************************************
** Form generated from reading UI file 'preferences.ui'
**
** Created: Tue 17. Jan 15:49:13 2012
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFERENCES_H
#define UI_PREFERENCES_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Preferences
{
public:
    QLineEdit *txtPathFromConfig;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *cmdToggleExtensions;
    QPushButton *cmdClose;
    QWidget *extensionWidget;
    QGroupBox *groupBox;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox_2;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QLabel *label_12;
    QLabel *lblLanguage;
    QLabel *label_5;
    QLabel *lblSearchDir;
    QLabel *label_13;
    QLabel *lblConnectVia;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_11;
    QLabel *lblUseInteract;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *cmdToogleUseInteract;
    QLabel *label_6;
    QLabel *lblStartupWindows;
    QPushButton *cmdToogleStartup;
    QHBoxLayout *horizontalLayout_2;
    QLabel *lblDelay;
    QComboBox *cmbDelay;
    QWidget *tab_2;
    QGroupBox *groupBox_3;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QLabel *lblShowBallon;
    QLabel *label_7;
    QLabel *lblNoPopUp;
    QLabel *label_4;
    QLabel *lblReconnect;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_8;
    QPushButton *cmdToogleBallon;
    QPushButton *cmdTooglePopup;
    QPushButton *cmdToogleReconnect;
    QLabel *label_3;
    QLabel *lblShowSplash;
    QPushButton *cmdToogleSpalshScreen;
    QSpacerItem *horizontalSpacer_7;
    QWidget *tab_3;
    QGroupBox *groupBox_4;
    QPushButton *cmdShowCredentials;
    QLabel *label_14;
    QGroupBox *groupBox_7;
    QPushButton *cmdRemoveCredentials;
    QLabel *label_15;
    QWidget *tab_4;
    QGroupBox *groupBox_5;
    QPushButton *cmdRemoveAllTap;
    QLabel *label;
    QGroupBox *groupBox_6;
    QPushButton *cmdAddNewTap;
    QLabel *label_10;
    QPushButton *cmdOpenInfo;
    QTreeWidget *trvConnections;
    QFrame *frame;
    QPushButton *cmdNewConfig;
    QPushButton *cmdRefreshData;
    QPushButton *cmdImportConfig;

    void setupUi(QDialog *Preferences)
    {
        if (Preferences->objectName().isEmpty())
            Preferences->setObjectName(QString::fromUtf8("Preferences"));
        Preferences->resize(441, 605);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Preferences->sizePolicy().hasHeightForWidth());
        Preferences->setSizePolicy(sizePolicy);
        Preferences->setMinimumSize(QSize(0, 0));
        Preferences->setMaximumSize(QSize(1111111, 111111));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/logo.png"), QSize(), QIcon::Normal, QIcon::Off);
        Preferences->setWindowIcon(icon);
        Preferences->setStyleSheet(QString::fromUtf8(""));
        Preferences->setModal(true);
        txtPathFromConfig = new QLineEdit(Preferences);
        txtPathFromConfig->setObjectName(QString::fromUtf8("txtPathFromConfig"));
        txtPathFromConfig->setGeometry(QRect(10, 650, 0, 0));
        horizontalLayoutWidget = new QWidget(Preferences);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(280, 333, 151, 41));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        cmdToggleExtensions = new QPushButton(horizontalLayoutWidget);
        cmdToggleExtensions->setObjectName(QString::fromUtf8("cmdToggleExtensions"));
        cmdToggleExtensions->setMinimumSize(QSize(45, 0));
        cmdToggleExtensions->setMaximumSize(QSize(45, 16777215));
        cmdToggleExtensions->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 45px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/tree_expand.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdToggleExtensions->setIcon(icon1);
        cmdToggleExtensions->setIconSize(QSize(22, 22));
        cmdToggleExtensions->setCheckable(true);
        cmdToggleExtensions->setFlat(true);

        horizontalLayout->addWidget(cmdToggleExtensions);

        cmdClose = new QPushButton(horizontalLayoutWidget);
        cmdClose->setObjectName(QString::fromUtf8("cmdClose"));
        cmdClose->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/close_dialog.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdClose->setIcon(icon2);
        cmdClose->setIconSize(QSize(22, 22));
        cmdClose->setDefault(true);
        cmdClose->setFlat(true);

        horizontalLayout->addWidget(cmdClose);

        extensionWidget = new QWidget(Preferences);
        extensionWidget->setObjectName(QString::fromUtf8("extensionWidget"));
        extensionWidget->setGeometry(QRect(0, 380, 441, 221));
        extensionWidget->setMinimumSize(QSize(0, 170));
        extensionWidget->setMaximumSize(QSize(16777215, 240));
        groupBox = new QGroupBox(extensionWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(5, 1, 431, 211));
        tabWidget = new QTabWidget(groupBox);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 20, 411, 181));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(7, 1, 391, 151));
        gridLayoutWidget_3 = new QWidget(groupBox_2);
        gridLayoutWidget_3->setObjectName(QString::fromUtf8("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(11, 17, 371, 128));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_12 = new QLabel(gridLayoutWidget_3);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_3->addWidget(label_12, 0, 0, 1, 1);

        lblLanguage = new QLabel(gridLayoutWidget_3);
        lblLanguage->setObjectName(QString::fromUtf8("lblLanguage"));

        gridLayout_3->addWidget(lblLanguage, 0, 1, 1, 1);

        label_5 = new QLabel(gridLayoutWidget_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 1, 0, 1, 1);

        lblSearchDir = new QLabel(gridLayoutWidget_3);
        lblSearchDir->setObjectName(QString::fromUtf8("lblSearchDir"));

        gridLayout_3->addWidget(lblSearchDir, 1, 1, 1, 1);

        label_13 = new QLabel(gridLayoutWidget_3);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout_3->addWidget(label_13, 2, 0, 1, 1);

        lblConnectVia = new QLabel(gridLayoutWidget_3);
        lblConnectVia->setObjectName(QString::fromUtf8("lblConnectVia"));

        gridLayout_3->addWidget(lblConnectVia, 2, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 3, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 1, 3, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 2, 3, 1, 1);

        label_11 = new QLabel(gridLayoutWidget_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_3->addWidget(label_11, 3, 0, 1, 1);

        lblUseInteract = new QLabel(gridLayoutWidget_3);
        lblUseInteract->setObjectName(QString::fromUtf8("lblUseInteract"));

        gridLayout_3->addWidget(lblUseInteract, 3, 1, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_10, 3, 3, 1, 1);

        cmdToogleUseInteract = new QPushButton(gridLayoutWidget_3);
        cmdToogleUseInteract->setObjectName(QString::fromUtf8("cmdToogleUseInteract"));
        cmdToogleUseInteract->setMaximumSize(QSize(16777215, 18));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/view-refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdToogleUseInteract->setIcon(icon3);
        cmdToogleUseInteract->setIconSize(QSize(12, 12));
        cmdToogleUseInteract->setFlat(true);

        gridLayout_3->addWidget(cmdToogleUseInteract, 3, 2, 1, 1);

        label_6 = new QLabel(gridLayoutWidget_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 4, 0, 1, 1);

        lblStartupWindows = new QLabel(gridLayoutWidget_3);
        lblStartupWindows->setObjectName(QString::fromUtf8("lblStartupWindows"));

        gridLayout_3->addWidget(lblStartupWindows, 4, 1, 1, 1);

        cmdToogleStartup = new QPushButton(gridLayoutWidget_3);
        cmdToogleStartup->setObjectName(QString::fromUtf8("cmdToogleStartup"));
        cmdToogleStartup->setMaximumSize(QSize(16777215, 18));
        cmdToogleStartup->setIcon(icon3);
        cmdToogleStartup->setIconSize(QSize(12, 12));
        cmdToogleStartup->setFlat(true);

        gridLayout_3->addWidget(cmdToogleStartup, 4, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        lblDelay = new QLabel(gridLayoutWidget_3);
        lblDelay->setObjectName(QString::fromUtf8("lblDelay"));

        horizontalLayout_2->addWidget(lblDelay);

        cmbDelay = new QComboBox(gridLayoutWidget_3);
        cmbDelay->setObjectName(QString::fromUtf8("cmbDelay"));
        cmbDelay->setMaximumSize(QSize(80, 16777215));

        horizontalLayout_2->addWidget(cmbDelay);


        gridLayout_3->addLayout(horizontalLayout_2, 4, 3, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        groupBox_3 = new QGroupBox(tab_2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(7, 1, 391, 151));
        gridLayoutWidget_2 = new QWidget(groupBox_3);
        gridLayoutWidget_2->setObjectName(QString::fromUtf8("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(11, 18, 371, 101));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(gridLayoutWidget_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        lblShowBallon = new QLabel(gridLayoutWidget_2);
        lblShowBallon->setObjectName(QString::fromUtf8("lblShowBallon"));

        gridLayout_2->addWidget(lblShowBallon, 0, 1, 1, 1);

        label_7 = new QLabel(gridLayoutWidget_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 1, 0, 1, 1);

        lblNoPopUp = new QLabel(gridLayoutWidget_2);
        lblNoPopUp->setObjectName(QString::fromUtf8("lblNoPopUp"));

        gridLayout_2->addWidget(lblNoPopUp, 1, 1, 1, 1);

        label_4 = new QLabel(gridLayoutWidget_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        lblReconnect = new QLabel(gridLayoutWidget_2);
        lblReconnect->setObjectName(QString::fromUtf8("lblReconnect"));

        gridLayout_2->addWidget(lblReconnect, 2, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_5, 0, 3, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_6, 1, 3, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_8, 2, 3, 1, 1);

        cmdToogleBallon = new QPushButton(gridLayoutWidget_2);
        cmdToogleBallon->setObjectName(QString::fromUtf8("cmdToogleBallon"));
        cmdToogleBallon->setMaximumSize(QSize(16777215, 18));
        cmdToogleBallon->setIcon(icon3);
        cmdToogleBallon->setIconSize(QSize(12, 12));
        cmdToogleBallon->setFlat(true);

        gridLayout_2->addWidget(cmdToogleBallon, 0, 2, 1, 1);

        cmdTooglePopup = new QPushButton(gridLayoutWidget_2);
        cmdTooglePopup->setObjectName(QString::fromUtf8("cmdTooglePopup"));
        cmdTooglePopup->setMaximumSize(QSize(16777215, 18));
        cmdTooglePopup->setIcon(icon3);
        cmdTooglePopup->setIconSize(QSize(12, 12));
        cmdTooglePopup->setFlat(true);

        gridLayout_2->addWidget(cmdTooglePopup, 1, 2, 1, 1);

        cmdToogleReconnect = new QPushButton(gridLayoutWidget_2);
        cmdToogleReconnect->setObjectName(QString::fromUtf8("cmdToogleReconnect"));
        cmdToogleReconnect->setMaximumSize(QSize(16777215, 18));
        cmdToogleReconnect->setIcon(icon3);
        cmdToogleReconnect->setIconSize(QSize(12, 12));
        cmdToogleReconnect->setFlat(true);

        gridLayout_2->addWidget(cmdToogleReconnect, 2, 2, 1, 1);

        label_3 = new QLabel(gridLayoutWidget_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 3, 0, 1, 1);

        lblShowSplash = new QLabel(gridLayoutWidget_2);
        lblShowSplash->setObjectName(QString::fromUtf8("lblShowSplash"));

        gridLayout_2->addWidget(lblShowSplash, 3, 1, 1, 1);

        cmdToogleSpalshScreen = new QPushButton(gridLayoutWidget_2);
        cmdToogleSpalshScreen->setObjectName(QString::fromUtf8("cmdToogleSpalshScreen"));
        cmdToogleSpalshScreen->setMaximumSize(QSize(16777215, 18));
        cmdToogleSpalshScreen->setIcon(icon3);
        cmdToogleSpalshScreen->setIconSize(QSize(12, 12));
        cmdToogleSpalshScreen->setFlat(true);

        gridLayout_2->addWidget(cmdToogleSpalshScreen, 3, 2, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_7, 3, 3, 1, 1);

        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        groupBox_4 = new QGroupBox(tab_3);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(6, 1, 391, 71));
        cmdShowCredentials = new QPushButton(groupBox_4);
        cmdShowCredentials->setObjectName(QString::fromUtf8("cmdShowCredentials"));
        cmdShowCredentials->setGeometry(QRect(20, 24, 100, 33));
        cmdShowCredentials->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/crypted.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdShowCredentials->setIcon(icon4);
        label_14 = new QLabel(groupBox_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(133, 31, 221, 20));
        groupBox_7 = new QGroupBox(tab_3);
        groupBox_7->setObjectName(QString::fromUtf8("groupBox_7"));
        groupBox_7->setGeometry(QRect(7, 75, 391, 71));
        cmdRemoveCredentials = new QPushButton(groupBox_7);
        cmdRemoveCredentials->setObjectName(QString::fromUtf8("cmdRemoveCredentials"));
        cmdRemoveCredentials->setGeometry(QRect(19, 24, 100, 33));
        cmdRemoveCredentials->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/list-remove.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdRemoveCredentials->setIcon(icon5);
        label_15 = new QLabel(groupBox_7);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(135, 32, 211, 20));
        tabWidget->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        groupBox_5 = new QGroupBox(tab_4);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(6, 1, 391, 71));
        cmdRemoveAllTap = new QPushButton(groupBox_5);
        cmdRemoveAllTap->setObjectName(QString::fromUtf8("cmdRemoveAllTap"));
        cmdRemoveAllTap->setGeometry(QRect(20, 24, 100, 33));
        cmdRemoveAllTap->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/delete.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdRemoveAllTap->setIcon(icon6);
        cmdRemoveAllTap->setIconSize(QSize(22, 22));
        label = new QLabel(groupBox_5);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(130, 24, 251, 31));
        groupBox_6 = new QGroupBox(tab_4);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(7, 75, 391, 71));
        cmdAddNewTap = new QPushButton(groupBox_6);
        cmdAddNewTap->setObjectName(QString::fromUtf8("cmdAddNewTap"));
        cmdAddNewTap->setGeometry(QRect(19, 24, 101, 33));
        cmdAddNewTap->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/add.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdAddNewTap->setIcon(icon7);
        cmdAddNewTap->setIconSize(QSize(22, 22));
        label_10 = new QLabel(groupBox_6);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(130, 30, 251, 20));
        tabWidget->addTab(tab_4, QString());
        cmdOpenInfo = new QPushButton(Preferences);
        cmdOpenInfo->setObjectName(QString::fromUtf8("cmdOpenInfo"));
        cmdOpenInfo->setGeometry(QRect(-10, 0, 461, 63));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(cmdOpenInfo->sizePolicy().hasHeightForWidth());
        cmdOpenInfo->setSizePolicy(sizePolicy1);
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/banner_neu_sml.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdOpenInfo->setIcon(icon8);
        cmdOpenInfo->setIconSize(QSize(448, 63));
        cmdOpenInfo->setFlat(true);
        trvConnections = new QTreeWidget(Preferences);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        trvConnections->setHeaderItem(__qtreewidgetitem);
        trvConnections->setObjectName(QString::fromUtf8("trvConnections"));
        trvConnections->setGeometry(QRect(8, 77, 425, 250));
        sizePolicy1.setHeightForWidth(trvConnections->sizePolicy().hasHeightForWidth());
        trvConnections->setSizePolicy(sizePolicy1);
        trvConnections->setMinimumSize(QSize(425, 250));
        trvConnections->setMaximumSize(QSize(425, 250));
        trvConnections->setContextMenuPolicy(Qt::CustomContextMenu);
        trvConnections->setStyleSheet(QString::fromUtf8("QTreeView::item {\n"
"	margin-top: 7px;\n"
"	margin-bottom: 5px;\n"
"	min-height: 30px;\n"
"}\n"
"\n"
"background-color: rgb(255, 255, 255);\n"
"\n"
"\n"
""));
        trvConnections->setFrameShape(QFrame::Box);
        trvConnections->setFrameShadow(QFrame::Plain);
        trvConnections->setAlternatingRowColors(false);
        trvConnections->setSelectionMode(QAbstractItemView::NoSelection);
        frame = new QFrame(Preferences);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(0, 60, 451, 8));
        frame->setMinimumSize(QSize(0, 8));
        frame->setMaximumSize(QSize(16777215, 5));
        frame->setStyleSheet(QString::fromUtf8("background-image: url(:/images/bg_toolbar.png);"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Plain);
        cmdNewConfig = new QPushButton(Preferences);
        cmdNewConfig->setObjectName(QString::fromUtf8("cmdNewConfig"));
        cmdNewConfig->setGeometry(QRect(10, 337, 100, 33));
        cmdNewConfig->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/start-here.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdNewConfig->setIcon(icon9);
        cmdNewConfig->setIconSize(QSize(22, 22));
        cmdNewConfig->setFlat(true);
        cmdRefreshData = new QPushButton(Preferences);
        cmdRefreshData->setObjectName(QString::fromUtf8("cmdRefreshData"));
        cmdRefreshData->setGeometry(QRect(230, 337, 41, 33));
        cmdRefreshData->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 30px; width:30px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 0px;\n"
"	padding-bottom: 0px;}"));
        cmdRefreshData->setIcon(icon3);
        cmdRefreshData->setIconSize(QSize(22, 22));
        cmdRefreshData->setFlat(true);
        cmdImportConfig = new QPushButton(Preferences);
        cmdImportConfig->setObjectName(QString::fromUtf8("cmdImportConfig"));
        cmdImportConfig->setGeometry(QRect(121, 337, 100, 33));
        cmdImportConfig->setStyleSheet(QString::fromUtf8(":enabled {border: 1px solid black}\n"
":disabled {border: 1px solid black}\n"
":hover {background-color: rgb(195, 195, 195);}\n"
"QPushButton {height: 25px; width: 50px;text-align:left;\n"
"	padding-left: 10px;\n"
"	padding-top: 3px;\n"
"	padding-bottom: 3px;}"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/import.png"), QSize(), QIcon::Normal, QIcon::Off);
        cmdImportConfig->setIcon(icon10);
        cmdImportConfig->setIconSize(QSize(22, 22));

        retranslateUi(Preferences);

        tabWidget->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(Preferences);
    } // setupUi

    void retranslateUi(QDialog *Preferences)
    {
        Preferences->setWindowTitle(QApplication::translate("Preferences", "Securepoint SSL VPN ", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cmdToggleExtensions->setToolTip(QApplication::translate("Preferences", "Show advanced settings", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        cmdToggleExtensions->setText(QString());
        cmdClose->setText(QApplication::translate("Preferences", "     &Close", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("Preferences", "Advanced:", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("Preferences", "General:", 0, QApplication::UnicodeUTF8));
        label_12->setText(QApplication::translate("Preferences", "Language:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblLanguage->setToolTip(QApplication::translate("Preferences", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Description:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">If this option is set, the client will translate Outputs to english. Default language is german.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; text-decoratio"
                        "n: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Parameter:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-useEnglish</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Example:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-useEn"
                        "glish</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Please note:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-style:italic;\">This setting will take no effect to messages from the service.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblLanguage->setText(QApplication::translate("Preferences", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("Preferences", "Search Directory:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblSearchDir->setToolTip(QApplication::translate("Preferences", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Description:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">If this option is set, the SSL VPN Client will search this directory include subdirectories for ovpn files. Default settings is to search only into %user%appdata%/Securepoint SSL VPN/</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0"
                        "px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Parameter:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-searchDir &lt;path&gt;</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Example:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0"
                        "px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-searchDir &quot;c:\\example\\example2&quot;</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblSearchDir->setText(QApplication::translate("Preferences", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_13->setText(QApplication::translate("Preferences", "Mode:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblConnectVia->setToolTip(QApplication::translate("Preferences", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Description:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">This option specify what mode is used to connect to the SSL VPN. There are two possibilities:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">1: Service</span></p>\n"
"<p style=\" ma"
                        "rgin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">2: Local OpenVPN application</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">It is recommended to use the service if the user didn't have administrator rights for the maschine. Please notice that to install the service the administrator permission is needed. After the installation it is possible to use the client as a normal user.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0"
                        "px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">The service option is set to default. To use the local OpenVpn application please set the option </span><span style=\" font-size:8pt; font-weight:600;\">-noService</span><span style=\" font-size:8pt;\">.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Parameter:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-noService</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;"
                        " font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Example:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-noService</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblConnectVia->setText(QApplication::translate("Preferences", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("Preferences", "No Interact:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblUseInteract->setToolTip(QApplication::translate("Preferences", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Description:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">If this option is yes, the client will be automatically do a reconnect, if the client exit with an error.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>\n"
"<p style=\""
                        " margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Parameter:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-noInteract</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Example:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-noInteract</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblUseInteract->setText(QApplication::translate("Preferences", "TextLabel", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        cmdToogleUseInteract->setToolTip(QApplication::translate("Preferences", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        cmdToogleUseInteract->setText(QString());
        label_6->setText(QApplication::translate("Preferences", "Startup on windows:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblStartupWindows->setToolTip(QApplication::translate("Preferences", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Description:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">If this option is yes, the client will be automatically startup when windows is started.</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblStartupWindows->setText(QApplication::translate("Preferences", "TextLabel", 0, QApplication::UnicodeUTF8));
        cmdToogleStartup->setText(QString());
        lblDelay->setText(QApplication::translate("Preferences", "Delay start config:", 0, QApplication::UnicodeUTF8));
        cmbDelay->clear();
        cmbDelay->insertItems(0, QStringList()
         << QApplication::translate("Preferences", "No delay", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "30 sec.", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "60 sec.", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "90 sec.", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Preferences", "120 sec.", 0, QApplication::UnicodeUTF8)
        );
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Preferences", "General", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("Preferences", "View:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Preferences", "Show Ballon:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblShowBallon->setToolTip(QApplication::translate("Preferences", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Description:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">If this option is set, the show of the Systray Ballon Messages will be inhibited. Without this setting the SBM will only displayed when the status dialog is hidden.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-inden"
                        "t:0; text-indent:0px; font-size:8pt; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Parameter:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-noBallon</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Example:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-i"
                        "ndent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-noBallon</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblShowBallon->setText(QApplication::translate("Preferences", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("Preferences", "PopUp with start config:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblNoPopUp->setToolTip(QApplication::translate("Preferences", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">Description:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">If this option is yes, the Status dialog will be not appear automatically if you set a start configuration with &lt;-start&gt;.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" m"
                        "argin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">Parameter:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">-noPopUp</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">Example:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- noPopup</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\""
                        "></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">- start &quot;c:\\example.ovpn&quot; -noPopUp</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblNoPopUp->setText(QApplication::translate("Preferences", "TextLabel", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("Preferences", "Auto reconnect:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblReconnect->setToolTip(QApplication::translate("Preferences", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">Description:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">If this option is set, the SSL VPN Client will be automatically reconnect to the SSL VPN server, if the connection is lost. This can take a while to determine that a connection is lost.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
""
                        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">If no user and password settings make, the client will be ask the userdata again.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">Parameter:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">-reconnect</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; m"
                        "argin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-weight:600; text-decoration: underline;\">Example:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">-reconnect</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblReconnect->setText(QApplication::translate("Preferences", "TextLabel", 0, QApplication::UnicodeUTF8));
        cmdToogleBallon->setText(QString());
        cmdTooglePopup->setText(QString());
        cmdToogleReconnect->setText(QString());
        label_3->setText(QApplication::translate("Preferences", "Show Splashscreen:", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        lblShowSplash->setToolTip(QApplication::translate("Preferences", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Description:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">If this option is yes, the splashscreen will be not appear automatically on startup.</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; text-decoration: underline"
                        ";\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Parameter:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">-noSplash</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt; font-weight:600; text-decoration: underline;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600; text-decoration: underline;\">Example:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">- noSplash</span></p"
                        ">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">- start &quot;c:\\example.ovpn&quot; -noSplash</span></p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        lblShowSplash->setText(QApplication::translate("Preferences", "TextLabel", 0, QApplication::UnicodeUTF8));
        cmdToogleSpalshScreen->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Preferences", "View", 0, QApplication::UnicodeUTF8));
        groupBox_4->setTitle(QApplication::translate("Preferences", "Saved credentials:", 0, QApplication::UnicodeUTF8));
        cmdShowCredentials->setText(QApplication::translate("Preferences", "Show", 0, QApplication::UnicodeUTF8));
        label_14->setText(QApplication::translate("Preferences", "Show a dialog with the saved data", 0, QApplication::UnicodeUTF8));
        groupBox_7->setTitle(QApplication::translate("Preferences", "Remove all data:", 0, QApplication::UnicodeUTF8));
        cmdRemoveCredentials->setText(QApplication::translate("Preferences", "Remove", 0, QApplication::UnicodeUTF8));
        label_15->setText(QApplication::translate("Preferences", "Remove all saved data", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("Preferences", "Saved Data", 0, QApplication::UnicodeUTF8));
        groupBox_5->setTitle(QApplication::translate("Preferences", "Remove all tap devices:", 0, QApplication::UnicodeUTF8));
        cmdRemoveAllTap->setText(QApplication::translate("Preferences", "Remove", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("Preferences", "This removes all tap devices on the system.", 0, QApplication::UnicodeUTF8));
        groupBox_6->setTitle(QApplication::translate("Preferences", "Add a new tap device:", 0, QApplication::UnicodeUTF8));
        cmdAddNewTap->setText(QApplication::translate("Preferences", "Add", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("Preferences", "This add a new tap device on the system.", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QApplication::translate("Preferences", "Tap", 0, QApplication::UnicodeUTF8));
        cmdOpenInfo->setText(QString());
        cmdNewConfig->setText(QApplication::translate("Preferences", "     New", 0, QApplication::UnicodeUTF8));
        cmdRefreshData->setText(QString());
        cmdImportConfig->setText(QApplication::translate("Preferences", "     Import", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Preferences: public Ui_Preferences {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFERENCES_H
