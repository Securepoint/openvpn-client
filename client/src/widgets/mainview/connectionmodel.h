#ifndef CONNECTIONMODEL_H
#define CONNECTIONMODEL_H

#include <QAbstractListModel>
#include "connectiondata.h"

class ConnectionModel : public QAbstractListModel
{
    Q_OBJECT

public:
    ConnectionModel(QObject *parent = 0);
    virtual ~ConnectionModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);

    void AddConnection(ConnectionData * pConnection);
    ConnectionData * GetConnection(int index);
    ConnectionData * GetConnection(const QModelIndex &index);
    void Reset();

    void LoadConnections();
private:
	struct ListItemData
	{
		ConnectionData * pConnection;
		int state;
	};

    QList<ListItemData*> connections;
};

#endif // CONNECTIONMODEL_H
