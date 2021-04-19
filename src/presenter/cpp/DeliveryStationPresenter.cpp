#include "DeliveryStationPresenter.h"

//Model
#include "DeliveryStation.h"

QString DeliveryStationPresenter::m_static_imagePath = "qrc:/placeholder_amogus.png";

DeliveryStationPresenter::DeliveryStationPresenter(const DeliveryStation* model,
                                                                 QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       DeliveryStationPresenter::m_static_imagePath,
                       parent)
    , model(model)
{}

DeliveryStationPresenter::DeliveryStationPresenter(int row, int column, QObject* parent)
    : MapItemPresenter(row,
                       column,
                       DeliveryStationPresenter::m_static_imagePath,
                       parent)
{}

bool DeliveryStationPresenter::operator==(const DeliveryStationPresenter& other) const
{
    return MapItemPresenter::operator==(other);
}

DeliveryStationPresenter* DeliveryStationPresenter::loadJsonObject(const QJsonObject& deliveryStationObj,
                                                                             QObject* parent)
{
    if (deliveryStationObj.contains("RowCoord") && deliveryStationObj["RowCoord"].isDouble() &&
        deliveryStationObj.contains("ColCoord") && deliveryStationObj["ColCoord"].isDouble() &&
        deliveryStationObj.contains("AcceptedOrderID") && deliveryStationObj["AcceptedOrderID"].isDouble())
    {
        int row    = deliveryStationObj["ColCoord"].toInt();
        int column = deliveryStationObj["RowCoord"].toInt();

        return new DeliveryStationPresenter(column, row, parent);
    }

    return nullptr;
}

QJsonObject DeliveryStationPresenter::saveJsonObject(int acceptedOrderID) const
{
    QJsonObject deliveryStationJsonObj;

    deliveryStationJsonObj.insert("RowCoord", row());
    deliveryStationJsonObj.insert("ColCoord", column());
    deliveryStationJsonObj.insert("AcceptedOrderID", acceptedOrderID);

    return deliveryStationJsonObj;
}
