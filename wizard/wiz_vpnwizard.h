#ifndef WIZ_VPNWIZARD_H
#define WIZ_VPNWIZARD_H

#include <QWizard>
#include <QtGui>

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QRadioButton;
QT_END_NAMESPACE

class VpnWizard : public QWizard
{
    Q_OBJECT

public:    
    VpnWizard();
    void accept();
};

#endif // WIZ_VPNWIZARD_H
