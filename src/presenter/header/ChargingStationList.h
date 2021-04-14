#ifndef CHARGING_STATION_LIST__H
#define CHARGING_STATION_LIST__H

#include <QObject>
#include <QList>

//Presenter
#include "ChargingStationPresenter.h"

class ChargingStationList : public QObject
{
    Q_OBJECT

public:
    explicit ChargingStationList(QObject* parent = nullptr);

    QList<const ChargingStationPresenter*>* chargingStations();

    bool setChargingStationAt(int index, const ChargingStationPresenter& chargingStation);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendChargingStation(const ChargingStationPresenter& chargingStation);
    void removeChargingStation(int index);

    void clear();

public:
    QList<const ChargingStationPresenter*> m_chargingStations;
};

#endif /* CHARGING_STATION_LIST__H */
