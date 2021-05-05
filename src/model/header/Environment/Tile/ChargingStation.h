#ifndef CHARGING_STATION__H
#define CHARGING_STATION__H

#include "IDepleting.h"
#include "Tile.h"
#include <algorithm>
#include <stdexcept>

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
        : Tile(pos), chargeRate(chargeRate), claimed(false) {}

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

    virtual void receive(const ClaimChStationSignal&) override
    { 
        if(claimed)
            throw std::runtime_error("Charging station is already claimed");

        claimed = true;
    }

    virtual void receive(const UnClaimChStationSignal&) override
    {
        if(!claimed)
            throw std::runtime_error("Charging station is not claimed");

        claimed = false;
    }

    // ######################################### Getter|Setter ##########################################
    int getChargeRate() const { return chargeRate; }
    void setChargeRate(int chargeRate) { this->chargeRate = chargeRate; }
    bool getClaimed() const { return claimed; }
    // ############################################################################################### //

private:
    /****************************************************************************************************
     * @brief The amount of energy that the station is capable of charging
     * in one chargeSignal.
     ****************************************************************************************************/
    int chargeRate;
    bool claimed;
};

#endif /* CHARGING_STATION__H */
