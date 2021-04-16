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
    explicit ChargingStationPresenter(int row, int column, QObject* parent = nullptr);

    bool operator==(const ChargingStationPresenter& other) const;

private:
    //Model
    const  ChargingStation<int>* model;

    //Static
    static QString m_static_imagePath;
};

#endif /* CHARGING_STATION_PRESENTER__H */
