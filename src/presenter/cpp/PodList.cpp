#include "PodList.h"

PodList::PodList(QObject* parent)
    : QObject(parent)
{}

QList<const PodPresenter*>* PodList::pods() { return &m_pods; }

bool PodList::setPodAt(int index, const PodPresenter& pod)
{
    if (index < 0 ||
        index >= m_pods.size())
        return false;

    m_pods[index] = &pod;
    return true;
}

void PodList::appendPod(const PodPresenter& pod)
{
    emit preItemAppended();
    m_pods.append(&pod);
    emit postItemAppended();
}

void PodList::removePod(int index)
{
    if (index < 0 ||
        index >= m_pods.size())
        return;

    emit preItemRemoved(index);
    m_pods.removeAt(index);
    emit postItemRemoved();
}

void PodList::clear()
{
    emit preItemRemoved(0);
    m_pods.clear();
    emit postItemRemoved();
}
