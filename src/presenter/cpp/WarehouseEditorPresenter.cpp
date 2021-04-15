#include "WarehouseEditorPresenter.h"

WarehouseEditorPresenter::WarehouseEditorPresenter(QObject* parent)
    : QObject(parent)
    , m_layout(new WarehouseLayoutPresenter(this))
{
    m_tileTypeTable = QVector<QVector<TileType>>(m_layout->rows());

    for (int i = 0; i < m_layout->rows(); ++i)
        m_tileTypeTable[i] = QVector<TileType>(m_layout->columns(), ROAD);
}

WarehouseLayoutPresenter* WarehouseEditorPresenter::layout() const { return m_layout; }

void WarehouseEditorPresenter::setTile(int row, int column, TileType type)
{
    removeTile(row, column);

    switch (type)
    {
        case ACTOR:
            m_layout->actors()          ->appendActor          (*new ActorPresenter(row, column, m_layout));
            break;
        case CHARGING_STATION:
            m_layout->chargingStations()->appendChargingStation(*new ChargingStationPresenter(m_layout));
            break;
        case POD_DOCK:
            m_layout->podDocks()        ->appendPodDock        (*new PodDockPresenter(m_layout));
            break;
        case DELIVERY_STATION:
            m_layout->deliveryStations()->appendDeliveryStation(*new DeliveryStationPresenter(m_layout));
            break;
        default: break;
    }
    m_tileTypeTable[row][column] = type;
}

void WarehouseEditorPresenter::removeTile(int row, int column)
{
    switch (m_tileTypeTable[row][column])
    {
        case ACTOR:
            m_layout->actors()          ->removeActor          (row, column);
            break;
        case CHARGING_STATION:
            m_layout->chargingStations()->removeChargingStation(row, column);
            break;
        case POD_DOCK:
            m_layout->podDocks()        ->removePodDock        (row, column);
            break;
        case DELIVERY_STATION:
            m_layout->deliveryStations()->removeDeliveryStation(row, column);
            break;
        default: return;
    }

    m_tileTypeTable[row][column] = ROAD;
}
