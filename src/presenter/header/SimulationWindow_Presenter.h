#ifndef SIMULATION_WINDOW_PRESENTER__H
#define SIMULATION_WINDOW_PRESENTER__H

#include "Actors.h"
#include "WarehouseManager.h"
#include "ISimulator.h"
#include <QObject>
#include <QVector>

class SimulationWindowPresenter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QList<Actors> *actors READ actors NOTIFY actorsChanged)

public:
    enum TickRate
    {
        HALF_SPEED,
        NORMAL,
        TWICE
    };
    Q_ENUM(TickRate);


    SimulationWindowPresenter(WarehouseManager &manager, QObject *parent = nullptr);
    virtual ~SimulationWindowPresenter();

public:
    const QList<Actors> *actors() const;

public:
signals:
    void actorsChanged();

public slots:
    void simulationStart();
    void simulationStop();
    void setTickRate(TickRate tickRate);

private:
    QList<Actors> mActors;
    WarehouseManager &manager;
};

#endif
