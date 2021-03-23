#include "WarehouseFilePersistence.h"
#include "Point.h"
#include "State.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <memory>
#include <vector>

WarehouseFilePersistence::WarehouseFilePersistence(QObject *parent)
    : QObject(parent)
{
}

WarehouseFilePersistence::~WarehouseFilePersistence()
{
}

State *WarehouseFilePersistence::load(QString &resource)
{
    QString jsonString;
    QFile sourceFile(resource);
    if (sourceFile.exists())
        if (sourceFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            jsonString = sourceFile.readAll();
            sourceFile.close();

            QJsonDocument warehouseDoc = QJsonDocument::fromJson(jsonString.toUtf8());
            QJsonObject warehouseJsonObj = warehouseDoc.object();
            return loadFromJsonObject(warehouseJsonObj);
        }
    return nullptr;
}

void WarehouseFilePersistence::save(const State &state, QString &resource) const
{
}

State *WarehouseFilePersistence::loadFromJsonObject(QJsonObject json)
{

    if (json.contains("WarehouseLayoutData") && json["WarehouseLayoutData"].isObject())
    {
        QJsonObject WarehouseLayoutData = json["WarehouseLayoutData"].toObject();
        int rowSize;
        int colSize;

        // Read Row and Column
        if (WarehouseLayoutData.contains("RowCount") && WarehouseLayoutData["RowCount"].isDouble() &&
            WarehouseLayoutData.contains("ColCount") && WarehouseLayoutData["ColCount"].isDouble())
        {
            rowSize = WarehouseLayoutData["RowCount"].toInt();
            colSize = WarehouseLayoutData["ColCount"].toInt();
        }

        // Create Environment
        std::shared_ptr<ObservableNavEnvironment<>> env = std::make_shared<ObservableNavEnvironment<>>(colSize, rowSize, 3);
        std::vector<std::shared_ptr<ChargingStation<>>> chStations;
        std::vector<std::shared_ptr<PodDock<>>> podDocks;
        std::vector<std::shared_ptr<DeliveryRobot<>>> robots;

        // Read Charging Stations
        loadChargingStation(chStations, env, WarehouseLayoutData);

        // Read PodDocks
        loadPodDock(podDocks,env,WarehouseLayoutData);
    }
    return nullptr;
}

void WarehouseFilePersistence::loadChargingStation(std::vector<std::shared_ptr<ChargingStation<>>> &chStations,
                                                   std::shared_ptr<ObservableNavEnvironment<>> &env,
                                                   QJsonObject &warehouseLayoutData)
{
    if (warehouseLayoutData.contains("ChargingStations") && warehouseLayoutData["ChargingStations"].isArray())
    {
        QJsonArray chargingStationsJson = warehouseLayoutData["ChargingStations"].toArray();
        chStations.reserve(chargingStationsJson.size());

        for (int i = 0; i < chargingStationsJson.size(); ++i)
        {
            QJsonObject chargingStationObj = chargingStationsJson[i].toObject();
            if (chargingStationObj.contains("RowCoord") && chargingStationObj["RowCoord"].isDouble() &&
                chargingStationObj.contains("ColCoord") && chargingStationObj["ColCoord"].isDouble())
            {
                Point<> position(chargingStationObj["ColCoord"].toInt(),
                                 chargingStationObj["RowCoord"].toInt(),
                                 0);

                std::shared_ptr<ChargingStation<>> station = std::make_shared<ChargingStation<>>(position, 1); // TODO PARAMETERIZE
                env->getBuffer()[env->getCoordAsIndex(position)] = station;
                chStations.push_back(station);
            }
        }
    }
}

void WarehouseFilePersistence::loadPodDock(std::vector<std::shared_ptr<PodDock<>>> &podDocks,
                                           std::shared_ptr<ObservableNavEnvironment<>> &env,
                                           QJsonObject &warehouseLayoutData)
{
    if (warehouseLayoutData.contains("PodDocks") && warehouseLayoutData["PodDocks"].isArray())
    {
        QJsonArray PodDocksJSon = warehouseLayoutData["PodDocks"].toArray();
        podDocks.reserve(PodDocksJSon.size());

        for (int i = 0; i < PodDocksJSon.size(); ++i)
        {
            QJsonObject podObj = PodDocksJSon[i].toObject();
            if(podObj.contains("RowCoord") && podObj["RowCoord"].isDouble() &&
               podObj.contains("ColCoord") && podObj["ColCoord"].isDouble())
            {
                Point<> position(podObj["ColCoord"].toInt(),
                                 podObj["RowCoord"].toInt(),
                                 0);

                std::shared_ptr<PodDock<>> podDock = std::make_shared<PodDock<>>(position);
                podDock->addAssociatedPod(env);

                env->getBuffer()[env->getCoordAsIndex(position)] = podDock;
                podDocks.push_back(podDock);
            }
        }
    }
}
