#include "PodDockList.h"

PodDockList::PodDockList(QObject* parent)
    : QObject(parent)
{}

QList<PodDockPresenter*>* PodDockList::podDocks() { return &m_podDocks; }

bool PodDockList::setPodDockAt(int index, PodDockPresenter& podDock)
{
    if (index < 0 ||
        index >= m_podDocks.size())
        return false;

    const PodDockPresenter* oldPodDock = m_podDocks.at(index);
    if (podDock == *oldPodDock)
        return false;

    m_podDocks[index] = &podDock;
    return true;
}

void PodDockList::appendPodDock(PodDockPresenter& podDock)
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

void PodDockList::removePodDock(int row, int column)
{
    if (row < 0 || column < 0)
        return;

    int index = -1;
    for (int i = 0; i < m_podDocks.size(); ++i)
        if (m_podDocks[i]->row()    == row &&
            m_podDocks[i]->column() == column)
        {
            index = i;
            break;
        }

    if (index == -1)
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
