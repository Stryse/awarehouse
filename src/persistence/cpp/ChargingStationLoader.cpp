#include "ChargingStationLoader.h"
#include "Point.h"
#include <climits>

std::shared_ptr<ChargingStation> ChargingStationLoader::load(const QJsonObject &chargingStationObj)
{
    if (chargingStationObj.contains("RowCoord") && chargingStationObj["RowCoord"].isDouble() &&
        chargingStationObj.contains("ColCoord") && chargingStationObj["ColCoord"].isDouble())
    {
        Point<> position(chargingStationObj["ColCoord"].toInt(),
                         chargingStationObj["RowCoord"].toInt(),
                         0);

        return std::make_shared<ChargingStation>(position, INT_MAX);
    }
    return nullptr;
}

QJsonObject ChargingStationLoader::save(const ChargingStation &station)
{
    QJsonObject stationObject;
    stationObject.insert("RowCoord", station.getPosition().getPosY());
    stationObject.insert("ColCoord", station.getPosition().getPosX());
    return stationObject;
}
