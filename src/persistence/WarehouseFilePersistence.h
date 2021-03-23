#ifndef WAREHOUSE_FILE_PERSISTENCE__H
#define WAREHOUSE_FILE_PERSISTENCE__H

#include "IWarehousePersistence.h"
#include <QString>
#include <QJsonObject>
#include <vector>
#include <memory>
#include "State.h"

class WarehouseFilePersistence : public QObject, public IWarehousePersistence<QString>
{
    Q_OBJECT

public:

    WarehouseFilePersistence(QObject* parent = nullptr);
    virtual ~WarehouseFilePersistence();

public:
    virtual State* load(QString &resource) override;
    virtual void save(const State &state, QString &resource) const override;

public: signals:
    void warehouseLoaded(const State* state);


private:
    State* loadFromJsonObject(QJsonObject json);
    void loadChargingStation(std::vector<std::shared_ptr<ChargingStation<>>>& chStations,
                             std::shared_ptr<ObservableNavEnvironment<>>& env,
                             QJsonObject& warehouseLayoutData);

    void loadPodDock(std::vector<std::shared_ptr<PodDock<>>>& podDocks,
                     std::shared_ptr<ObservableNavEnvironment<>>& env,
                     QJsonObject& warehouseLayoutData);
};

#endif /* WAREHOUSE_FILE_PERSISTENCE__H */
