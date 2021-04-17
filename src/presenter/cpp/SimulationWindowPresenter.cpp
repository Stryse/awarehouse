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
    , m_layout(nullptr)
    , m_maps(nullptr)
{
    m_defaultMapPath = ":/maps/Map01.json";
    m_filePath = "../maps";
    loadWarehouse(m_defaultMapPath);

    createMapDir();

    m_layout = new WarehouseLayoutPresenter(m_manager.getDisplayedWarehouse()->getState().get());
}

//Getter
WarehouseLayoutPresenter* SimulationWindowPresenter::layout() const { return m_layout;      }
bool                      SimulationWindowPresenter::paused() const { return m_manager.getDisplayedWarehouseSimulator()->isAvailable(); }
QStringList               SimulationWindowPresenter::maps()   const { return m_maps.stringList(); }
QString                   SimulationWindowPresenter::filePath() const { return m_filePath; }
QString SimulationWindowPresenter::defaultMapPath() const { return m_defaultMapPath; }


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

void SimulationWindowPresenter::createMapDir()
{
    QDir mapsDirectory(m_filePath);
    qDebug() << mapsDirectory.absolutePath();
    if(!mapsDirectory.exists())
    {
        mapsDirectory.mkdir(m_filePath);
    }

    QStringList maps = mapsDirectory.entryList(QStringList() << "*.json" << "*.JSON", QDir::Files);
    for(int i = 0; i < maps.size(); ++i )
        maps[i].chop(5);

    maps.push_front("Default");
    m_maps.setStringList(maps);
}
