#include "treeconitem.h"
#include "treebutton.h"

#include "openvpn.h"

TreeConItem::TreeConItem(QTreeWidgetItem *parent)
    : QTreeWidgetItem (parent),
      button (NULL),
      vpn (NULL)
{    
}

void TreeConItem::setTreeButton(TreeButton *button)
{
    this->button = button;
}

void TreeConItem::setOpenVPN(OpenVpn *vpn)
{
    this->vpn = vpn;
}

TreeButton *TreeConItem::getTreeButton() const
{
    return this->button;
}

OpenVpn *TreeConItem::getOpenVPN() const
{
    return this->vpn;
}
