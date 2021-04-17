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

class SimulationWindowPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY( WarehouseLayoutPresenter* layout   READ layout CONSTANT               )
    Q_PROPERTY( bool                      paused   READ paused NOTIFY   pausedChanged )
    Q_PROPERTY( QStringList               maps     READ maps   NOTIFY   mapsChanged   )
    Q_PROPERTY( QString                   filePath READ filePath CONSTANT             )
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
    WarehouseLayoutPresenter* layout() const;
    bool                      paused() const;
    QStringList               maps()   const;
    QString                   filePath() const;
    QString                   defaultMapPath() const;

private:
    void setPaused(bool paused);

signals:
    void pausedChanged();
    void mapsChanged();

public slots:
    void simulationStart();
    void simulationStop();

    void setTickRate(TickRate tickRate);

    void loadWarehouse(const QString& filePath);
    void reloadWarehouse();

private:
    WarehouseManager          m_manager;
    WarehouseLayoutPresenter* m_layout;
    QString                   m_loadedWarehousePath;
    QStringListModel          m_maps;

    QString m_filePath;
    QString m_defaultMapPath;
    bool m_paused;

    void createMapDir();
};

#endif /* SIMULATION_WINDOW_PRESENTER__H */
