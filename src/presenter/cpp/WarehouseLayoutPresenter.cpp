#include "WarehouseLayoutPresenter.h"
#include "ChargingStationPresenter.h"
#include "PodDockPresenter.h"
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

    // Connect charging stations presenter to model
    chargingStations.reserve(state->getChargingStations().size());
    for(auto& station : state->getChargingStations())
        chargingStations.append(new ChargingStationPresenter(station.get(),this));

    // Connect pod docks presenter to model
    podDocks.reserve(state->getPodDocks().size());
    for(auto& podDock : state->getPodDocks())
        podDocks.append(new PodDockPresenter(podDock.get(),this));

    // Connect robots presenter to model
    robots.reserve(state->getRobots().size());
    for(auto& robot : state->getRobots())
        robots.append(new Actors(robot.get(),this));
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

QList<const MapItemPresenter*>* WarehouseLayoutPresenter::getMap() { return &map; }
QList<const Actors*>* WarehouseLayoutPresenter::getActors() { return &robots; }
QList<const ChargingStationPresenter*>* WarehouseLayoutPresenter::getChargingStations() { return &chargingStations; }
QList<const PodDockPresenter*>* WarehouseLayoutPresenter::getPodDocks() { return &podDocks; }
