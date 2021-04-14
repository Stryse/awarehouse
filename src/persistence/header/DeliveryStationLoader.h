#ifndef DELIVERY_STATION_LOADER__H
#define DELIVERY_STATION_LOADER__H

#include "DeliveryStation.h"
#include <QJsonObject>
#include <memory>

class DeliveryStationLoader
{
public:
    static std::shared_ptr<DeliveryStation> load(const QJsonObject &deliveryStationObj);
    static QJsonObject save(const DeliveryStation &deliveryStation);
};

#endif /* DELIVERY_STATION_LOADER__H */