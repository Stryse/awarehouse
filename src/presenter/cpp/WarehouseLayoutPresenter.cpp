#include "WarehouseLayoutPresenter.h"
#include "ChargingStationPresenter.h"
#include "PodDockPresenter.h"
#include "RoadPresenter.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include "State.h"

WarehouseLayoutPresenter::WarehouseLayoutPresenter(const State* state, QObject* parent)
    : QObject(parent)
{
    // Connect row and column to model
    setRowCount(state->getRowCount());
    setColCount(state->getColCount());
    m_map.resize(m_colCount * m_rowCount);

    // Connect charging stations presenter to model
    chargingStations.reserve(state->getChargingStations().size());
    for(auto& station : state->getChargingStations())
    {
        auto chPresenter = new ChargingStationPresenter(station.get(),this);
        chargingStations.append(chPresenter);
        m_map[index(station->getPosition().getPosX(),
                    station->getPosition().getPosY())] = chPresenter;
    }

    // Connect pod docks presenter to model
    podDocks.reserve(state->getPodDocks().size());
    for(auto& podDock : state->getPodDocks())
    {
        auto pdPresenter = new PodDockPresenter(podDock.get(),this);
        podDocks.append(pdPresenter);
        m_map[index(podDock->getPosition().getPosX(),
                    podDock->getPosition().getPosY())] = pdPresenter;
    }

    // Connect robots presenter to model
    robots.reserve(state->getRobots().size());
    for(auto& robot : state->getRobots())
    {
        auto acPresenter = new Actors(robot.get(),this);
        robots.append(acPresenter);
        m_map[index(robot->getBody()->getPose().getPosition().getPosX(),
                    robot->getBody()->getPose().getPosition().getPosY())] = acPresenter;
    }

    // Fill other tiles with road
    for(int x = 0; x < m_rowCount; ++x)
        for(int y = 0; y < m_colCount; ++y)
        {
            if(m_map[x * m_rowCount + y] == nullptr)
                m_map[x * m_rowCount + y] = new RoadPresenter(x,y,this);
        }
}

int WarehouseLayoutPresenter::rowCount() const
{
    return m_rowCount;
}

void WarehouseLayoutPresenter::setRowCount(int value)
{
    m_rowCount = value;
    emit rowCountChanged(m_rowCount);
}

int WarehouseLayoutPresenter::colCount() const
{
    return m_colCount;
}

void WarehouseLayoutPresenter::setColCount(int colCount)
{
    m_colCount = colCount;
    emit colCountChanged(m_colCount);
}

QList<const MapItemPresenter*>* WarehouseLayoutPresenter::getMap() { return &m_map; }

QList<const Actors*>* WarehouseLayoutPresenter::getActors() { return &robots; }

QList<const ChargingStationPresenter*>* WarehouseLayoutPresenter::getChargingStations() { return &chargingStations; }

QList<const PodDockPresenter*>* WarehouseLayoutPresenter::getPodDocks() { return &podDocks; }

QList<const Order*>* WarehouseLayoutPresenter::getOrders() { return &orders; }

int WarehouseLayoutPresenter::index(int row, int col) { return row * m_rowCount + col; }
