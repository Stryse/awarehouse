#include "Order.h"

Order::Order(/*Point position,*/ int orderNumber, QString assignedRobotName) : /*mPosition(position),*/ mOrderNumber(orderNumber), mAssignedRobotName(assignedRobotName)
{
}

/*Point Order::position() const
{
    return mPosition;
}

void Order::setPosition(const Point &position)
{
    if(mPosition == position)
        return;

    mPosition = position;
    emit positionChanged();
}*/

int Order::orderNumber() const
{
    return mOrderNumber;
}

void Order::setOrderNumber(int orderNumber)
{
    if(mOrderNumber == orderNumber)
        return;

    mOrderNumber = orderNumber;
    emit orderNumberChanged();
}

QString Order::assignedRobotName()
{
    return mAssignedRobotName;
}

void Order::setAssignedRobotName(const QString &name)
{
    if(mAssignedRobotName == name)
        return;

    mAssignedRobotName = name;
    emit nameChanged();
}
