#include "ActorList.h"

ActorList::ActorList(QObject* parent)
    : QObject(parent)
{}

QList<ActorPresenter*>* ActorList::actors() { return &m_actors; }

bool ActorList::setActorAt(int index, ActorPresenter& actor)
{
    if (index < 0 ||
        index >= m_actors.size())
        return false;

    const ActorPresenter* oldActor = m_actors.at(index);
    if (actor == *oldActor)
        return false;

    m_actors[index] = &actor;
    return true;
}

void ActorList::appendActor(ActorPresenter& actor)
{
    emit preItemAppended();
    m_actors.append(&actor);
    emit postItemAppended();
}

void ActorList::removeActor(int index)
{
    if (index < 0 ||
        index >= m_actors.size())
        return;

    emit preItemRemoved(index);
    m_actors.removeAt(index);
    emit postItemRemoved();
}

void ActorList::removeActor(int row, int column)
{
    if (row < 0 || column < 0)
        return;

    int index = -1;
    for (int i = 0; i < m_actors.size(); ++i)
        if (m_actors[i]->row()    == row &&
            m_actors[i]->column() == column)
        {
            index = i;
            break;
        }

    if (index == -1)
        return;

    emit preItemRemoved(index);
    m_actors.removeAt(index);
    emit postItemRemoved();
}

void ActorList::clear()
{
    emit preItemRemoved(0);
    m_actors.clear();
    emit postItemRemoved();
}
