#include "Order.h"

Order::Order(int positionX, int positionY, int orderNumber, QString orderName, QString assignedRobotName)
    : mOrderPositionX(positionX), mOrderPositionY(positionY), mOrderNumber(orderNumber),
      mOrderName(orderName), mAssignedRobotName(assignedRobotName)
{
}

Order::Order(const Order &order)
    : mOrderPositionX(order.mOrderPositionX), mOrderPositionY(order.mOrderPositionY), mOrderNumber(order.mOrderNumber),
      mOrderName(order.mOrderName), mAssignedRobotName(order.mAssignedRobotName)
{
}

int Order::positionX() const
{
    return mOrderPositionX;
}

void Order::setPositionX(int positionX)
{
    if(mOrderPositionX == positionX)
        return;

    mOrderPositionX = positionX;
    emit positionXChanged();
}

int Order::positionY() const
{
    return mOrderPositionY;
}

void Order::setPositionY(int positionY)
{
    if(mOrderPositionY == positionY)
        return;

    mOrderPositionY = positionY;
    emit positionYChanged();
}

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

QString Order::orderName() const
{
    return mOrderName;
}

void Order::setOrderName(const QString &name)
{
    if(mOrderName == name)
        return;

    mOrderName = name;
    emit orderNameChanged();
}

QString Order::assignedRobotName() const
{
    return mAssignedRobotName;
}

void Order::setAssignedRobotName(const QString &name)
{
    if(mAssignedRobotName == name)
        return;

    mAssignedRobotName = name;
    emit robotNameChanged();
}

Order& Order::operator=(const Order &other)
{
    this->mAssignedRobotName = other.mAssignedRobotName;
    this->mOrderName = other.mOrderName;
    this->mOrderNumber = other.mOrderNumber;
    this->mOrderPositionX = other.mOrderPositionX;
    this->mOrderPositionY = other.mOrderPositionY;
    return *this;
}
