#include "connectionmodel.h"

#include <Config/Configs.h>
#include <service/srvcli.h>

ConnectionModel::ConnectionModel(QObject *parent)
    : QAbstractListModel(parent)
{
    this->LoadConnections();
}

ConnectionModel::~ConnectionModel()
{

}

void ConnectionModel::Reset()
{
    qDeleteAll(this->connections);

    this->connections.clear();
}



void ConnectionModel::LoadConnections()
{
    beginResetModel();

    this->Reset();

    auto configList = Configs::instance()->getList();
    for(auto &config : configList) {
        AddConnection(config.second);
    }

    endResetModel();
}

ConnectionData * ConnectionModel::GetConnection(int index)
{
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

    //
    if(index.row() < this->connections.size()) {
       ListItemData *listItem = this->connections.at(index.row());
       //
       return listItem->pConnection;
    }

    return nullptr;
}

void ConnectionModel::AddConnection(ConnectionData *pConnection)
{
    auto listItem = new ListItemData();
    listItem->pConnection = pConnection;
    //
    this->connections.push_back(listItem);
}

int ConnectionModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    //
    return this->connections.size();
}

 QVariant ConnectionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant ();
    }

    ListItemData *listItem = nullptr;
    //
    if(index.row() < this->connections.size()) {
        listItem = this->connections.at(index.row());
    } else {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        return (listItem->pConnection->GetName());
    } else if (role == Qt::UserRole + 1) {
        return (listItem->pConnection->GetIP());
    } else if (role == Qt::UserRole + 2) {
        return (listItem->pConnection->GetLastUsed());
    } else if (role == Qt::UserRole + 3) {
        return (listItem->pConnection->GetLastConnected());
    } else if (role == Qt::UserRole + 4) {
        return (listItem->pConnection->GetState());
    } else if (role == Qt::UserRole + 5) {
        return (listItem->pConnection->IsAutoStart());
    } else if (role == Qt::UserRole + 6) {
        return (listItem->pConnection->IsUserConfig());
    } else if (role == Qt::UserRole + 7) {
        return (listItem->pConnection->GetConfigPath());
    }  else if (role == Qt::UserRole + 8) {
        return (listItem->pConnection->GetId());
    } else if (role == Qt::UserRole + 100) {
        return (listItem->state);
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
        return true;
    }

    ListItemData *pItem = nullptr;
    // A valid index is necessary
    if(index.row() >= this->connections.size() || !index.isValid()) {
        return true;
    }

    pItem = this->connections.at(index.row());
    //
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

    return true;
}
