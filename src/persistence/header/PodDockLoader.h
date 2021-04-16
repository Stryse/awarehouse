#ifndef POD_DOCK_LOADER__H
#define POD_DOCK_LOADER__H

#include "OrderModel.h"
#include "Pod.h"
#include "PodDock.h"
#include <QJsonObject>
#include <memory>

class PodDockLoader
{
public:
    static std::shared_ptr<PodDock> load(const QJsonObject &podDockObj);

    static void loadOrders(const QJsonObject &podDockObj,
                           Pod<OrderModel> &pod);

    static QJsonObject save(const PodDock &podDock);
};

#endif /* POD_DOCK_LOADER__H */
