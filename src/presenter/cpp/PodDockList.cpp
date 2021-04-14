#include "PodDockList.h"

PodDockList::PodDockList(QObject* parent)
    : QObject(parent)
{}

QList<const PodDockPresenter*>* PodDockList::podDocks() { return &m_podDocks; }

bool PodDockList::setPodDockAt(int index, const PodDockPresenter &actor)
{
    if (index < 0 ||
        index >= m_podDocks.size())
        return false;

    m_podDocks[index] = &actor;
    return true;
}

void PodDockList::appendPodDock(const PodDockPresenter &actor)
{
    emit preItemAppended();
    m_podDocks.append(&actor);
    emit postItemAppended();
}

void PodDockList::removePodDock(int index)
{
    if (index < 0 ||
        index >= m_podDocks.size())
        return;

    emit preItemRemoved(index);
    m_podDocks.removeAt(index);
    emit postItemRemoved();
}

void PodDockList::clear()
{
    emit preItemRemoved(0);
    m_podDocks.clear();
    emit postItemRemoved();
}
