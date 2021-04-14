#ifndef CHARGING_STATION_PRESENTER__H
#define CHARGING_STATION_PRESENTER__H

#include <QObject>

//Presenter
#include "MapItemPresenter.h"

template <typename T> class ChargingStation;

class ChargingStationPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit ChargingStationPresenter(const ChargingStation<int>* model,
                                                         QObject* parent = nullptr);

private:
    //Model
    const  ChargingStation<int>* model;

    //Static
    static QString imagePath;
};

#endif /* CHARGING_STATION_PRESENTER__H */
