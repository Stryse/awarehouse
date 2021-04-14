#include "OrderLoader.h"

std::unique_ptr<OrderModel> OrderLoader::loadOrder(const QJsonObject &orderObj)
{
    if (orderObj.contains("Category") && orderObj["Category"].isDouble())
        return std::make_unique<OrderModel>(orderObj["Category"].toInt());

    return nullptr;
}

QJsonObject OrderLoader::save(const OrderModel &order)
{
    QJsonObject orderObject;
    orderObject.insert("Category", order.getCategory());
    return orderObject;
}