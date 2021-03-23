#ifndef CHARGING_STATION_PRESENTER__H
#define CHARGING_STATION_PRESENTER__H

#include <QObject>
#include <MapItemPresenter.h>
#include <QJsonObject>

class ChargingStationPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit ChargingStationPresenter(int rowCoord,
                                      int colCoord,
                                      QObject *parent = nullptr);


    static ChargingStationPresenter* read(QJsonObject& jsonObj, QObject *parent = nullptr);
private:

    static QString imagePath;
};

#endif /* CHARGING_STATION_PRESENTER__H */
