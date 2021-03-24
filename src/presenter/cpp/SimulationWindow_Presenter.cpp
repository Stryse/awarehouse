#include "SimulationWindow_Presenter.h"
#include "Warehouse.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

SimulationWindowPresenter::SimulationWindowPresenter(WarehouseManager &manager, QObject *parent)
    : QObject(parent),
      mActorOutliner(nullptr),
      mOrderOutliner(new OrderOutlinerList(this)),
      manager(manager),
      layout(nullptr)
{
    loadWarehouse(":/maps/Map01.json");
    layout = new WarehouseLayoutPresenter(manager.getDisplayedWarehouse()->getState().get());
    mActorOutliner = new ActorOutlinerList(layout->getActors(),this);
}

SimulationWindowPresenter::~SimulationWindowPresenter() {}

ActorOutlinerList *SimulationWindowPresenter::actors() const
{
    return mActorOutliner;
}

void SimulationWindowPresenter::setActors(ActorOutlinerList *actors)
{
    mActorOutliner = actors;
}

OrderOutlinerList *SimulationWindowPresenter::orders() const
{
    return mOrderOutliner;
}

void SimulationWindowPresenter::setOrders(OrderOutlinerList *orders)
{
    mOrderOutliner = orders;
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
    switch (tickRate)
    {
    case NORMAL:
        manager.setTickRate(1000);
        break;
    case HALF_SPEED:
        manager.setTickRate(2000);
        break;
    case TWICE:
        manager.setTickRate(500);
        break;
    }
    qDebug() << "TickRate changed to " << tickRate;
}

void SimulationWindowPresenter::loadWarehouse(const QString &filePath)
{
    bool success = manager.getDisplayedWarehouse()->loadState(filePath);

    if(success)
        loadedWarehousePath = filePath;
}

void SimulationWindowPresenter::reloadWarehouse()
{
    loadWarehouse(loadedWarehousePath);
}
