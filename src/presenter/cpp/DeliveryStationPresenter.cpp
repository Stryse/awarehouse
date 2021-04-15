#include "DeliveryStationPresenter.h"

//Model
#include "DeliveryStation.h"

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

QString DeliveryStationPresenter::m_static_imagePath = "qrc:/placeholder_amogus.png";
