#ifndef CHARGING_STATION_PRESENTER__H
#define CHARGING_STATION_PRESENTER__H

#include <QObject>
#include <MapItemPresenter.h>

class ChargingStationPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit ChargingStationPresenter(const QString& imagePath,
                                      int rowCoord,
                                      int colCoord,
                                      QObject *parent = nullptr);

private:

    static QString imagePath;
};

#endif /* CHARGING_STATION_PRESENTER__H */
