#include "ChargingStationPresenter.h"

//Model
#include "ChargingStation.h"

ChargingStationPresenter::ChargingStationPresenter(const ChargingStation* model,
                                                                      QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       ChargingStationPresenter::m_static_imagePath,
                       parent)
    , model(model)
{}

QString ChargingStationPresenter::m_static_imagePath = "qrc:/images/chstation.png";
