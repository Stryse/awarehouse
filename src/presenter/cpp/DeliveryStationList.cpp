#include "DeliveryStationList.h"

DeliveryStationList::DeliveryStationList(QObject* parent)
    : QObject(parent)
{}

QList<DeliveryStationPresenter*>* DeliveryStationList::deliveryStations() { return &m_deliveryStations; }

bool DeliveryStationList::setDeliveryStationAt(int index, DeliveryStationPresenter& deliveryStation)
{
    if (index < 0 ||
        index >= m_deliveryStations.size())
        return false;

    const DeliveryStationPresenter* oldDeliveryStation = m_deliveryStations.at(index);
    if (deliveryStation == *oldDeliveryStation)
        return false;

    m_deliveryStations[index] = &deliveryStation;
    emit dataChanged(index);
    return true;
}

void DeliveryStationList::appendDeliveryStation(DeliveryStationPresenter& deliveryStation)
{
    emit preItemAppended();

    int last = m_deliveryStations.size();
    m_deliveryStations.append(&deliveryStation);
    connect(&deliveryStation, &DeliveryStationPresenter::mapItemChanged, this, [=]()
    {
        emit dataChanged(last);
    });

    emit postItemAppended();
}

void DeliveryStationList::removeDeliveryStation(int index)
{
    if (index < 0 ||
        index >= m_deliveryStations.size())
        return;

    emit preItemRemoved(index);

    m_deliveryStations[index]->disconnect(this);
    m_deliveryStations.removeAt(index);
    for (int i = index; i < m_deliveryStations.size(); ++i)
    {
        m_deliveryStations[i]->disconnect(this);
        connect(m_deliveryStations[i], &MapItemPresenter::mapItemChanged, this, [=]()
        {
            emit dataChanged(i);
        });
    }

    emit postItemRemoved();
}

void DeliveryStationList::removeDeliveryStation(int row, int column)
{
    if (row < 0 || column < 0)
        return;

    for (int i = 0; i < m_deliveryStations.size(); ++i)
        if (m_deliveryStations[i]->row()    == row &&
            m_deliveryStations[i]->column() == column)
        {
            removeDeliveryStation(i);
            return;
        }
}

void DeliveryStationList::clear()
{
    emit preItemRemoved(0);
    m_deliveryStations.clear();
    emit postItemRemoved();
}
