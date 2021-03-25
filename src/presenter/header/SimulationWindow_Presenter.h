#ifndef SIMULATION_WINDOW_PRESENTER__H
#define SIMULATION_WINDOW_PRESENTER__H

#include "Actors.h"
#include "Order.h"
#include "WarehouseManager.h"
#include "WarehouseLayoutPresenter.h"
#include "OutlinerList.h"
#include "OrderOutlinerList.h"
#include "ISimulator.h"
#include <QObject>
#include <QVector>

class SimulationWindowPresenter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(OrderOutlinerList* orders READ orders CONSTANT)
    Q_PROPERTY(ActorOutlinerList* actors READ actors CONSTANT)
    Q_PROPERTY(bool paused READ paused NOTIFY pausedChanged)

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
    ActorOutlinerList *actors() const;
    void setActors(ActorOutlinerList* actors);

    OrderOutlinerList *orders() const;
    void setOrders(OrderOutlinerList* orders);

    bool paused() const;


signals:
    void pausedChanged();

public slots:
    void simulationStart();
    void simulationStop();
    void setTickRate(TickRate tickRate);
    void loadWarehouse(const QString& filePath);
    void reloadWarehouse();
    //void reloadSimulation();

private:
    void setPaused(bool paused);

private:
    ActorOutlinerList* mActorOutliner;
    OrderOutlinerList* mOrderOutliner;

    WarehouseManager &manager;
    WarehouseLayoutPresenter* layout;
    QString loadedWarehousePath;
    bool m_paused;
};

#endif
