#include "SimulationWindow_Presenter.h"
#include <QDebug>

SimulationWindowPresenter::SimulationWindowPresenter(WarehouseManager &manager, QObject *parent)
    : QObject(parent), manager(manager)
{
    mActors.append(Actors("József", "Lalalala", 100, Actors::Robot, Actors::Up));
    mActors.append(Actors("Robi", "Szökdécsel", 100, Actors::Robot, Actors::Down));
    mActors.append(Actors("Szia", "Lajos", 69, Actors::Robot, Actors::Down));
    mActors.append(Actors("Sus", "Amongus", 42, Actors::Robot, Actors::Down));
}

SimulationWindowPresenter::~SimulationWindowPresenter() {}

const QList<Actors> *SimulationWindowPresenter::actors() const
{
    return &mActors;
}

void SimulationWindowPresenter::simulationStart()
{
    manager.simulationStart();
    qDebug() << "Simulation started...";
}

void SimulationWindowPresenter::simulationStop()
{
    manager.simulationStop();
    qDebug() << "Simulation stopped...";
}

void SimulationWindowPresenter::setTickRate(TickRate tickRate)
{
    switch (tickRate) {
        case NORMAL: manager.setTickRate(1000);
            break;
        case HALF_SPEED: manager.setTickRate(2000);
            break;
        case TWICE: manager.setTickRate(500);
            break;
    }
    qDebug() << "TickRate changed to " << tickRate;
}
