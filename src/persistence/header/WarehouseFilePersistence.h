#ifndef WAREHOUSE_FILE_PERSISTENCE__H
#define WAREHOUSE_FILE_PERSISTENCE__H

#include "IWarehousePersistence.h"
#include "Settings.h"
#include "State.h"
#include <QJsonObject>
#include <QString>
#include <memory>
#include <vector>

class WarehouseFilePersistence : public IWarehousePersistence<QString>
{
public:
    WarehouseFilePersistence();
    virtual ~WarehouseFilePersistence();

public:
    virtual State *load(const QString &resource, const Settings * = nullptr) override;
    virtual bool save(const State &state, const QString &resource) const override;

private:
    State *loadFromJsonObject(QJsonObject json, const Settings * = nullptr);

    void loadChargingStation(std::vector<std::shared_ptr<ChargingStation>> &chStations,
                             std::shared_ptr<ObservableNavEnvironment> &env,
                             QJsonObject &warehouseLayoutData);

    void loadDeliveryStation(std::vector<std::shared_ptr<DeliveryStation>> &deliveryStations,
                             std::shared_ptr<ObservableNavEnvironment> &env,
                             QJsonObject &warehouseLayoutData);

    void loadPodDock(std::vector<std::shared_ptr<PodDock>> &podDocks,
                     std::vector<Pod<OrderModel> *> &pods,
                     std::shared_ptr<ObservableNavEnvironment> &env,
                     QJsonObject &warehouseLayoutData);

    void loadRobots(std::vector<std::shared_ptr<DeliveryRobot>> &robots,
                    std::shared_ptr<ObservableNavEnvironment> &env,
                    QJsonObject &warehouseLayoutData,
                    const Settings * = nullptr);
};

#endif /* WAREHOUSE_FILE_PERSISTENCE__H */
