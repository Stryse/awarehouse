#include "SimulationWindow_Presenter.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

SimulationWindowPresenter::SimulationWindowPresenter(WarehouseManager &manager, QObject *parent)
    : QObject(parent), mActorOutliner(new OutlinerList(this)), manager(manager), layout(nullptr)
{
    mOrders.append(Order(0, 1, 3, "3_FELADAT01"));
    mOrders.append(Order(1, 2, 1, "1_FELADAT01"));
    mOrders.append(Order(3, 4, 2, "2_FELADAT01"));
    loadWarehouse(":/maps/Map01.json");
}

SimulationWindowPresenter::~SimulationWindowPresenter() {}

OutlinerList *SimulationWindowPresenter::actors() const
{
    return mActorOutliner;
}

void SimulationWindowPresenter::setActors(OutlinerList* actors)
{
    mActorOutliner = actors;
}

const QList<Order>* SimulationWindowPresenter::orders() const
{
    return &mOrders;
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

void SimulationWindowPresenter::loadWarehouse(const QString& filePath)
{
    QString jsonString;
    QFile sourceFile(filePath);
    if(sourceFile.exists())
        if(sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            jsonString = sourceFile.readAll();
            sourceFile.close();

            QJsonDocument warehouseDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject warehouseJsonObj = warehouseDoc.object();
            layout = new WarehouseLayoutPresenter(warehouseJsonObj,this);
        }
}
