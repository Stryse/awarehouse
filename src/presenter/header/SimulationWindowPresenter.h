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
    Q_PROPERTY(                 Settings* settings             READ settings                                    )
    Q_PROPERTY(                      bool paused               READ paused               NOTIFY   pausedChanged )

    Q_PROPERTY(     PersistencePresenter* persistence          READ persistence          CONSTANT               )

public:
    explicit SimulationWindowPresenter(QObject* parent = nullptr);

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

    PersistencePresenter*     persistence() const;

private:
    void setPaused(bool paused);

    void createMapDir();

signals:
    void pausedChanged();
    void mapsChanged();

public slots:
    void simulationStart();
    void simulationStop();

    void setTickRate(TickRate tickRate);

    void loadWarehouse(const QString& warehousePath, const Settings* settings);
    void reloadWarehouse();

private:
    WarehouseManager m_manager;

    QString                   m_currentWarehousePath;
    WarehouseLayoutPresenter* m_layout;
    Settings                  m_settings;
    bool                      m_paused;

    PersistencePresenter*     m_persistence;
};

#endif /* SIMULATION_WINDOW_PRESENTER__H */
