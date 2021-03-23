#include "OutlinerList.h"

ActorOutlinerList::ActorOutlinerList(QObject* parent)
    : QObject(parent)
{
    mActors.append(Actors("József", "Lalalala", 100, Actors::Robot, Actors::Up));
    mActors.append(Actors("Robi", "Szökdécsel", 100, Actors::Robot, Actors::Down));
    mActors.append(Actors("Szia", "Lajos", 69, Actors::Robot, Actors::Down));
    mActors.append(Actors("Sus", "Amongus", 42, Actors::Robot, Actors::Down));
}

QList<Actors> ActorOutlinerList::actors() const
{
    return mActors;
}

bool ActorOutlinerList::setActorAt(int index, const Actors &actor)
{
    if (index < 0 || index >= mActors.size())
        return false;

    mActors[index] = actor;
    return true;
}

void ActorOutlinerList::removeActor(int index)
{
    if(index < 0 || index >= mActors.size())
        return;

    emit preItemRemoved(index);
    mActors.remove(index);
    emit postItemRemoved();
}

void ActorOutlinerList::clear()
{
    emit preItemRemoved(0);
    mActors.clear();
    emit postItemRemoved();
}

void ActorOutlinerList::appendActor(const Actors &actor)
{
    emit preItemAppended();
    mActors.append(actor);
    emit postItemAppended();
}
