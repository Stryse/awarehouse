#include "SimulationWindowPresenter.h"

#include <QDebug>
#include <QDir>
#include <QDebug>

//Model
#include "Warehouse.h"

SimulationWindowPresenter::SimulationWindowPresenter(QObject* parent)
    : QObject(parent)
    , m_layout(nullptr)
    , m_persistence(new PersistencePresenter(this))
{
    loadWarehouse(m_persistence->defaultWarehousePath(), &m_settings);
    m_layout = new WarehouseLayoutPresenter(m_manager.getDisplayedWarehouse()->getState().get(), this);
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

void SimulationWindowPresenter::loadWarehouse(const  QString& warehousePath,
                                              const Settings* settings)
{
    bool success = m_manager.getDisplayedWarehouse()->loadState(warehousePath, settings);

    if(success)
        m_currentWarehousePath = warehousePath;
}

void SimulationWindowPresenter::reloadWarehouse()
{
    loadWarehouse(m_currentWarehousePath, &m_settings);
    simulationStop();
}
