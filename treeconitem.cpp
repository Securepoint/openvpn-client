#include "treeconitem.h"
#include "treebutton.h"

TreeConItem::TreeConItem(QTreeWidgetItem *parent) : QTreeWidgetItem (parent)
{
    // Pointer initialisieren
    this->vpn = NULL;
    this->button = NULL;
}

void TreeConItem::setTreeButton(TreeButton *button) {
    this->button = button;
}

void TreeConItem::setOpenVPN(OpenVpn *vpn) {
    this->vpn = vpn;
}

TreeButton *TreeConItem::getTreeButton() {
    return this->button;
}

OpenVpn *TreeConItem::getOpenVPN() {
    return this->vpn;
}
