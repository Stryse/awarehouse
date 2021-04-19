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
    emit dataChanged(index);
    return true;
}

void ActorList::loadJsonArray(const QJsonArray& actorsJson)
{
    clear();

    for (int i = 0; i < actorsJson.size(); ++i)
    {
        ActorPresenter* actor = ActorPresenter::loadJsonObject(actorsJson[i].toObject(), this);
        if (actor != nullptr)
            appendActor(*actor);
    }
}

QJsonArray ActorList::saveJsonArray() const
{
    QJsonArray actorsJsonArray;
    for (const auto& actor : m_actors)
        actorsJsonArray.append(actor->saveJsonObject());

    return actorsJsonArray;
}

void ActorList::appendActor(ActorPresenter& actor)
{
    emit preItemAppended();

    int last = m_actors.size();
    m_actors.append(&actor);
    connect(&actor, &ActorPresenter::mapItemChanged, this, [=]()
    {
        emit dataChanged(last);
    });

    emit postItemAppended();
}

void ActorList::removeActor(int index)
{
    if (index < 0 ||
        index >= m_actors.size())
        return;

    emit preItemRemoved(index);

    m_actors[index]->disconnect(this);
    m_actors.removeAt(index);
    for (int i = index; i < m_actors.size(); ++i)
    {
        m_actors[i]->disconnect(this);
        connect(m_actors[i], &MapItemPresenter::mapItemChanged, this, [=]()
        {
            emit dataChanged(i);
        });
    }

    emit postItemRemoved();
}

void ActorList::removeActor(int row, int column)
{
    if (row < 0 || column < 0)
        return;

    for (int i = 0; i < m_actors.size(); ++i)
        if (m_actors[i]->row()    == row &&
            m_actors[i]->column() == column)
        {
            removeActor(i);
            return;
        }
}

void ActorList::clear()
{
    for (int i = m_actors.size() - 1; i >= 0; --i)
        removeActor(i);
}
