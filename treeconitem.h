#ifndef TREECONITEM_H
#define TREECONITEM_H

#include <QtCore>
#include <QtGui>

class OpenVpn;
class TreeButton;

class TreeConItem : public QTreeWidgetItem
{
public:
    TreeConItem(QTreeWidgetItem *parent=0);
    void setTreeButton (TreeButton *button=0);
    void setOpenVPN (OpenVpn *vpn=0);

    OpenVpn *getOpenVPN () const;
    TreeButton *getTreeButton () const;

private:
    TreeButton *button;
    OpenVpn *vpn;
};

#endif // TREECONITEM_H
