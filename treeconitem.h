#ifndef TREECONITEM_H
#define TREECONITEM_H

#include <QtCore>
#include <QtGui>

#include "openvpn.h"

class TreeButton;


class TreeConItem : public QTreeWidgetItem
{
public:
    TreeConItem(QTreeWidgetItem *parent=0);
    void setTreeButton (TreeButton *button);
    void setOpenVPN (OpenVpn *vpn);
    OpenVpn *getOpenVPN ();
    TreeButton *getTreeButton ();

private:
    TreeButton *button;
    OpenVpn *vpn;
};

#endif // TREECONITEM_H
