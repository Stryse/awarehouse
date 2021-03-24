#include "OutlinerList.h"

ActorOutlinerList::ActorOutlinerList(QList<const Actors*>* actorList, QObject* parent)
    : QObject(parent), mActors(actorList)
{
}

QList<const Actors*> ActorOutlinerList::actors() const
{
    return *mActors;
}

bool ActorOutlinerList::setActorAt(int index, const Actors &actor)
{
    if (index < 0 || index >= mActors->size())
        return false;

    (*mActors)[index] = &actor;
    return true;
}

void ActorOutlinerList::removeActor(int index)
{
    if(index < 0 || index >= mActors->size())
        return;

    emit preItemRemoved(index);
    mActors->remove(index);
    emit postItemRemoved();
}

void ActorOutlinerList::clear()
{
    emit preItemRemoved(0);
    mActors->clear();
    emit postItemRemoved();
}

void ActorOutlinerList::appendActor(const Actors &actor)
{
    emit preItemAppended();
    mActors->append(&actor);
    emit postItemAppended();
}
