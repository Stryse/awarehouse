#ifndef CHARGING_STATION_PRESENTER__H
#define CHARGING_STATION_PRESENTER__H

#include <QObject>
#include <QJsonObject>

//Presenter
#include "MapItemPresenter.h"

class ChargingStation;

class ChargingStationPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit ChargingStationPresenter(const ChargingStation* model,
                                                    QObject* parent = nullptr);
    explicit ChargingStationPresenter(int row, int column, QObject* parent = nullptr);

    bool operator==(const ChargingStationPresenter& other) const;

    static ChargingStationPresenter* loadJsonObject(const QJsonObject& chargingStationObj,
                                                              QObject* parent = nullptr);
    QJsonObject saveJsonObject() const;

private:
    //Model
    const  ChargingStation* model;

    //Static
    static QString m_static_imagePath;
};

#endif /* CHARGING_STATION_PRESENTER__H */
