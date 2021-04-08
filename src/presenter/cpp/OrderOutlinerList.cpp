#include "OrderOutlinerList.h"

#include "OutlinerList.h"

OrderOutlinerList::OrderOutlinerList(QObject* parent)
    : QObject(parent)
{
    mOrders.append(Order(0, 1, 3, "3_FELADAT01"));
    mOrders.append(Order(1, 2, 1, "1_FELADAT01"));
    mOrders.append(Order(3, 4, 2, "2_FELADAT01"));
}

QList<Order> OrderOutlinerList::orders() const
{
    return mOrders;
}

bool OrderOutlinerList::setOrderAt(int index, const Order &order)
{
    if (index < 0 || index >= mOrders.size())
        return false;

    mOrders[index] = order;
    return true;
}

void OrderOutlinerList::removeOrder(int index)
{
    if(index < 0 || index >= mOrders.size())
        return;

    emit preItemRemoved(index);
    mOrders.removeAt(index);
    emit postItemRemoved();
}

void OrderOutlinerList::clear()
{
    emit preItemRemoved(0);
    mOrders.clear();
    emit postItemRemoved();
}

void OrderOutlinerList::appendOrder(const Order &order)
{
    emit preItemAppended();
    mOrders.append(order);
    emit postItemAppended();
}
