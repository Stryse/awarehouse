#ifndef CHARGING_STATION_PRESENTER__H
#define CHARGING_STATION_PRESENTER__H

#include <QObject>

//Presenter
#include "MapItemPresenter.h"

class ChargingStation;

class ChargingStationPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit ChargingStationPresenter(const ChargingStation* model,
                                                         QObject* parent = nullptr);

private:
    //Model
    const  ChargingStation* model;

    //Static
    static QString m_static_imagePath;
};

#endif /* CHARGING_STATION_PRESENTER__H */
