#ifndef ORDER_LOADER__H
#define ORDER_LOADER__H

#include "OrderModel.h"
#include "Pod.h"
#include "PodDock.h"
#include <QJsonObject>
#include <memory>

/****************************************************************
 * @brief Loads and saves an order from a Json Object.
 ****************************************************************/
class OrderLoader
{
public:
    static std::unique_ptr<OrderModel> loadOrder(const QJsonObject &orderObj);
    static QJsonObject save(const OrderModel &order);
};

#endif /* ORDER_LOADER__H */