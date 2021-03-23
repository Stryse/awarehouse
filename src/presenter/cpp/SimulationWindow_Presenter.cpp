#include "SimulationWindow_Presenter.h"
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

SimulationWindowPresenter::SimulationWindowPresenter(WarehouseManager &manager, QObject *parent)
    : QObject(parent), mActorOutliner(new OutlinerList(this)), manager(manager), layout(nullptr)
{
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
