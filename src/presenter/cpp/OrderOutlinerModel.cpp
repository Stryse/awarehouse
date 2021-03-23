#include "OrderOutlinerModel.h"

OrderOutlinerModel::OrderOutlinerModel(QObject *parent)
    : QAbstractListModel(parent), mOrders(nullptr)
{}

int OrderOutlinerModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid() || !mOrders)
        return 0;

    return mOrders->orders().size();
}

QVariant OrderOutlinerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mOrders)
       return QVariant();

    const Order& order = mOrders->orders().at(index.row());
    switch(role) {
            case OrderNameRole:
                return QVariant(order.orderName());
            case PositionRole:
            {
                QString pos = "X: " + QString::number(order.positionX()) + " Y: " + QString::number(order.positionY());
                return QVariant(pos);
            }
            case RobotNameRole:
                return QVariant(order.assignedRobotName());
            case OrderNumberRole:
                return QVariant(order.orderNumber());
    }
    return QVariant();
}

bool OrderOutlinerModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(data(index,role) != value)
    {
        // TODO IMPLEMENT
        emit dataChanged(index,index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags OrderOutlinerModel::flags(const QModelIndex& index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}


QHash<int, QByteArray> OrderOutlinerModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[OrderNameRole] = "ordername";
    names[PositionRole] = "position";
    names[RobotNameRole] = "robotname";
    names[OrderNumberRole] = "ordernumber";
    return names;
}

OrderOutlinerList *OrderOutlinerModel::orders() const
{
    return mOrders;
}

void OrderOutlinerModel::setOrders(OrderOutlinerList *orders)
{
    beginResetModel();

    if(mOrders)
        mOrders->disconnect(this);

    mOrders = orders;
    if(mOrders)
    {
        connect(mOrders, &OrderOutlinerList::preItemAppended,this,[=](){
            const int index = mOrders->orders().size();
            beginInsertRows(QModelIndex(),index,index);
        });

        connect(mOrders, &OrderOutlinerList::postItemAppended,this,[=](){
            endInsertRows();
        });

        connect(mOrders, &OrderOutlinerList::preItemRemoved, this,[=](int index){
            beginRemoveRows(QModelIndex(),index, index);
        });

        connect(mOrders, &OrderOutlinerList::postItemRemoved, this, [=](){
            endRemoveRows();
        });
    }

    endResetModel();
}
