#ifndef CHARGING_STATION_PRESENTER__H
#define CHARGING_STATION_PRESENTER__H

#include <QObject>
#include <MapItemPresenter.h>

template <typename T> class ChargingStation;
class ChargingStationPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit ChargingStationPresenter(const ChargingStation<int>* model, QObject *parent = nullptr);

private:

    const ChargingStation<int>* model;
    static QString imagePath;
};

#endif /* CHARGING_STATION_PRESENTER__H */
