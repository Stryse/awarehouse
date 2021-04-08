#include "WarehouseFilePersistence.h"
#include "Point.h"
#include "State.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <memory>
#include <vector>

WarehouseFilePersistence::WarehouseFilePersistence() {}
WarehouseFilePersistence::~WarehouseFilePersistence() {}

State *WarehouseFilePersistence::load(const QString &resource)
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

bool WarehouseFilePersistence::save(const State &state, const QString &resource) const
{
    QFile saveFile(resource);
    if(!saveFile.open(QIODevice::WriteOnly))
        return false;

    // Object root
    QJsonObject saveObject;

    // Primary child
    QJsonObject warehouseLayoutData;


    warehouseLayoutData.insert("RowCount", state.getRowCount());
    warehouseLayoutData.insert("ColCount", state.getColCount());

    // Write Charging Stations
    QJsonArray chargingStations;
    for(const auto& station : state.getChargingStations())
    {
        QJsonObject stationObject;
        stationObject.insert("RowCoord", station->getPosition().getPosY());
        stationObject.insert("ColCoord", station->getPosition().getPosX());
        chargingStations.append(stationObject);
    }
    warehouseLayoutData.insert("ChargingStations",chargingStations);

    // Write PodDocks
    QJsonArray podDocks;
    for(const auto& podDock : state.getPodDocks())
    {
        QJsonObject podDockObject;
        podDockObject.insert("RowCoord", podDock->getPosition().getPosY());
        podDockObject.insert("ColCoord", podDock->getPosition().getPosX());
        podDocks.append(podDockObject);
    }
    warehouseLayoutData.insert("PodDocks",podDocks);

    // Write Robots
    QJsonArray robots;
    for(const auto& robot : state.getRobots())
    {
        QJsonObject robotObject;
        robotObject.insert("RowCoord",robot->getBody()->getPose().getPosition().getPosY());
        robotObject.insert("ColCoord",robot->getBody()->getPose().getPosition().getPosX());
        robotObject.insert("OrientationY",robot->getBody()->getPose().getOrientation().getY());
        robots.append(robotObject);
    }
    warehouseLayoutData.insert("DeliveryRobots",robots);

    saveObject.insert("WarehouseLayoutData",warehouseLayoutData);
    saveFile.write(QJsonDocument(saveObject).toJson());
    return true;
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
        std::shared_ptr<ObservableNavEnvironment<>> env = std::make_shared<ObservableNavEnvironment<>>(rowSize, colSize, 3);
        std::vector<std::shared_ptr<ChargingStation<>>> chStations;
        std::vector<std::shared_ptr<PodDock<>>> podDocks;
        std::vector<std::shared_ptr<DeliveryRobot<>>> robots;

        // Read Charging Stations
        loadChargingStation(chStations, env, WarehouseLayoutData);

        // Read PodDocks
        loadPodDock(podDocks, env, WarehouseLayoutData);

        // Fill empty volumes with default tile
        fillEmptyVolumes(env);

        // Read Robots
        loadRobots(robots, env, WarehouseLayoutData);

        // Construct State from read data
        return new State(std::move(env),
                         std::move(chStations),
                         std::move(podDocks),
                         std::move(robots),
                         rowSize,colSize);
    }
    return nullptr;
}

void WarehouseFilePersistence::fillEmptyVolumes(std::shared_ptr<ObservableNavEnvironment<>> &env)
{
    for (size_t x = 0; x < env->getXLength(); ++x)
        for (size_t y = 0; y < env->getYLength(); ++y)
            for (size_t z = 0; z < env->getZLength(); ++z)
            {
                if (env->getBuffer()[env->getCoordAsIndex(x, y, z)] == nullptr)
                    env->getBuffer()[env->getCoordAsIndex(x, y, z)] = std::make_shared<Tile>(Point<>(x, y, z));
            }
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
            if (podObj.contains("RowCoord") && podObj["RowCoord"].isDouble() &&
                podObj.contains("ColCoord") && podObj["ColCoord"].isDouble())
            {
                Point<> position(podObj["ColCoord"].toInt(),
                                 podObj["RowCoord"].toInt(),
                                 0);

                std::shared_ptr<PodDock<>> podDock = std::make_shared<PodDock<>>(position);
                env->getBuffer()[env->getCoordAsIndex(position)] = podDock;
                podDock->addAssociatedPod(env);
                podDocks.push_back(podDock);
            }
        }
    }
}

void WarehouseFilePersistence::loadRobots(std::vector<std::shared_ptr<DeliveryRobot<>>> &robots,
                                          std::shared_ptr<ObservableNavEnvironment<>> &env,
                                          QJsonObject &warehouseLayoutData)
{
    if (warehouseLayoutData.contains("DeliveryRobots") && warehouseLayoutData["DeliveryRobots"].isArray())
    {
        QJsonArray RobotsArray = warehouseLayoutData["DeliveryRobots"].toArray();
        robots.reserve(RobotsArray.size());

        for (int i = 0; i < RobotsArray.size(); ++i)
        {
            QJsonObject robotObj = RobotsArray[i].toObject();
            if (robotObj.contains("RowCoord") && robotObj["RowCoord"].isDouble() &&
                robotObj.contains("ColCoord") && robotObj["ColCoord"].isDouble() &&
                robotObj.contains("OrientationY") && robotObj["OrientationY"].isDouble())
            {
                Point<> position(robotObj["ColCoord"].toInt(),
                                 robotObj["RowCoord"].toInt(),
                                 0);

                // TODO make maybe unique pointer
                robots.push_back(std::make_shared<DeliveryRobot<>>(env, position, DirectionVector<>(0, robotObj["OrientationY"].toInt(), 0)));
            }
        }
    }
}
