#ifndef WAREHOUSE_LAYOUT_PRESENTER__H
#define WAREHOUSE_LAYOUT_PRESENTER__H

#include <QJsonObject>
#include <QList>
#include <QObject>

//Presenter
#include "MapItemPresenter.h"
#include "ActorPresenter.h"
#include "ChargingStationPresenter.h"
#include "PodDockPresenter.h"
#include "TaskPresenter.h"

class State;

class WarehouseLayoutPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY( int                             rows    READ rows    WRITE    setRows    NOTIFY rowsChanged     )
    Q_PROPERTY( int                             columns READ columns WRITE    setColumns NOTIFY columnsChanged )
    Q_PROPERTY( QList<const MapItemPresenter*>* map     READ map     CONSTANT                                  )
    Q_PROPERTY( QList<const ActorPresenter*>*   robots  READ actors  CONSTANT                                  )
    Q_PROPERTY( QList<const TaskPresenter*>*    tasks   READ tasks   CONSTANT                                  )

public:
    explicit WarehouseLayoutPresenter(const State* state, QObject* parent = nullptr);

public:
    int index(int row, int col);

    int rows()    const;
    int columns() const;

    void setRows(int value);
    void setColumns(int colCount);

    //Entity Getter
    QList<const MapItemPresenter*>*         map();
    QList<const ActorPresenter*>*           actors();
    QList<const ChargingStationPresenter*>* chargingStations();
    QList<const PodDockPresenter*>*         podDocks();
    QList<const TaskPresenter*>*            tasks();

signals:
    void rowsChanged(int rows);
    void columnsChanged(int columns);

private:
    int m_rows;
    int m_columns;

    //Warehouse Layout Entities
    QList<const MapItemPresenter*>         m_map;
    QList<const ActorPresenter*>           m_robots;
    QList<const ChargingStationPresenter*> m_chargingStations;
    QList<const PodDockPresenter*>         m_podDocks;
    QList<const TaskPresenter*>            m_tasks;
};

#endif
