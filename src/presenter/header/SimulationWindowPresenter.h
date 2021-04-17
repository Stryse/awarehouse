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
#include "Settings.h"

class SimulationWindowPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY(               QStringList warehouses           READ warehouses           NOTIFY   mapsChanged   )
    Q_PROPERTY(                   QString defaultWarehousePath READ defaultWarehousePath CONSTANT               )
    Q_PROPERTY(                   QString warehouseDirPath     READ warehouseDirPath     CONSTANT               )

    Q_PROPERTY( WarehouseLayoutPresenter* layout               READ layout               CONSTANT               )
    Q_PROPERTY(                 Settings* settings             READ settings                                    )
    Q_PROPERTY(                      bool paused               READ paused               NOTIFY   pausedChanged )

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
    QStringList               warehouses()           const;
    QString                   defaultWarehousePath() const;
    QString                   warehouseDirPath()     const;

    WarehouseLayoutPresenter* layout()               const;
    Settings*                 settings();
    bool                      paused()               const;

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

    QStringListModel m_warehouses;
    QString          m_defaultWarehousePath;
    QString          m_warehouseDirPath;

    QString                   m_currentWarehousePath;
    WarehouseLayoutPresenter* m_layout;
    Settings                  m_settings;
    bool                      m_paused;
};

#endif /* SIMULATION_WINDOW_PRESENTER__H */
