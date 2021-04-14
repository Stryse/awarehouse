#include "DeliveryStationLoader.h"
#include "Point.h"

std::shared_ptr<DeliveryStation> DeliveryStationLoader::load(const QJsonObject &deliveryStationObj)
{
    if (deliveryStationObj.contains("RowCoord") && deliveryStationObj["RowCoord"].isDouble() &&
        deliveryStationObj.contains("ColCoord") && deliveryStationObj["ColCoord"].isDouble() &&
        deliveryStationObj.contains("AcceptedOrderID") && deliveryStationObj["AcceptedOrderID"].isDouble())
    {
        Point<> position(deliveryStationObj["ColCoord"].toInt(),
                         deliveryStationObj["RowCoord"].toInt(),
                         0);

        return std::make_shared<DeliveryStation>(position, deliveryStationObj["AcceptedOrderID"].toInt());
    }
    return nullptr;
}

QJsonObject DeliveryStationLoader::save(const DeliveryStation &deliveryStation)
{
    QJsonObject stationObject;
    stationObject.insert("RowCoord", deliveryStation.getPosition().getPosY());
    stationObject.insert("ColCoord", deliveryStation.getPosition().getPosX());
    stationObject.insert("AcceptedOrderID", deliveryStation.getAcceptedOrderID());
    return stationObject;
}
