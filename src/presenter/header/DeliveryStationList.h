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

    QList<DeliveryStationPresenter*>* deliveryStations();

    bool setDeliveryStationAt(int index, DeliveryStationPresenter& deliveryStation);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void dataChanged(int index);

public slots:
    void appendDeliveryStation(DeliveryStationPresenter& deliveryStation);
    void removeDeliveryStation(int index);
    void removeDeliveryStation(int row, int column);

    void clear();

public:
    QList<DeliveryStationPresenter*> m_deliveryStations;
};

#endif /* DELIVERY_STATION_LIST__H */
