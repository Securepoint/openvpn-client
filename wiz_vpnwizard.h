#ifndef WIZ_VPNWIZARD_H
#define WIZ_VPNWIZARD_H

#include <QWizard>
#include "wiz_startpage.h"
#include "wiz_generalpage.h"
#include "wiz_remotepage.h"
#include "wiz_endpage.h"
#include "wiz_certpage.h"
#include "wiz_advpage.h"
#include "appfunc.h"
#include "openvpn.h"
#include "Configs.h"


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
    static VpnWizard *getInstance ();
    void accept();

private:
    VpnWizard();
    static VpnWizard *mInst;

};

#endif // WIZ_VPNWIZARD_H
