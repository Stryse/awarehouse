#ifndef SIMULATION_WINDOW_PRESENTER__H
#define SIMULATION_WINDOW_PRESENTER__H

#include <QObject>
#include <QVector>
#include <QStringListModel>

//Model
#include "ISimulator.h"
#include "WarehouseManager.h"

//Presenter
#include "WarehouseLayoutPresenter.h"
#include "PersistencePresenter.h"
#include "Settings.h"

class SimulationWindowPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY( WarehouseLayoutPresenter* layout               READ layout               CONSTANT               )
    Q_PROPERTY(                 Settings* settings             READ settings             CONSTANT               )
    Q_PROPERTY(                      bool paused               READ paused               NOTIFY   pausedChanged )

    Q_PROPERTY(     PersistencePresenter* persistence          READ persistence          CONSTANT               )

    Q_PROPERTY( int                       time     READ time   NOTIFY   timeChanged   )
public:
    explicit SimulationWindowPresenter(PersistencePresenter* persistence = nullptr,
                                                    QObject* parent      = nullptr);

    enum TickRate
    {
        HALF_SPEED,
        NORMAL,
        TWICE
    };
    Q_ENUM(TickRate);

public:
    //Getter
    WarehouseLayoutPresenter* layout()      const;
    Settings*                 settings();
    bool                      paused()      const;
    int                       time() const;

    PersistencePresenter*     persistence() const;

private:
    void setPaused(bool paused);
    void setTime(int time);

    void createMapDir();

signals:
    void pausedChanged();
    void mapsChanged();
    void timeChanged();

public slots:
    void simulationStart();
    void simulationStop();

    void setTickRate(TickRate tickRate);

    int getCurrentWarehouseIndex() const;

    void loadWarehouse(const QString& warehousePath);
    void reloadWarehouse();

private:
    WarehouseManager m_manager;

    QString                   m_currentWarehouseName;
    WarehouseLayoutPresenter* m_layout;
    Settings                  m_settings;
    bool                      m_paused;

    PersistencePresenter*     m_persistence;
    int m_time;
};

#endif /* SIMULATION_WINDOW_PRESENTER__H */
