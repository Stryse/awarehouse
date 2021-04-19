#include "WarehouseLayoutPresenter.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

//Model
#include "State.h"

WarehouseLayoutPresenter::WarehouseLayoutPresenter(QObject* parent)
    : QObject(parent)
    , m_rows   (10)
    , m_columns(10)
{}

//Getter
int WarehouseLayoutPresenter::index(int row,
                                    int col) { return row * m_rows + col; }

int WarehouseLayoutPresenter::rows()    const { return m_rows;    }
int WarehouseLayoutPresenter::columns() const { return m_columns; }

//Setter
void WarehouseLayoutPresenter::setRows(int rows)
{
    if (m_rows == rows)
        return;

    m_rows = rows;
    emit rowsChanged(m_rows);
}
void WarehouseLayoutPresenter::setColumns(int columns)
{
    if (m_columns == columns)
        return;

    m_columns = columns;
    emit columnsChanged(m_columns);
}

//Entity Getter
ActorList           *WarehouseLayoutPresenter::actors()           { return &m_actors;           }
ChargingStationList *WarehouseLayoutPresenter::chargingStations() { return &m_chargingStations; }
PodDockList         *WarehouseLayoutPresenter::podDocks()         { return &m_podDocks;         }
DeliveryStationList *WarehouseLayoutPresenter::deliveryStations() { return &m_deliveryStations; }
TaskList            *WarehouseLayoutPresenter::tasks()            { return &m_tasks;            }

const ActorList           *WarehouseLayoutPresenter::actors()           const { return &m_actors;           }
const ChargingStationList *WarehouseLayoutPresenter::chargingStations() const { return &m_chargingStations; }
const PodDockList         *WarehouseLayoutPresenter::podDocks()         const { return &m_podDocks;         }
const DeliveryStationList *WarehouseLayoutPresenter::deliveryStations() const { return &m_deliveryStations; }
const TaskList            *WarehouseLayoutPresenter::tasks()            const { return &m_tasks;            }

void WarehouseLayoutPresenter::loadWarehouseLayout(const State* state)
{
    clear();

    setRows(state->getRowCount());
    setColumns(state->getColCount());

    //Connect robots presenter to model
    m_actors.actors()->reserve(state->getRobots().size());
    auto& actors = state->getRobots();
    for (size_t i = 0; i < actors.size(); ++i)
    {
        auto acPresenter = new ActorPresenter(actors[i].get(), this);
        m_actors.appendActor(*acPresenter);
    }

    //Connect charging stations presenter to model
    m_chargingStations.chargingStations()->reserve(state->getChargingStations().size());
    auto& chargingStations = state->getChargingStations();
    for (size_t i = 0; i < chargingStations.size(); ++i)
    {
        auto csPresenter = new ChargingStationPresenter(chargingStations[i].get(), this);
        m_chargingStations.appendChargingStation(*csPresenter);
    }

    //Connect pod docks presenter to model
    m_podDocks.podDocks()->reserve(state->getPodDocks().size());
    auto& podDocks = state->getPodDocks();
    for (size_t i = 0; i < podDocks.size(); ++i)
    {
        auto pdPresenter = new PodDockPresenter(podDocks[i].get(), this);
        m_podDocks.appendPodDock(*pdPresenter);
    }

    //Connect pod presenter to model
    m_pods.pods()->reserve(state->getPods().size());
    auto& pods = state->getPods();
    for (size_t i = 0; i < pods.size(); ++i)
    {
        auto podPresenter = new PodPresenter(pods[i],this);
        m_pods.appendPod(*podPresenter);
    }

    //Connect charging stations presenter to model
    m_deliveryStations.deliveryStations()->reserve(state->getDeliveryStations().size());
    auto& deliveryStations = state->getDeliveryStations();
    for (size_t i = 0; i < deliveryStations.size(); ++i)
    {
        auto dsPresenter = new DeliveryStationPresenter(deliveryStations[i].get(),this);
        m_deliveryStations.appendDeliveryStation(*dsPresenter);
    }

    //Connect tasks presenter to model
    m_tasks.tasks()->reserve(state->getTaskManager().getTasks().size());
    auto& tasks = state->getTaskManager().getTasks();
    for (size_t i = 0; i < tasks.size(); ++i)
    {
         auto taskPresenter = new TaskPresenter(tasks[i].get(), this);
         m_tasks.tasks()->append(taskPresenter);
    }
}

void WarehouseLayoutPresenter::clear()
{
    m_actors.clear();
    m_chargingStations.clear();
    m_podDocks.clear();
    m_deliveryStations.clear();
    m_tasks.clear();
}
