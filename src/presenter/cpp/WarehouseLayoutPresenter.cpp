#include "WarehouseLayoutPresenter.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

//Model
#include "State.h"

WarehouseLayoutPresenter::WarehouseLayoutPresenter(const State* state,
                                                       QObject* parent)
    : QObject(parent)
{
    //Connect row and column to model
    setRows(state->getRowCount());
    setColumns(state->getColCount());

    //Connect robots presenter to model
    m_robots.actors()->reserve(state->getRobots().size());
    for(auto& robot : state->getRobots())
    {
        auto acPresenter = new ActorPresenter(robot.get(),this);
        m_robots.actors()->append(acPresenter);
    }

    //Connect charging stations presenter to model
    m_chargingStations.chargingStations()->reserve(state->getChargingStations().size());
    for(auto& station : state->getChargingStations())
    {
        auto csPresenter = new ChargingStationPresenter(station.get(),this);
        m_chargingStations.chargingStations()->append(csPresenter);
    }

    //Connect pod docks presenter to model
    m_podDocks.podDocks()->reserve(state->getPodDocks().size());
    for(auto& podDock : state->getPodDocks())
    {
        auto pdPresenter = new PodDockPresenter(podDock.get(),this);
        m_podDocks.podDocks()->append(pdPresenter);
    }

    //Connect charging stations presenter to model
    m_deliveryStations.deliveryStations()->reserve(state->getDeliveryStations().size());
    for(auto& station : state->getDeliveryStations())
    {
        auto dsPresenter = new DeliveryStationPresenter(station.get(),this);
        m_deliveryStations.deliveryStations()->append(dsPresenter);
    }
}

WarehouseLayoutPresenter::WarehouseLayoutPresenter(QObject* parent)
    : QObject(parent)
    , m_rows   (10)
    , m_columns(10)
    //Other?
{}

//Getter
int  WarehouseLayoutPresenter::index(int row,
                                     int col)  { return row * m_rows + col;    }

int  WarehouseLayoutPresenter::rows()    const { return m_rows;                }
int  WarehouseLayoutPresenter::columns() const { return m_columns;             }

//Setter
void WarehouseLayoutPresenter::setRows(int rows)       { m_rows    = rows;
                                                          emit rowsChanged(m_rows);       }
void WarehouseLayoutPresenter::setColumns(int columns) { m_columns = columns;
                                                          emit columnsChanged(m_columns); }

//Entity Getter
ActorList*           WarehouseLayoutPresenter::actors()           { return &m_robots;           }
ChargingStationList* WarehouseLayoutPresenter::chargingStations() { return &m_chargingStations; }
PodDockList*         WarehouseLayoutPresenter::podDocks()         { return &m_podDocks;         }
DeliveryStationList* WarehouseLayoutPresenter::deliveryStations() { return &m_deliveryStations; }
TaskList*            WarehouseLayoutPresenter::tasks()            { return &m_tasks;            }

const ActorList*           WarehouseLayoutPresenter::actors()           const { return &m_robots;           }
const ChargingStationList* WarehouseLayoutPresenter::chargingStations() const { return &m_chargingStations; }
const PodDockList*         WarehouseLayoutPresenter::podDocks()         const { return &m_podDocks;         }
const DeliveryStationList* WarehouseLayoutPresenter::deliveryStations() const { return &m_deliveryStations; }
const TaskList*            WarehouseLayoutPresenter::tasks()            const { return &m_tasks;            }

