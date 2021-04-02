#include "ChargingStationLoader.h"
#include "Point.h"

std::shared_ptr<ChargingStation<>> ChargingStationLoader::load(const QJsonObject &chargingStationObj)
{
    if (chargingStationObj.contains("RowCoord") && chargingStationObj["RowCoord"].isDouble() &&
        chargingStationObj.contains("ColCoord") && chargingStationObj["ColCoord"].isDouble())
    {
        Point<> position(chargingStationObj["ColCoord"].toInt(),
                         chargingStationObj["RowCoord"].toInt(),
                         0);

        return std::make_shared<ChargingStation<>>(position, 1); // TODO PARAMETERIZE
    }
    return nullptr;
}

QJsonObject ChargingStationLoader::save(const ChargingStation<> &station)
{
}