#ifndef WAREHOUSE_LAYOUT_PRESENTER__H
#define WAREHOUSE_LAYOUT_PRESENTER__H

#include "Actors.h"
#include "ChargingStationPresenter.h"
#include "MapItemPresenter.h"
#include "PodDockPresenter.h"
#include <QJsonObject>
#include <QList>
#include <QObject>

class State;
//

class WarehouseLayoutPresenter : public QObject
{
        Q_OBJECT
        Q_PROPERTY(int rowCount READ rowCount WRITE setRowCount NOTIFY rowCountChanged)
        Q_PROPERTY(int colCount READ colCount WRITE setColCount NOTIFY colCountChanged)

public:
        explicit WarehouseLayoutPresenter(const State* state, QObject *parent = nullptr);

public:
        int rowCount() const;
        void setRowCount(int value);

        int colCount() const;
        void setColCount(int colCount);

        QList<const MapItemPresenter *>* getMap();
        QList<const Actors*>* getActors();
        QList<const ChargingStationPresenter*>* getChargingStations();
        QList<const PodDockPresenter*>* getPodDocks();


signals:
        void rowCountChanged(int rowCount);
        void colCountChanged(int colCount);

private:
        int m_rowCount;
        int m_colCount;

        QList<const MapItemPresenter *> map;
        QList<const Actors*> robots;
        QList<const ChargingStationPresenter*> chargingStations;
        QList<const PodDockPresenter*> podDocks;
        //QList<PodPresenter> pods;
};
#endif
