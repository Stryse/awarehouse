#include "SimulationWindowPresenter.h"

#include <QDebug>
#include <QDir>
#include <QDebug>

//Model
#include "Warehouse.h"

SimulationWindowPresenter::SimulationWindowPresenter(PersistencePresenter* persistence,
                                                                  QObject* parent)
    : QObject(parent)
    , m_layout(new WarehouseLayoutPresenter(this))
    , m_persistence(persistence == nullptr ? new PersistencePresenter(this) : persistence)
{
    loadWarehouse(PersistencePresenter::defaultWarehouseName);
}

//Getter
WarehouseLayoutPresenter* SimulationWindowPresenter::layout()      const { return m_layout;                                                  }
Settings*                 SimulationWindowPresenter::settings()          { return &m_settings;                                               }
bool                      SimulationWindowPresenter::paused()      const { return m_manager.getDisplayedWarehouseSimulator()->isAvailable(); }
PersistencePresenter*     SimulationWindowPresenter::persistence() const { return m_persistence;                                             }

//Setter
void SimulationWindowPresenter::setPaused(bool paused)
{
    if (m_paused == paused)
        return;

    m_paused = paused;
    emit pausedChanged();
}

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

void SimulationWindowPresenter::loadWarehouse(const QString& warehouseName)
{
    bool success = m_manager.getDisplayedWarehouse()->loadState(PersistencePresenter::getWarehousePath(warehouseName), &m_settings);

    if(success)
    {
        m_layout->loadWarehouseLayout(m_manager.getDisplayedWarehouse()->getState().get());
        m_currentWarehouseName = warehouseName;
    }
}

void SimulationWindowPresenter::reloadWarehouse()
{
    loadWarehouse(m_currentWarehouseName);
    simulationStop();
}
