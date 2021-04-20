#include "PodDockList.h"

#include <QJsonObject>

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
    emit dataChanged(index);
    return true;
}

//void PodDockList::loadJsonArray(const QJsonArray& podDocksJSon)
//{
//    clear();

//    for (int i = 0; i < podDocksJSon.size(); ++i)
//    {
//        PodDockPresenter* podDock = PodDockPresenter::loadJsonObject(podDocksJSon[i].toObject(), this);
//        if (podDock != nullptr)
//            appendPodDock(*podDock);
//    }
//}

//QJsonArray PodDockList::saveJsonArray() const
//{
//    QJsonArray podDocksJsonArray;
//    for (const auto& podDock : m_podDocks)
//        podDocksJsonArray.append(podDock->saveJsonObject());

//    return podDocksJsonArray;
//}

void PodDockList::appendPodDock(PodDockPresenter& podDock)
{
    emit preItemAppended();

    int last = m_podDocks.size();
    m_podDocks.append(&podDock);
    connect(&podDock, &PodDockPresenter::mapItemChanged, this, [=]()
    {
        emit dataChanged(last);
    });

    emit postItemAppended();
}

void PodDockList::removePodDock(int index)
{
    if (index < 0 ||
        index >= m_podDocks.size())
        return;

    emit preItemRemoved(index);

    m_podDocks[index]->disconnect(this);
    m_podDocks.removeAt(index);
    for (int i = index; i < m_podDocks.size(); ++i)
    {
        m_podDocks[i]->disconnect(this);
        connect(m_podDocks[i], &MapItemPresenter::mapItemChanged, this, [=]()
        {
            emit dataChanged(i);
        });
    }

    emit postItemRemoved();
}

void PodDockList::removePodDock(int row, int column)
{
    if (row < 0 || column < 0)
        return;

    for (int i = 0; i < m_podDocks.size(); ++i)
        if (m_podDocks[i]->row()    == row &&
            m_podDocks[i]->column() == column)
        {
            removePodDock(i);
            return;
        }
}

void PodDockList::clear()
{
    if (m_podDocks.size() == 0)
        return;

    for (int i = m_podDocks.size() - 1; i >= 0; --i)
        removePodDock(i);
}
