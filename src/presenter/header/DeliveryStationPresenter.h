#ifndef DELIVERY_STATION_PRESENTER_H
#define DELIVERY_STATION_PRESENTER_H

#include <QObject>

//Presenter
#include "MapItemPresenter.h"

class DeliveryStation;

class DeliveryStationPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit DeliveryStationPresenter(const DeliveryStation* model,
                                                    QObject* parent = nullptr);
private:
    //Model
    const  DeliveryStation* model;

    //Static
    static QString m_static_imagePath;
};

#endif /* DELIVERY_STATION_PRESENTER_H */
