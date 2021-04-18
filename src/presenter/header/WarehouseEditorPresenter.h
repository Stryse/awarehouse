#ifndef WAREHOUSE_EDITOR_PRESENTER__H
#define WAREHOUSE_EDITOR_PRESENTER__H

#include <QObject>
#include <QString>
#include <QVector>
#include <QJsonObject>

//Presenter
#include "WarehouseLayoutPresenter.h"
#include "PersistencePresenter.h"
#include "ChargingStationPresenter.h"

class WarehouseEditorPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY( WarehouseLayoutPresenter* layout      READ layout      CONSTANT )
    Q_PROPERTY(     PersistencePresenter* persistence READ persistence CONSTANT )

public:
    explicit WarehouseEditorPresenter(QObject* parent = nullptr);

    enum TileType
    {
        ROAD,
        ACTOR,
        CHARGING_STATION,
        POD_DOCK,
        DELIVERY_STATION
    };
    Q_ENUM(TileType);

public:
    //Getter
    WarehouseLayoutPresenter* layout()      const;
    PersistencePresenter*     persistence() const;

private:
    void updateRowsInTable(int newRows);
    void updateColumnsInTable(int newColumns);

    void loadJsonObject(const QJsonObject& json);

    void createTileTypeTable(int rows, int columns);

public slots:
    void setTile(int row, int column, TileType type);
    void removeTile(int row, int column);

    void loadWarehouse(const QString& warehouseName);
    void saveWarehouse(const QString& warehouseName);

public:
    static TileType m_baseTile;

private:
    WarehouseLayoutPresenter*  m_layout;
    QVector<QVector<TileType>> m_tileTypeTable;

    PersistencePresenter*      m_persistence;
};

#endif /* WAREHOUSE_EDITOR_PRESENTER__H */
