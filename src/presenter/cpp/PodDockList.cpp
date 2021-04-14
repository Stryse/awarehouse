#include "PodDockList.h"

PodDockList::PodDockList(QObject* parent)
    : QObject(parent)
{}

QList<const PodDockPresenter*>* PodDockList::podDocks() { return &m_podDocks; }

bool PodDockList::setPodDockAt(int index, const PodDockPresenter& podDock)
{
    if (index < 0 ||
        index >= m_podDocks.size())
        return false;

    m_podDocks[index] = &podDock;
    return true;
}

void PodDockList::appendPodDock(const PodDockPresenter& podDock)
{
    emit preItemAppended();
    m_podDocks.append(&podDock);
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
