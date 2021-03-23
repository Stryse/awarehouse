#ifndef WAREHOUSE_LAYOUT_PRESENTER__H
#define WAREHOUSE_LAYOUT_PRESENTER__H

#include <QObject>
#include <QJsonObject>
#include <QList>
#include "MapItemPresenter.h"

class ChargingStationsPresenter;
class DeliveryRobotPresenter;
class PodDockPresenter;
class PodPresenter;

class WarehouseLayoutPresenter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount WRITE setRowCount NOTIFY rowCountChanged)
    Q_PROPERTY(int colCount READ colCount WRITE setColCount NOTIFY colCountChanged)


public:
        explicit WarehouseLayoutPresenter(QObject *parent = nullptr);
        explicit WarehouseLayoutPresenter(QJsonObject& source,QObject *parent = nullptr);

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

        QList<MapItemPresenter*> map;
        //QList<DeliveryRobotPresenter> robots;
        QList<ChargingStationsPresenter> chargingStations;
        //QList<PodDockPresenter> podDocks;
        //QList<PodPresenter> pods;
};
#endif
