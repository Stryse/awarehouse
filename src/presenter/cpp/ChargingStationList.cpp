#include "ChargingStationList.h"

#include <QJsonObject>

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
    emit dataChanged(index);
    return true;
}

void ChargingStationList::loadJsonArray(const QJsonArray& chargingStationsJson)
{
    clear();

    for (int i = 0; i < chargingStationsJson.size(); ++i)
    {
        ChargingStationPresenter* chargingStation = ChargingStationPresenter::loadJsonObject(chargingStationsJson[i].toObject(), this);
        if (chargingStation != nullptr)
            appendChargingStation(*chargingStation);
    }
}

QJsonArray ChargingStationList::saveJsonArray() const
{
    QJsonArray chargingStationsJsonArray;
    for (const auto& chargingStation : m_chargingStations)
        chargingStationsJsonArray.append(chargingStation->saveJsonObject());

    return chargingStationsJsonArray;
}

void ChargingStationList::appendChargingStation(ChargingStationPresenter& chargingStation)
{
    emit preItemAppended();

    int last = m_chargingStations.size();
    m_chargingStations.append(&chargingStation);
    connect(&chargingStation, &ChargingStationPresenter::mapItemChanged, this, [=]()
    {
        emit dataChanged(last);
    });

    emit postItemAppended();

}

void ChargingStationList::removeChargingStation(int index)
{
    if (index < 0 ||
        index >= m_chargingStations.size())
        return;

    emit preItemRemoved(index);

    m_chargingStations[index]->disconnect(this);
    m_chargingStations.removeAt(index);
    for (int i = index; i < m_chargingStations.size(); ++i)
    {
        m_chargingStations[i]->disconnect(this);
        connect(m_chargingStations[i], &MapItemPresenter::mapItemChanged, this, [=]()
        {
            emit dataChanged(i);
        });
    }

    emit postItemRemoved();
}

void ChargingStationList::removeChargingStation(int row, int column)
{
    if (row < 0 || column < 0)
        return;

    for (int i = 0; i < m_chargingStations.size(); ++i)
        if (m_chargingStations[i]->row()    == row &&
            m_chargingStations[i]->column() == column)
        {
            removeChargingStation(i);
            return;
        }
}

void ChargingStationList::clear()
{
    if (m_chargingStations.size() == 0)
        return;

    for (int i = m_chargingStations.size() - 1; i >= 0; --i)
        removeChargingStation(i);
}
