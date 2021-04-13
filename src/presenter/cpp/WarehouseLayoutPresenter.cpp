#include "WarehouseLayoutPresenter.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>

//Model
#include "State.h"

//Presenter
#include "ChargingStationPresenter.h"
#include "PodDockPresenter.h"
#include "RoadPresenter.h"

WarehouseLayoutPresenter::WarehouseLayoutPresenter(const State* state, QObject* parent)
    : QObject(parent)
{
    //Connect row and column to model
    setRows(state->getRowCount());
    setColumns(state->getColCount());
    m_map.resize(m_rows * m_columns);

    //Connect charging stations presenter to model
    m_chargingStations.reserve(state->getChargingStations().size());
    for(auto& station : state->getChargingStations())
    {
        auto chPresenter = new ChargingStationPresenter(station.get(),this);
        m_chargingStations.append(chPresenter);
        m_map[index(station->getPosition().getPosX(),
                    station->getPosition().getPosY())] = chPresenter;
    }

    //Connect pod docks presenter to model
    m_podDocks.reserve(state->getPodDocks().size());
    for(auto& podDock : state->getPodDocks())
    {
        auto pdPresenter = new PodDockPresenter(podDock.get(),this);
        m_podDocks.append(pdPresenter);
        m_map[index(podDock->getPosition().getPosX(),
                    podDock->getPosition().getPosY())] = pdPresenter;
    }

    //Connect robots presenter to model
    m_robots.reserve(state->getRobots().size());
    for(auto& robot : state->getRobots())
    {
        auto acPresenter = new ActorPresenter(robot.get(),this);
        m_robots.append(acPresenter);
        m_map[index(robot->getBody()->getPose().getPosition().getPosX(),
                    robot->getBody()->getPose().getPosition().getPosY())] = acPresenter;
    }

    //Fill other tiles with road
    for(int x = 0; x < m_rows; ++x)
        for(int y = 0; y < m_columns; ++y)
        {
            if (m_map[x * m_rows + y] == nullptr)
                m_map[x * m_columns + y] = new RoadPresenter(x,y,this);
        }
}

int WarehouseLayoutPresenter::index(int row, int col) { return row * m_columns + col; }

int WarehouseLayoutPresenter::rows()    const { return m_rows; }
int WarehouseLayoutPresenter::columns() const { return m_columns; }

void WarehouseLayoutPresenter::setRows(int rows)       { m_rows    = rows;
                                                          emit rowsChanged(m_rows);   }
void WarehouseLayoutPresenter::setColumns(int columns) { m_columns = columns;
                                                          emit columnsChanged(m_columns); }

QList<const MapItemPresenter*>*         WarehouseLayoutPresenter::map()              { return &m_map;              }
QList<const ActorPresenter*>*           WarehouseLayoutPresenter::actors()           { return &m_robots;           }
QList<const ChargingStationPresenter*>* WarehouseLayoutPresenter::chargingStations() { return &m_chargingStations; }
QList<const PodDockPresenter*>*         WarehouseLayoutPresenter::podDocks()         { return &m_podDocks;         }
QList<const TaskPresenter*>*            WarehouseLayoutPresenter::tasks()            { return &m_tasks;            }

