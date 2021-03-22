#include "SimulationWindow_Presenter.h"

SimulationWindowPresenter::SimulationWindowPresenter(QObject *parent)
    : QObject(parent)
{
    mActors.append(Actors("József", "Lalalala", 100, Actors::Robot, Actors::Up));
    mActors.append(Actors("Robi", "Szökdécsel", 100, Actors::Robot, Actors::Down));
    mActors.append(Actors("Szia", "Lajos", 69, Actors::Robot, Actors::Down));
    mActors.append(Actors("Sus", "Amongus", 42, Actors::Robot, Actors::Down));
    emit actorsChanged();
}

SimulationWindowPresenter::~SimulationWindowPresenter() {}

const QList<Actors> *SimulationWindowPresenter::actors() const
{
    return &mActors;
}
