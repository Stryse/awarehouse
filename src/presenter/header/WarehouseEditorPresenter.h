#ifndef WAREHOUSE_EDITOR_PRESENTER__H
#define WAREHOUSE_EDITOR_PRESENTER__H

#include <QObject>
#include <QString>
#include <QVector>

//Presenter
#include "WarehouseLayoutPresenter.h"

class WarehouseEditorPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY( WarehouseLayoutPresenter* layout READ layout CONSTANT )

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

public slots:
    void setTile(int row, int column, TileType type);
    void removeTile(int row, int column);

public:
    //Getter
    WarehouseLayoutPresenter* layout() const;

private:
    WarehouseLayoutPresenter*  m_layout;
    QVector<QVector<TileType>> m_tileTypeTable;
};

#endif /* WAREHOUSE_EDITOR_PRESENTER__H */
