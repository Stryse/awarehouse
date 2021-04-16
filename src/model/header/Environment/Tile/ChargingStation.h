#ifndef CHARGING_STATION__H
#define CHARGING_STATION__H

#include "IDepleting.h"
#include "Tile.h"
#include <algorithm>

/************************************************************
 * @brief A Tile which can charge Depleting energy resources.
 *
 ***********************************************************/
class ChargingStation : public Tile
{
public:
    using Point = Tile::Point;
    using OccupantType = Tile::OccupantType;

public:
    ChargingStation(const Point &pos, int chargeRate)
        : Tile(pos), chargeRate(chargeRate) {}

    virtual ~ChargingStation() {}

public:
    // ############################ IAgentSignalHandler Implementation #################################
    /***************************************************************************************************
     * @brief Charges the resource with the bottleneck chargerate.
     ***************************************************************************************************/
    virtual void receive(IDepleting &resource, const ChargeSignal &) const override
    {
        int maxCharge = std::min(chargeRate, resource.getMaxChargeRate());
        resource.charge(maxCharge);
    }

    // ######################################### Getter|Setter ##########################################
    int getChargeRate() const { return chargeRate; }
    void setChargeRate(int chargeRate) { this->chargeRate = chargeRate; }
    // ############################################################################################### //

private:
    /****************************************************************************************************
     * @brief The amount of energy that the station is capable of charging
     * in one chargeSignal.
     ****************************************************************************************************/
    int chargeRate;
};

#endif /* CHARGING_STATION__H */
