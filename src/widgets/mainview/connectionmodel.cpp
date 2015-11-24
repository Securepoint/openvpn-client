#include "connectionmodel.h"

#include <Config/Configs.h>
#include <service/srvcli.h>

ConnectionModel::ConnectionModel(QObject *parent)
    : QAbstractListModel(parent)
{
    dividerItem = nullptr;
    this->LoadConnections();
}

ConnectionModel::~ConnectionModel()
{

}

void ConnectionModel::Reset()
{
    qDeleteAll(this->systemConnections);
    qDeleteAll(this->connections);

    this->systemConnections.clear();
    this->connections.clear();


    if(!dividerItem)
    {
        auto pConnection = new ConnectionData();
        pConnection->SetName("User configurations");
        AddConnection(pConnection, true, 1);
    }

    auto pConnection = new ConnectionData();
    pConnection->SetName("System configuration");
    AddConnection(pConnection, true, 2);
}



void ConnectionModel::LoadConnections()
{
    beginResetModel();

    this->Reset();

    auto configList = Configs::instance()->getList();
    for(auto &config : configList)
    {
        AddConnection(config.second, !config.second->IsUserConfig());
    }

    if(this->connections.size() < 1)
        (this->systemConnections.pop_front());


    endResetModel();
}

ConnectionData * ConnectionModel::GetConnection(int index)
{
    for(auto pConnection : this->systemConnections) {
        if(pConnection->pConnection->GetId() == index)
            return pConnection->pConnection;
    }

    for(auto pConnection : this->connections) {
        if(pConnection->pConnection->GetId() == index)
            return pConnection->pConnection;
    }

    return nullptr;
}

ConnectionData * ConnectionModel::GetConnection(const QModelIndex &index)
{
    if (!index.isValid()) {
        return nullptr;
    }

    ListItemData * pItem = nullptr;
    if(index.row() < this->systemConnections.size() && this->systemConnections.size() > 1) 
    {
        pItem = this->systemConnections.at(index.row());
    } 
    else if(this->connections.size() > 0 
        && ((index.row() == this->systemConnections.size() && this->systemConnections.size() > 1) || (this->systemConnections.size() == 1 && index.row() == this->systemConnections.size()-1))) 
    {
        if(systemConnections.size() <= 1)
        {
            pItem = this->connections.at(0);
        }
        else
        {
            pItem = dividerItem;
        }
    } 
    else if(index.row() < this->systemConnections.size() + this->connections.size() + 1) 
    {
        if(this->systemConnections.size() <= 1) 
        {
            pItem = this->connections.at(index.row());
        } 
        else 
        {
            pItem = this->connections.at(index.row() - (this->systemConnections.size() > 1 ? this->systemConnections.size() : 0) - ((this->systemConnections.size() > 1 || this->connections.size() >= 1) ? 1 : 0));
        }
    } 
    else 
    {
        return nullptr;
    }

    return pItem->pConnection;
}

void ConnectionModel::AddConnection(ConnectionData * pConnection, bool bSystem, int bDivider)
{
    auto lItem = new ListItemData();
    lItem->pConnection = pConnection;
    if(bDivider == 1)
    {
         lItem->bDivider = true;
         dividerItem = lItem;
    }
    else
    {
        if(bDivider == 2)
        {
            lItem->bDivider = true;
        }

        if(bSystem) {
            this->systemConnections.push_back(lItem);
        } else {
            this->connections.push_back(lItem);
        }
    }
}

int ConnectionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    //
    if(this->systemConnections.size() <= 1)
        return this->connections.size();
    else
        return this->connections.size() +  (this->systemConnections.size() > 1 ? this->systemConnections.size() : 0) + 1 - (this->connections.size() == 0 ? 1 : 0);
}

 QVariant ConnectionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant ();
    }

     ListItemData * pItem = nullptr;
    if(index.row() < this->systemConnections.size() && this->systemConnections.size() > 1) {
        pItem = this->systemConnections.at(index.row());
    } else if(this->connections.size() > 0 && ((index.row() == this->systemConnections.size() && this->systemConnections.size() > 1) || (this->systemConnections.size() == 1 && index.row() == this->systemConnections.size()-1))) {
        if(systemConnections.size() <= 1)
        {
            pItem = this->connections.at(0);
        }
        else
            pItem = dividerItem;
    } else if(index.row() < this->systemConnections.size() + this->connections.size() + 1) {
        if(this->systemConnections.size() <= 1)
            pItem = this->connections.at(index.row());
        else
            pItem = this->connections.at(index.row() - (this->systemConnections.size() > 1 ? this->systemConnections.size() : 0) - ((this->systemConnections.size() > 1 || this->connections.size() >= 1) ? 1 : 0));
    } else
        return QVariant();

    if (role == Qt::DisplayRole) {
        return (pItem->pConnection->GetName());
    } else if (role == Qt::UserRole + 1) {
        return (pItem->pConnection->GetIP());
    } else if (role == Qt::UserRole + 2) {
        return (pItem->pConnection->GetLastUsed());
    } else if (role == Qt::UserRole + 3) {
        return (pItem->pConnection->GetLastConnected());
    } else if (role == Qt::UserRole + 4) {
        return (pItem->pConnection->GetState());
    } else if (role == Qt::UserRole + 5) {
        return (pItem->pConnection->IsAutoStart());
    } else if (role == Qt::UserRole + 6) {
        return (pItem->pConnection->IsUserConfig());
    } else if (role == Qt::UserRole + 7) {
        return (pItem->pConnection->GetConfigPath());
    }  else if (role == Qt::UserRole + 8) {
        return (pItem->pConnection->GetId());
    } else if (role == Qt::UserRole + 100) {
        return (pItem->state);
    } else if (role == Qt::UserRole + 101) {
        return (pItem->bDivider ? "__divider__" : "");
    }

    return QVariant ();
}


 bool ConnectionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    //
    // Sets the model data
    //

    if (index.row() < 0) {
        // Someone tries to set data of an inavlid item
        //qWarning() << "Invalid index";
        return true;
    }

    ListItemData * pItem = nullptr;
     if(index.row() < this->systemConnections.size() && this->systemConnections.size() > 1) {
        pItem = this->systemConnections.at(index.row());
    } else if(this->connections.size() > 0 && ((index.row() == this->systemConnections.size() && this->systemConnections.size() > 1) || (this->systemConnections.size() == 1 && index.row() == this->systemConnections.size()-1))) {
        if(systemConnections.size() <= 1)
        {
            pItem = this->connections.at(0);
        }
        else
            pItem = dividerItem;
    } else if(index.row() < this->systemConnections.size() + this->connections.size() + 1) {
        if(this->systemConnections.size() <= 1)
            pItem = this->connections.at(index.row());
        else
            pItem = this->connections.at(index.row() - (this->systemConnections.size() > 1 ? this->systemConnections.size() : 0) - ((this->systemConnections.size() > 1 || this->connections.size() >= 1) ? 1 : 0));
    } else
        return true;

    // A valid index is necessary
    if (index.isValid()) {
        switch (role) {
            case Qt::UserRole + 100: {
                 (pItem)->state = value.toInt();
                break;
            }
            case Qt::UserRole + 4: {
                (pItem)->pConnection->SetState((ConnectionState)value.toInt());
                break;
            }
            default: {
                // Keep gcc quiet
                int dummy;
                Q_UNUSED(dummy)
                break;
            }
        }
    }

    return true;
}
