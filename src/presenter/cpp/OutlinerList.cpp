#include "OutlinerList.h"

OutlinerList::OutlinerList(QObject* parent)
    : QObject(parent)
{
    mActors.append(Actors("József", "Lalalala", 100, Actors::Robot, Actors::Up));
    mActors.append(Actors("Robi", "Szökdécsel", 100, Actors::Robot, Actors::Down));
    mActors.append(Actors("Szia", "Lajos", 69, Actors::Robot, Actors::Down));
    mActors.append(Actors("Sus", "Amongus", 42, Actors::Robot, Actors::Down));
}

QList<Actors> OutlinerList::actors() const
{
    return mActors;
}

bool OutlinerList::setActorAt(int index, const Actors &actor)
{
    if (index < 0 || index >= mActors.size())
        return false;

    mActors[index] = actor;
    return true;
}

void OutlinerList::removeActor(int index)
{
    if(index < 0 || index >= mActors.size())
        return;

    emit preItemRemoved(index);
    mActors.remove(index);
    emit postItemRemoved();
}

void OutlinerList::clear()
{
    emit preItemRemoved(0);
    mActors.clear();
    emit postItemRemoved();
}

void OutlinerList::appendActor(const Actors &actor)
{
    emit preItemAppended();
    mActors.append(actor);
    emit postItemAppended();
}
