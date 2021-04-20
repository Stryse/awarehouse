#include "PodList.h"

PodList::PodList(QObject* parent)
    : QObject(parent)
{}

QList<PodPresenter*>* PodList::pods() { return &m_pods; }

bool PodList::setPodAt(int index, PodPresenter& pod)
{
    if (index < 0 ||
        index >= m_pods.size())
        return false;

    m_pods[index] = &pod;
    return true;
}

void PodList::loadJsonArray(const QJsonArray& podsJson)
{
    clear();

    for (int i = 0; i < podsJson.size(); ++i)
    {
        PodPresenter* pod = PodPresenter::loadJsonObject(podsJson[i].toObject(), this);
        if (pod != nullptr)
            appendPod(*pod);
    }
}

QJsonArray PodList::saveJsonArray() const
{
    QJsonArray podsJsonArray;
    for (const auto& pod : m_pods)
        podsJsonArray.append(pod->saveJsonObject());

    return podsJsonArray;
}

void PodList::appendPod(PodPresenter& pod)
{
    emit preItemAppended();

    int last = m_pods.size();
    m_pods.append(&pod);
    connect(&pod, &PodPresenter::mapItemChanged, this, [=]()
    {
        emit dataChanged(last);
    });

    emit postItemAppended();
}

void PodList::removePod(int index)
{
    if (index < 0 ||
        index >= m_pods.size())
        return;

    emit preItemRemoved(index);

    m_pods[index]->disconnect(this);
    m_pods.removeAt(index);
    for (int i = index; i < m_pods.size(); ++i)
    {
        m_pods[i]->disconnect(this);
        connect(m_pods[i], &MapItemPresenter::mapItemChanged, this, [=]()
        {
            emit dataChanged(i);
        });
    }

    emit postItemRemoved();
}

void PodList::removePod(int row, int column)
{
    if (row < 0 || column < 0)
        return;

    for (int i = 0; i < m_pods.size(); ++i)
        if (m_pods[i]->row()    == row &&
            m_pods[i]->column() == column)
        {
            removePod(i);
            return;
        }
}

void PodList::clear()
{
    for (int i = m_pods.size() - 1; i >= 0; --i)
        removePod(i);
}
