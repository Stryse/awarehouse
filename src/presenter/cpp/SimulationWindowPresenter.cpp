#include "SimulationWindowPresenter.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QDebug>

//Model
#include "Warehouse.h"

SimulationWindowPresenter::SimulationWindowPresenter(QObject* parent)
    : QObject(parent)
    , m_warehouses          (nullptr            )
    , m_defaultWarehousePath(":/maps/Map01.json")
    , m_warehouseDirPath    ("../maps"          )
    , m_layout              (nullptr            )
{
    createMapDir();

    loadWarehouse(m_defaultWarehousePath, &m_settings);
    m_layout = new WarehouseLayoutPresenter(m_manager.getDisplayedWarehouse()->getState().get());
}

//Getter
QStringList               SimulationWindowPresenter::warehouses()           const { return m_warehouses.stringList();                                 }
QString                   SimulationWindowPresenter::defaultWarehousePath() const { return m_defaultWarehousePath;                                    }
QString                   SimulationWindowPresenter::warehouseDirPath()     const { return m_warehouseDirPath;                                        }
WarehouseLayoutPresenter* SimulationWindowPresenter::layout()               const { return m_layout;                                                  }
Settings*                 SimulationWindowPresenter::settings()                   { return &m_settings;                                               }
bool                      SimulationWindowPresenter::paused()               const { return m_manager.getDisplayedWarehouseSimulator()->isAvailable(); }


//Setter
void SimulationWindowPresenter::setPaused(bool paused)
{
    m_paused = paused;
    emit pausedChanged();
}

void SimulationWindowPresenter::createMapDir()
{
    QDir mapsDirectory(m_warehouseDirPath);
    if(!mapsDirectory.exists())
        mapsDirectory.mkdir(m_warehouseDirPath);

    QStringList maps = mapsDirectory.entryList(QStringList() << "*.json" << "*.JSON", QDir::Files);
    for(int i = 0; i < maps.size(); ++i )
        maps[i].chop(5);

    maps.push_front("Default");
    m_warehouses.setStringList(maps);
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
