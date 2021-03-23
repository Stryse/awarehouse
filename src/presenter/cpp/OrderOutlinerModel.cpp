#include "OrderOutlinerModel.h"

OrderOutlinerModel::OrderOutlinerModel(QObject *parent)
    : QAbstractListModel(parent), mOrders(nullptr)
{}

int OrderOutlinerModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid() || !mOrders)
        return 0;

    return mOrders->size();
}

QVariant OrderOutlinerModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !mOrders)
       return QVariant();

    const Order& order = mOrders->at(index.row());
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

QHash<int, QByteArray> OrderOutlinerModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[OrderNameRole] = "ordername";
    names[PositionRole] = "position";
    names[RobotNameRole] = "robotname";
    names[OrderNumberRole] = "ordernumber";
    return names;
}

const QList<Order> *OrderOutlinerModel::orders() const
{
    return mOrders;
}

void OrderOutlinerModel::setOrders(const QList<Order> *orders)
{
    beginResetModel();
    this->mOrders = orders;
    endResetModel();
}
