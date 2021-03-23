#ifndef WAREHOUSE_LAYOUT_PRESENTER__H
#define WAREHOUSE_LAYOUT_PRESENTER__H

#include "Actors.h"
#include "ChargingStationPresenter.h"
#include "MapItemPresenter.h"
#include "PodDockPresenter.h"
#include <QJsonObject>
#include <QList>
#include <QObject>

class WarehouseLayoutPresenter : public QObject
{
        Q_OBJECT
        Q_PROPERTY(int rowCount READ rowCount WRITE setRowCount NOTIFY rowCountChanged)
        Q_PROPERTY(int colCount READ colCount WRITE setColCount NOTIFY colCountChanged)

public:
        explicit WarehouseLayoutPresenter(QObject *parent = nullptr);
        explicit WarehouseLayoutPresenter(QJsonObject &source, QObject *parent = nullptr);

public:
        int rowCount() const;
        void setRowCount(int value);

        int colCount() const;
        void setColCount(int colCount);

signals:
        void rowCountChanged();
        void colCountChanged();

private:
        int m_rowCount;
        int m_colCount;

        QList<MapItemPresenter *> map;
        QList<Actors *> robots;
        QList<ChargingStationPresenter *> chargingStations;
        QList<PodDockPresenter *> podDocks;
        //QList<PodPresenter> pods;
};
#endif
