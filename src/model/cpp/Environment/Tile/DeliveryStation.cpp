#include "DeliveryStation.h"
#include "OrderModel.h"
#include "Pod.h"
#include <algorithm>

DeliveryStation::DeliveryStation(const DeliveryStation::Point &pos, int acceptedOrderId)
    : Tile(pos), acceptedOrderId(acceptedOrderId)
{
}

DeliveryStation::~DeliveryStation() {}

void DeliveryStation::receive(std::set<std::unique_ptr<OrderModel>, pointer_element_comparator<OrderModel>> &orderInventory,
                              const PutDownOrderSignal &putdownOrderSignal)
{
    auto order = std::find_if(orderInventory.begin(), orderInventory.end(), [=](const std::unique_ptr<OrderModel> &order) {
        return order->getCategory() == getAcceptedOrderID();
    });

    if (order == orderInventory.end())
        return;

    orderInventory.erase(order);
}

int DeliveryStation::getAcceptedOrderID() const
{
    return acceptedOrderId;
}