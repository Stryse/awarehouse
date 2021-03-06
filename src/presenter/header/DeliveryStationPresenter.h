#ifndef DELIVERY_STATION_PRESENTER_H
#define DELIVERY_STATION_PRESENTER_H

#include <QObject>
#include <QJsonObject>

//Presenter
#include "MapItemPresenter.h"

class DeliveryStation;

class DeliveryStationPresenter : public MapItemPresenter
{
    Q_OBJECT

public:
    explicit DeliveryStationPresenter(const DeliveryStation* model,
                                                    QObject* parent = nullptr);
    explicit DeliveryStationPresenter(int row, int column, QObject* parent);

    bool operator==(const DeliveryStationPresenter& other) const;

    static DeliveryStationPresenter* loadJsonObject(const QJsonObject& deliveryStationObj,
                                                              QObject* parent = nullptr);
    QJsonObject saveJsonObject(int acceptedOrderID) const;

private:
    //Model
    const  DeliveryStation* model;

    //Static
    static QString m_static_imagePath;
};

#endif /* DELIVERY_STATION_PRESENTER_H */
