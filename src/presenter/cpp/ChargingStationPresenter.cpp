#include "ChargingStationPresenter.h"
#include "ChargingStation.h"



QString ChargingStationPresenter::imagePath = "qrc:/images/chstation.png";

ChargingStationPresenter::ChargingStationPresenter(const ChargingStation<int>* model, QObject* parent)
    : MapItemPresenter(ChargingStationPresenter::imagePath,
                       model->getPosition().getPosY(),
                       model->getPosition().getPosX(),
                       parent)
    ,model(model)
{

}
