#ifndef WAREHOUSE_LAYOUT_PRESENTER__H
#define WAREHOUSE_LAYOUT_PRESENTER__H

#include "Actors.h"
#include "ChargingStationPresenter.h"
#include "MapItemPresenter.h"
#include "PodDockPresenter.h"
#include "Order.h"
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
        Q_PROPERTY(QList<const MapItemPresenter *>* map READ getMap CONSTANT)

public:
        explicit WarehouseLayoutPresenter(const State* state, QObject *parent = nullptr);

public:
        int rowCount() const;
        void setRowCount(int value);

        int colCount() const;
        void setColCount(int colCount);

        // Entity Getter
        QList<const MapItemPresenter *>* getMap();
        QList<const Actors*>* getActors();
        QList<const ChargingStationPresenter*>* getChargingStations();
        QList<const PodDockPresenter*>* getPodDocks();
        QList<const Order*>* getOrders();

        int index(int row, int col);
signals:
        void rowCountChanged(int rowCount);
        void colCountChanged(int colCount);

private:
        int m_rowCount;
        int m_colCount;

        // Warehouse Layout Entities
        QList<const MapItemPresenter *> m_map;
        QList<const Actors*> robots;
        QList<const ChargingStationPresenter*> chargingStations;
        QList<const PodDockPresenter*> podDocks;

        // Orders
        QList<const Order*> orders;
};
#endif
