#include "SimulationWindowPresenter.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

//Model
#include "Warehouse.h"

SimulationWindowPresenter::SimulationWindowPresenter(QObject* parent)
    : QObject(parent)
    , m_layout(nullptr)
{
    loadWarehouse(":/maps/Map01.json");

    m_layout = new WarehouseLayoutPresenter(m_manager.getDisplayedWarehouse()->getState().get());
}

//Getter
WarehouseLayoutPresenter* SimulationWindowPresenter::layout() const { return m_layout;      }
bool                      SimulationWindowPresenter::paused() const { return m_manager.getDisplayedWarehouseSimulator()->isAvailable(); }

//Setter
void SimulationWindowPresenter::setPaused(bool paused)              { m_paused    = paused;
                                                                      emit pausedChanged(); }

void SimulationWindowPresenter::simulationStart()
{
    m_manager.simulationStart();
    qDebug() << "Simulation started...";
}

void SimulationWindowPresenter::simulationStop()
{
    m_manager.simulationStop();
    qDebug() << "Simulation stopped...";
}

void SimulationWindowPresenter::setTickRate(TickRate tickRate)
{
    switch (tickRate)
    {
        case NORMAL:
            m_manager.setTickRate(1000);
            break;
        case HALF_SPEED:
            m_manager.setTickRate(2000);
            break;
        case TWICE:
            m_manager.setTickRate(500);
            break;
    }
    qDebug() << "TickRate changed to " << tickRate;
}

void SimulationWindowPresenter::loadWarehouse(const QString& filePath)
{
    bool success = m_manager.getDisplayedWarehouse()->loadState(filePath);

    if(success)
        m_loadedWarehousePath = filePath;
}

void SimulationWindowPresenter::reloadWarehouse()
{
    loadWarehouse(m_loadedWarehousePath);
    simulationStop();
}
