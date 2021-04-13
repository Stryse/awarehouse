#include "ActorList.h"

ActorList::ActorList(QList<const ActorPresenter*>* actors, QObject* parent)
    : QObject(parent)
    , m_actors(actors)
{}

QList<const ActorPresenter*> ActorList::actors() const { return *m_actors; }

bool ActorList::setActorAt(int index, const ActorPresenter &actor)
{
    if (index < 0 ||
        index >= m_actors->size())
        return false;

    (*m_actors)[index] = &actor;
    return true;
}

void ActorList::appendActor(const ActorPresenter &actor)
{
    emit preItemAppended();
    m_actors->append(&actor);
    emit postItemAppended();
}

void ActorList::removeActor(int index)
{
    if (index < 0 ||
        index >= m_actors->size())
        return;

    emit preItemRemoved(index);
    m_actors->removeAt(index);
    emit postItemRemoved();
}

void ActorList::clear()
{
    emit preItemRemoved(0);
    m_actors->clear();
    emit postItemRemoved();
}
