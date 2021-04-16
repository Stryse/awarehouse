#ifndef CHARGING_STATION_LOADER__H
#define CHARGING_STATION_LOADER__H

#include "ChargingStation.h"
#include <QJsonObject>
#include <memory>

class ChargingStationLoader
{
public:
    static std::shared_ptr<ChargingStation> load(const QJsonObject &chargingStationObj);
    static QJsonObject save(const ChargingStation &station);
};

#endif /* CHARGING_STATION_LOADER__H */
