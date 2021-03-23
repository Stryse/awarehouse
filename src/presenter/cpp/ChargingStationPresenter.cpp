#include "ChargingStationPresenter.h"

ChargingStationPresenter::ChargingStationPresenter(const QString& imagePath,
                                                   int rowCoord,
                                                   int colCoord,
                                                   QObject* parent)
    : MapItemPresenter(ChargingStationPresenter::imagePath, rowCoord,colCoord, parent)
{
}

QString ChargingStationPresenter::imagePath = "asd.png";

