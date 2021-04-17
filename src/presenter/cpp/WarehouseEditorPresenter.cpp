#include "WarehouseEditorPresenter.h"

#include "WarehouseLayoutPresenter.h"

WarehouseEditorPresenter::TileType WarehouseEditorPresenter::m_baseTile = ROAD;

WarehouseEditorPresenter::WarehouseEditorPresenter(QObject* parent)
    : QObject(parent)
    , m_layout(new WarehouseLayoutPresenter(this))
    , m_tileTypeTable(QVector<QVector<TileType>>())
{
    for (int i = 0; i < m_layout->rows(); ++i)
        m_tileTypeTable.emplace_back(QVector<TileType>(m_layout->columns(), m_baseTile));

    connect(m_layout, &WarehouseLayoutPresenter::rowsChanged,    this, [=](int rows)    { updateRowsInTable(rows);       });
    connect(m_layout, &WarehouseLayoutPresenter::columnsChanged, this, [=](int columns) { updateColumnsInTable(columns); });
}

WarehouseLayoutPresenter* WarehouseEditorPresenter::layout() const { return m_layout; }

void WarehouseEditorPresenter::updateRowsInTable(int newRows)
{
    int rowDelta = newRows - m_tileTypeTable.size();

    if (rowDelta > 0)
        for (int i = 0; i < rowDelta; ++i)
            m_tileTypeTable.emplace_back(QVector<TileType>(m_layout->columns(), m_baseTile));
    else
    {
        for (int i = 0; i < rowDelta * -1; ++i)
            m_tileTypeTable.pop_back();

        //Actors
        ActorList& actors = *m_layout->actors();
        for (int i = 0; i < actors.actors()->size(); ++i)
        {
            ActorPresenter& actor = *actors.actors()->at(i);
            if (actor.row() >= newRows)
                actors.removeActor(i);
        }

        //ChargingStations
        ChargingStationList& chargingStations = *m_layout->chargingStations();
        for (int i = 0; i < chargingStations.chargingStations()->size(); ++i)
        {
            ChargingStationPresenter& chargingStation = *chargingStations.chargingStations()->at(i);
            if (chargingStation.row() >= newRows)
                chargingStations.removeChargingStation(i);
        }

        //PodDocks
        PodDockList& poDocks = *m_layout->podDocks();
        for (int i = 0; i < poDocks.podDocks()->size(); ++i)
        {
            PodDockPresenter& podDock = *poDocks.podDocks()->at(i);
            if (podDock.row() >= newRows)
                poDocks.removePodDock(i);
        }

        //ChargingStations
        DeliveryStationList& deliveryStations = *m_layout->deliveryStations();
        for (int i = 0; i < deliveryStations.deliveryStations()->size(); ++i)
        {
            DeliveryStationPresenter& deliveryStation = *deliveryStations.deliveryStations()->at(i);
            if (deliveryStation.row() >= newRows)
                deliveryStations.removeDeliveryStation(i);
        }
    }
}

void WarehouseEditorPresenter::updateColumnsInTable(int newColumns)
{
    int columnDelta = newColumns - m_tileTypeTable[0].size();

    if (columnDelta > 0)
        for (int i = 0; i < m_layout->rows(); ++i)
            for (int j = 0; j < columnDelta; ++j)
                m_tileTypeTable[i].emplace_back(m_baseTile);
    else
    {
        for (int i = 0; i < m_layout->rows(); ++i)
            for (int j = 0; j < columnDelta * -1; ++j)
                m_tileTypeTable[i].pop_back();

        //Actors
        ActorList& actors = *m_layout->actors();
        for (int i = 0; i < actors.actors()->size(); ++i)
        {
            ActorPresenter& actor = *actors.actors()->at(i);
            if (actor.column() >= newColumns)
                actors.removeActor(i);
        }

        //ChargingStations
        ChargingStationList& chargingStations = *m_layout->chargingStations();
        for (int i = 0; i < chargingStations.chargingStations()->size(); ++i)
        {
            ChargingStationPresenter& chargingStation = *chargingStations.chargingStations()->at(i);
            if (chargingStation.column() >= newColumns)
                chargingStations.removeChargingStation(i);
        }

        //PodDocks
        PodDockList& poDocks = *m_layout->podDocks();
        for (int i = 0; i < poDocks.podDocks()->size(); ++i)
        {
            PodDockPresenter& podDock = *poDocks.podDocks()->at(i);
            if (podDock.column() >= newColumns)
                poDocks.removePodDock(i);
        }

        //ChargingStations
        DeliveryStationList& deliveryStations = *m_layout->deliveryStations();
        for (int i = 0; i < deliveryStations.deliveryStations()->size(); ++i)
        {
            DeliveryStationPresenter& deliveryStation = *deliveryStations.deliveryStations()->at(i);
            if (deliveryStation.column() >= newColumns)
                deliveryStations.removeDeliveryStation(i);
        }
    }
}

void WarehouseEditorPresenter::setTile(int row, int column, TileType type)
{
    removeTile(row, column);

    switch (type)
    {
        case ACTOR:
            m_layout->actors()->appendActor(*new ActorPresenter(row, column, m_layout));
            break;
        case CHARGING_STATION:
            m_layout->chargingStations()->appendChargingStation(*new ChargingStationPresenter(row, column, m_layout));
            break;
        case POD_DOCK:
            m_layout->podDocks()->appendPodDock(*new PodDockPresenter(row, column, m_layout));
            break;
        case DELIVERY_STATION:
            m_layout->deliveryStations()->appendDeliveryStation(*new DeliveryStationPresenter(row, column, m_layout));
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
            m_layout->actors()->removeActor(row, column);
            break;
        case CHARGING_STATION:
            m_layout->chargingStations()->removeChargingStation(row, column);
            break;
        case POD_DOCK:
            m_layout->podDocks()->removePodDock(row, column);
            break;
        case DELIVERY_STATION:
            m_layout->deliveryStations()->removeDeliveryStation(row, column);
            break;
        default: return;
    }

    m_tileTypeTable[row][column] = m_baseTile;
}
