#include "PodDockLoader.h"
#include "OrderLoader.h"
#include "Pod.h"
#include "Point.h"
#include <QJsonArray>

std::shared_ptr<PodDock> PodDockLoader::load(const QJsonObject &podDockObj)
{
    if (podDockObj.contains("RowCoord") && podDockObj["RowCoord"].isDouble() &&
        podDockObj.contains("ColCoord") && podDockObj["ColCoord"].isDouble())
    {
        Point<> position(podDockObj["ColCoord"].toInt(),
                         podDockObj["RowCoord"].toInt(),
                         0);

        return std::make_shared<PodDock>(position);
    }
    return nullptr;
}

void PodDockLoader::loadOrders(const QJsonObject &podDockObj,
                               Pod<OrderModel> &pod)
{
    if (podDockObj.contains("Orders") && podDockObj["Orders"].isArray())
    {
        QJsonArray ordersJson = podDockObj["Orders"].toArray();
        for (int i = 0; i < ordersJson.size(); ++i)
            pod.push(OrderLoader::loadOrder(ordersJson[i].toObject()));
    }
}

QJsonObject PodDockLoader::save(const PodDock &podDock)
{
    QJsonObject podDockObject;
    podDockObject.insert("RowCoord", podDock.getPosition().getPosY());
    podDockObject.insert("ColCoord", podDock.getPosition().getPosX());

    return podDockObject;
}
