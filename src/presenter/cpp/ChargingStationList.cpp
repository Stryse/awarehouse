#include "ChargingStationList.h"

ChargingStationList::ChargingStationList(QObject* parent)
    : QObject(parent)
{}

QList<ChargingStationPresenter*>* ChargingStationList::chargingStations() { return &m_chargingStations; }

bool ChargingStationList::setChargingStationAt(int index, ChargingStationPresenter& chargingStation)
{
    if (index < 0 ||
        index >= m_chargingStations.size())
        return false;

    const ChargingStationPresenter* oldchargingStation = m_chargingStations.at(index);
    if (chargingStation == *oldchargingStation)
        return false;

    m_chargingStations[index] = &chargingStation;
    return true;
}

void ChargingStationList::appendChargingStation(ChargingStationPresenter& chargingStation)
{
    emit preItemAppended();
    m_chargingStations.append(&chargingStation);
    emit postItemAppended();
}

void ChargingStationList::removeChargingStation(int index)
{
    if (index < 0 ||
        index >= m_chargingStations.size())
        return;

    emit preItemRemoved(index);
    m_chargingStations.removeAt(index);
    emit postItemRemoved();
}

void ChargingStationList::removeChargingStation(int row, int column)
{
    if (row < 0 || column < 0)
        return;

    int index = -1;
    for (int i = 0; i < m_chargingStations.size(); ++i)
        if (m_chargingStations[i]->row()    == row &&
            m_chargingStations[i]->column() == column)
        {
            index = i;
            break;
        }

    if (index == -1)
        return;

    emit preItemRemoved(index);
    m_chargingStations.removeAt(index);
    emit postItemRemoved();
}

void ChargingStationList::clear()
{
    emit preItemRemoved(0);
    m_chargingStations.clear();
    emit postItemRemoved();
}
