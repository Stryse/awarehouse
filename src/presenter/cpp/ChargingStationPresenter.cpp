#include "ChargingStationPresenter.h"

//Model
#include "ChargingStation.h"

ChargingStationPresenter::ChargingStationPresenter(const ChargingStation<int>* model,
                                                                      QObject* parent)
    : MapItemPresenter(model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       ChargingStationPresenter::imagePath,
                       parent)
    , model(model)
{}

QString ChargingStationPresenter::imagePath = "qrc:/images/chstation.png";
