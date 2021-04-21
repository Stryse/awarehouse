#ifndef DELIVERY_STATION__H
#define DELIVERY_STATION__H

#include "Tile.h"

class DeliveryStation : public Tile
{
public:
    using Point = Tile::Point;
    using OccupantType = Tile::OccupantType;

public:
    explicit DeliveryStation(const Point &pos, int acceptedOrderId);
    virtual ~DeliveryStation();

public:
    int getAcceptedOrderID() const;

public:
    /*************************************************************************************************************
     * @brief 
     *************************************************************************************************************/
    virtual void receive(std::set<std::unique_ptr<OrderModel>, pointer_element_comparator<OrderModel>> &orderInventory,
                         const PutDownOrderSignal &putdownOrderSignal) override;

private:
    int acceptedOrderId;
};

#endif /* DELIVERY_STATION__H */