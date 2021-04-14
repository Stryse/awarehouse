#include "DeliveryStationList.h"

DeliveryStationList::DeliveryStationList(QObject* parent)
    : QObject(parent)
{}

QList<const DeliveryStationPresenter*>* DeliveryStationList::deliveryStations() { return &m_deliveryStations; }

bool DeliveryStationList::setDeliveryStationAt(int index, const DeliveryStationPresenter& deliveryStation)
{
    if (index < 0 ||
        index >= m_deliveryStations.size())
        return false;

    m_deliveryStations[index] = &deliveryStation;
    return true;
}

void DeliveryStationList::appendDeliveryStation(const DeliveryStationPresenter& deliveryStation)
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

void DeliveryStationList::clear()
{
    emit preItemRemoved(0);
    m_deliveryStations.clear();
    emit postItemRemoved();
}
