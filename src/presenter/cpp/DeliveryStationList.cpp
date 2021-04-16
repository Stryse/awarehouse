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
    return true;
}

void DeliveryStationList::appendDeliveryStation(DeliveryStationPresenter& deliveryStation)
{
    emit preItemAppended();
    m_deliveryStations.append(&deliveryStation);
    emit postItemAppended();
}

void DeliveryStationList::removeDeliveryStation(int index)
{
    if (index < 0 ||
        index >= m_deliveryStations.size())
        return;

    emit preItemRemoved(index);
    m_deliveryStations.removeAt(index);
    emit postItemRemoved();
}

void DeliveryStationList::removeDeliveryStation(int row, int column)
{
    if (row < 0 || column < 0)
        return;

    int index = -1;
    for (int i = 0; i < m_deliveryStations.size(); ++i)
        if (m_deliveryStations[i]->row()    == row &&
            m_deliveryStations[i]->column() == column)
        {
            index = i;
            break;
        }

    if (index == -1)
        return;

    emit preItemRemoved(index);
    m_deliveryStations.removeAt(index);
    emit postItemRemoved();
}

void DeliveryStationList::clear()
{
    emit preItemRemoved(0);
    m_deliveryStations.clear();
    emit postItemRemoved();
}
