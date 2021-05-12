#include "SimulationWindowPresenter.h"

#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include <QDebug>

//Model
#include "Warehouse.h"

SimulationWindowPresenter::SimulationWindowPresenter(PersistencePresenter* persistence,
                                                                  QObject* parent)
    : QObject(parent)
    , m_layout(new WarehouseLayoutPresenter(this))
    , m_paused(true)
    , m_persistence(persistence == nullptr ? new PersistencePresenter(this) : persistence)
    , m_time(0)

{
    loadWarehouse(PersistencePresenter::defaultWarehouseName);

    m_manager.getDisplayedWarehouseSimulator()->onTick.connect([=](){
        setTime(m_manager.getDisplayedWarehouse()->getTimeStamp());
    });
}

//Getter
WarehouseLayoutPresenter* SimulationWindowPresenter::layout()      const { return m_layout;      }
Settings*                 SimulationWindowPresenter::settings()          { return &m_settings;   }
bool                      SimulationWindowPresenter::paused()      const { return m_paused;      }
PersistencePresenter*     SimulationWindowPresenter::persistence() const { return m_persistence; }
int                       SimulationWindowPresenter::time()        const { return m_time;        }

//Setter
void SimulationWindowPresenter::setPaused(bool paused)
{
    if (m_paused == paused)
        return;

    m_paused = paused;
    emit pausedChanged();
}

void SimulationWindowPresenter::setTime(int time)
{
    if(m_time == time)
        return;

    m_time = time;
    emit timeChanged();
}

void SimulationWindowPresenter::simulationStart()
{
    setPaused(false);
    m_manager.simulationStart();
    qDebug() << "Simulation started...";
}

void SimulationWindowPresenter::simulationStop()
{
    setPaused(true);
    m_manager.simulationStop();
    qDebug() << "Simulation stopped...";
}

void SimulationWindowPresenter::setTickRate(TickRate tickRate)
{
    switch (tickRate)
    {
        case NORMAL:
            m_manager.setTickRate(300);
            break;
        case HALF_SPEED:
            m_manager.setTickRate(500);
            break;
        case TWICE:
            m_manager.setTickRate(150);
            break;
    }
    qDebug() << "TickRate changed to " << tickRate;
}

int SimulationWindowPresenter::getCurrentWarehouseIndex() const
{
    return m_persistence->getWarehouseIndex(m_currentWarehouseName);
}

void SimulationWindowPresenter::loadWarehouse(const QString& warehouseName)
{
    bool success = m_manager.getDisplayedWarehouse()->loadState(PersistencePresenter::getWarehousePath(warehouseName), &m_settings);

    if(success)
    {
        m_layout->loadWarehouseLayout(m_manager.getDisplayedWarehouse()->getState().get());
        m_currentWarehouseName = warehouseName;
        setTime(0);
        emit m_layout->actors()->sumEnergyChanged();
        emit m_layout->actors()->sumMovesChanged();
    }
}

void SimulationWindowPresenter::reloadWarehouse()
{
    simulationStop();
    loadWarehouse(m_currentWarehouseName);
}

void SimulationWindowPresenter::exportStatistics() const
{
    QString currTime = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm");

    QDir statisticsDir("../stats");

    if (!statisticsDir.exists())
        statisticsDir.mkdir("../stats");

    QFile statisticsFile("../stats/statistics_" + currTime + ".txt");

    if (statisticsFile.open(QIODevice::WriteOnly))
    {
        QTextStream statStream(&statisticsFile);

        statStream << "Total Time Spent: "    << m_time                                                                                        << "\n" <<
                      "Total Energy Used: "   << m_layout->actors()->sumEnergy()                                                               << "\n" <<
                      "Total Move Count: "    << m_layout->actors()->sumEnergy()                                                               << "\n" <<
                      "Average Energy Used: " << std::round(m_layout->actors()->sumEnergy() / (double)m_layout->actors()->count() * 100) / 100 << "\n" <<
                      "Average Move Count: "  << std::round(m_layout->actors()->sumMoves()  / (double)m_layout->actors()->count() * 100) / 100 << "\n\n";

        for (const auto& actor : *m_layout->actors()->actors())
            statStream << "Energy Used: " << actor->energyUsed() << " | Move Count: " << actor->moves() << "\n";

        statisticsFile.close();
    }
}
