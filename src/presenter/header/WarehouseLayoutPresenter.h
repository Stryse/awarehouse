#ifndef WAREHOUSE_LAYOUT_PRESENTER__H
#define WAREHOUSE_LAYOUT_PRESENTER__H

#include <QJsonObject>
#include <QList>
#include <QObject>

//Presenter
#include "MapItemPresenter.h"
#include "ActorList.h"
#include "TaskList.h"
#include "ChargingStationPresenter.h"
#include "PodDockPresenter.h"

class State;

class WarehouseLayoutPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(        int rows    READ rows    WRITE    setRows    NOTIFY rowsChanged    )
    Q_PROPERTY(        int columns READ columns WRITE    setColumns NOTIFY columnsChanged )
    Q_PROPERTY( ActorList* robots  READ actors  CONSTANT                                  )
    Q_PROPERTY(  TaskList* tasks   READ tasks   CONSTANT                                  )

public:
    explicit WarehouseLayoutPresenter(const   State* state,
                                            QObject* parent = nullptr);

public:
    //Getter
    int index(int row, int col);

    int rows()    const;
    int columns() const;

    //Setter
    void setRows(int value);
    void setColumns(int colCount);

    //Entity Getter
    ActorList*                              actors();
    QList<const ChargingStationPresenter*>* chargingStations();
    QList<const PodDockPresenter*>*         podDocks();
    TaskList*                               tasks();

signals:
    void rowsChanged(int rows);
    void columnsChanged(int columns);

private:
    int m_rows;
    int m_columns;

    //Warehouse Layout Entities
    ActorList                              m_robots;
    QList<const ChargingStationPresenter*> m_chargingStations;
    QList<const PodDockPresenter*>         m_podDocks;
    TaskList                               m_tasks;
};

#endif
