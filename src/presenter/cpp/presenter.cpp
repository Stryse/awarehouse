#include "Presenter.h"

Presenter::Presenter(QObject *parent) : QObject(parent)
{

}

QVector<Actors*> Presenter::actorItems() const
{
    return mActor;
}

void Presenter::changeActorItems(const QVector<Actors*>* newActors)
{
    mActor.clear();
    mActor = *newActors;
}

void Presenter::addItemToActor(Actors& newItem)
{
    if(mActor.contains(newItem))
        return;

    mActor.append(&newItem);
    emit itemAppendedInActors();
}

QVector<Order*> Presenter::orderItems() const
{
    return mOrders;
}

void Presenter::changeOrderItems(const QVector<Order*>* newOrders)
{
    mOrders.clear();
    mOrders = *newOrders;
}

void Presenter::addItemToOrder(Order &newItem)
{
    if(mOrders.contains(newItem))
        return;

    mOrders.append(&newItem);
    emit itemAppendedInOrders();
}

void Presenter::deleteItemFromOrder(const Order &item)
{
    if(!mOrders.contains(item))
        return;

    mOrders.removeOne(item);
    emit itemDeletedFromOrders();
}


