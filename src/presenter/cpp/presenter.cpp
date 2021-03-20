#include "Presenter.h"

Presenter::Presenter(QObject *parent) : QObject(parent)
{

}

QVector<Actors> Presenter::actorItems() const
{
    return mActor;
}

void Presenter::addItemToActor(const Actors& newItem)
{
    if(mActor.contains(newItem))
        return;

    mActor.append(newItem);
    emit itemAppendedInActors();
}


