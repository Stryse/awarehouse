#include "ChargingStationPresenter.h"

ChargingStationPresenter::ChargingStationPresenter(int rowCoord,
                                                   int colCoord,
                                                   QObject* parent)

    : MapItemPresenter(ChargingStationPresenter::imagePath, rowCoord,colCoord, parent)
{
}

ChargingStationPresenter* ChargingStationPresenter::read(QJsonObject& jsonObj, QObject* parent)
{
    if(jsonObj.contains("RowCoord") && jsonObj["RowCoord"].isDouble() &&
       jsonObj.contains("ColCoord") && jsonObj["ColCoord"].isDouble())
    {
        int rowCoord = jsonObj["RowCoord"].toInt();
        int colCoord = jsonObj["ColCoord"].toInt();
        return new ChargingStationPresenter(rowCoord,colCoord,parent);
    }
    return nullptr;
}

QString ChargingStationPresenter::imagePath = "asd.png";

