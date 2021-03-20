#include "OrderListModel.h"
#include "Presenter.h"

OrderListModel::OrderListModel(QObject *parent)
    : QAbstractItemModel(parent)
{
}

int OrderListModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid())
        return 0;

    return mOrderList->orderItems().size();
}

QVariant OrderListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || mOrderList->orderItems().size() == 0)
        return QVariant();

    const Order item = *(mOrderList->orderItems().at(index.row()));
    switch(role){
        case NameRole:
            return QVariant(item.assignedRobotName());
        case TaskRole:
            return QVariant(item.orderNumber());
    }

    return QVariant();
}

QHash<int, QByteArray> OrderListModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[NameRole] = "name";
    names[TaskRole] = "task";
    return names;
}

QVector<Order*> OrderListModel::list() const
{
    return mOrderList->orderItems();
}

void OrderListModel::setOrderList(QVector<Order*> *orders)
{
    beginResetModel();

    mOrderList->changeOrderItems(orders);

    if(mOrderList->actorItems().size() != 0)
    {
        connect(mOrderList, &Presenter::itemAppendedInOrders, this, [=](){endInsertRows();});
        connect(mOrderList, &Presenter::itemDeletedFromOrders, this, [=](){endRemoveRows();});
    }

    endResetModel();
}
