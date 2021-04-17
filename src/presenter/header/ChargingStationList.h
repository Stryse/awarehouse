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

    QList<ChargingStationPresenter*>* chargingStations();

    bool setChargingStationAt(int index, ChargingStationPresenter& chargingStation);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

    void dataChanged(int index);

public slots:
    void appendChargingStation(ChargingStationPresenter& chargingStation);
    void removeChargingStation(int index);
    void removeChargingStation(int row, int column);

    void clear();

public:
    QList<ChargingStationPresenter*> m_chargingStations;
};

#endif /* CHARGING_STATION_LIST__H */