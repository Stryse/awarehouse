#ifndef SIMULATION_WINDOW_PRESENTER__H
#define SIMULATION_WINDOW_PRESENTER__H

#include "Actors.h"
#include "Order.h"
#include "WarehouseManager.h"
#include "WarehouseLayoutPresenter.h"
#include "OutlinerList.h"
#include "ISimulator.h"
#include <QObject>
#include <QVector>

class SimulationWindowPresenter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QList<Order> *orders READ orders NOTIFY ordersChanged)
    Q_PROPERTY(OutlinerList* actors READ actors CONSTANT)

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
    OutlinerList *actors() const;
    void setActors(OutlinerList* actors);
    const QList<Order> *orders() const;

public:
signals:
    void ordersChanged();

public slots:
    void simulationStart();
    void simulationStop();
    void setTickRate(TickRate tickRate);
    void loadWarehouse(const QString& filePath);
    void addActor();
    //void reloadSimulation();

private:
    OutlinerList* mActorOutliner;
    QList<Order> mOrders;
    WarehouseManager &manager;
    WarehouseLayoutPresenter* layout;
};

#endif
