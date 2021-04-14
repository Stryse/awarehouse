#ifndef DELIVERY_STATION_LIST__H
#define DELIVERY_STATION_LIST__H

#include <QObject>
#include <QList>

//Presenter
#include "DeliveryStationPresenter.h"

class DeliveryStationList : public QObject
{
    Q_OBJECT

public:
    explicit DeliveryStationList(QObject* parent = nullptr);

    QList<const DeliveryStationPresenter*>* deliveryStations();

    bool setDeliveryStationAt(int index, const DeliveryStationPresenter& deliveryStation);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendDeliveryStation(const DeliveryStationPresenter& deliveryStation);
    void removeDeliveryStation(int index);

    void clear();

public:
    QList<const DeliveryStationPresenter*> m_deliveryStations;
};

#endif /* DELIVERY_STATION_LIST__H */
