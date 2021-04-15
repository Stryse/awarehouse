#include "DeliveryStation.h"

DeliveryStation::DeliveryStation(const DeliveryStation::Point &pos, int acceptedOrderId)
    : Tile(pos), acceptedOrderId(acceptedOrderId)
{
}

DeliveryStation::~DeliveryStation() {}

void DeliveryStation::receive(std::unique_ptr<OrderModel> &order, const PutDownOrderSignal &putdownOrderSignal)
{
}

int DeliveryStation::getAcceptedOrderID() const
{
    return acceptedOrderId;
}