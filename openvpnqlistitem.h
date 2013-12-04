#ifndef OPENVPNQLISTITEM_H
#define OPENVPNQLISTITEM_H
#include <QListWidgetItem>
#include "openvpn.h"

class OpenVpnQListItem : public QListWidgetItem
{
public:
    OpenVpnQListItem(OpenVpn *e, QListWidgetItem *parentItem = 0);
    OpenVpn *itemObject;
};

#endif // OPENVPNQLISTITEM_H
