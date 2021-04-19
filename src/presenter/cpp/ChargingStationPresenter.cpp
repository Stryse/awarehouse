#include "ChargingStationPresenter.h"

//Model
#include "ChargingStation.h"

QString ChargingStationPresenter::m_static_imagePath = "qrc:/images/chstation.png";

ChargingStationPresenter::ChargingStationPresenter(const ChargingStation* model,
                                                                 QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       ChargingStationPresenter::m_static_imagePath,
                       parent)
    , model(model)
{}

ChargingStationPresenter::ChargingStationPresenter(int row, int column, QObject* parent)
    : MapItemPresenter(row,
                       column,
                       ChargingStationPresenter::m_static_imagePath,
                       parent)
{}

bool ChargingStationPresenter::operator==(const ChargingStationPresenter& other) const
{
    return MapItemPresenter::operator==(other);
}

ChargingStationPresenter* ChargingStationPresenter::loadJsonObject(const QJsonObject& chargingStationObj,
                                                                             QObject* parent)
{
    if (chargingStationObj.contains("RowCoord") && chargingStationObj["RowCoord"].isDouble() &&
        chargingStationObj.contains("ColCoord") && chargingStationObj["ColCoord"].isDouble())
    {
        int row    = chargingStationObj["ColCoord"].toInt();
        int column = chargingStationObj["RowCoord"].toInt();

        return new ChargingStationPresenter(column, row, parent);
    }

    return nullptr;
}

QJsonObject ChargingStationPresenter::saveJsonObject() const
{
    QJsonObject chargingStationJsonObj;

    chargingStationJsonObj.insert("RowCoord", row());
    chargingStationJsonObj.insert("ColCoord", column());

    return chargingStationJsonObj;
}
