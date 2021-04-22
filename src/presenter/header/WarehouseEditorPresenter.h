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

    Q_PROPERTY(                   QString currentWarehouseName READ currentWarehouseName NOTIFY   currentWarehouseNameChanged )
    Q_PROPERTY( WarehouseLayoutPresenter* layout               READ layout               CONSTANT                             )
    Q_PROPERTY(     PersistencePresenter* persistence          READ persistence          CONSTANT                             )

public:
    explicit WarehouseEditorPresenter(PersistencePresenter* persistence = nullptr,
                                                   QObject* parent      = nullptr);

    enum TileType
    {
        ROAD,
        ACTOR,
        CHARGING_STATION,
        POD,
        DELIVERY_STATION
    };
    Q_ENUM(TileType);

public:
    //Getter
    QString                   currentWarehouseName() const;
    WarehouseLayoutPresenter* layout()               const;
    PersistencePresenter*     persistence()          const;

private:
    void updateRowsInTable(int newRows);
    void updateColumnsInTable(int newColumns);

    void        loadJsonObject(const QJsonObject& json);
    QJsonObject saveJsonObject() const;

    void createTileTypeTable(int rows, int columns);

signals:
    void currentWarehouseNameChanged(QString warehouseName);

public slots:
    void setTile(int row, int column, TileType type);
    void removeTile(int row, int column);

    void clearWarehouse();

    void loadWarehouse(const QString& warehouseName);
    bool saveWarehouse(const QString& warehouseName);

    bool isDefaultWarehouse() const;

public:
    static const QString  newWarehouseName;
    static       TileType baseTile;

private:
    QString                    m_currentWarehouseName;

    WarehouseLayoutPresenter*  m_layout;
    QVector<QVector<TileType>> m_tileTypeTable;

    PersistencePresenter*      m_persistence;
};

#endif /* WAREHOUSE_EDITOR_PRESENTER__H */
