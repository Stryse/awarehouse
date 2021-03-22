#ifndef SIMULATION_WINDOW_PRESENTER__H
#define SIMULATION_WINDOW_PRESENTER__H

#include <QObject>
#include <QVector>
#include "Actors.h"

class SimulationWindowPresenter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QList<Actors>* actors READ actors NOTIFY actorsChanged)

public:
    SimulationWindowPresenter(QObject *parent = nullptr);
    virtual ~SimulationWindowPresenter();

public:
    const QList<Actors>* actors() const;

public: signals:
    void actorsChanged();

private:
    QList<Actors> mActors;
};

#endif
