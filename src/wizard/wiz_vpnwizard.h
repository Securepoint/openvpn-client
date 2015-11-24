#ifndef WIZ_VPNWIZARD_H
#define WIZ_VPNWIZARD_H

//#include <WinSock2.h>
#include <Windows.h>

#include "framelesswizard.h"
#include <QtGui>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;
QT_END_NAMESPACE



class VpnWizard : public FramelessWizard
{
    Q_OBJECT

public:    
    VpnWizard(QWidget * parent = 0);
    void accept();
};

#endif // WIZ_VPNWIZARD_H
