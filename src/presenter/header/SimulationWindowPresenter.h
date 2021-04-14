#ifndef SIMULATION_WINDOW_PRESENTER__H
#define SIMULATION_WINDOW_PRESENTER__H

#include <QObject>
#include <QVector>

//Model
#include "ISimulator.h"
#include "WarehouseManager.h"

//Presenter
#include "WarehouseLayoutPresenter.h"
#include "ActorList.h"
#include "TaskList.h"

class SimulationWindowPresenter : public QObject
{
    Q_OBJECT

    Q_PROPERTY( TaskList*                 tasks  READ tasks  CONSTANT               )
    Q_PROPERTY( ActorList*                actors READ actors CONSTANT               )
    Q_PROPERTY( WarehouseLayoutPresenter* layout READ layout CONSTANT               )
    Q_PROPERTY( bool                      paused READ paused NOTIFY   pausedChanged )

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
    ActorList*                actors() const;
    TaskList*                 tasks()  const;
    bool                      paused() const;

private:
    void setPaused(bool paused);

signals:
    void pausedChanged();

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

    bool m_paused;
};

#endif
