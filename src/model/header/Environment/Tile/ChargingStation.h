#ifndef CHARGING_STATION__H
#define CHARGING_STATION__H

#include "IDepleting.h"
#include "LibConfig.h"
#include "Tile.h"
#include <algorithm>

/************************************************************
 * @brief A Tile which can charge Depleting energy resources.
 *
 ***********************************************************/
template <typename TEnergy = config::agent::DefaultEnergy>
class ChargingStation : public Tile
{
public:
    using Energy = TEnergy;
    using Point = Tile::Point;
    using OccupantType = Tile::OccupantType;

public:
    ChargingStation(const Point &pos, const Energy &chargeRate)
        : Tile(pos), chargeRate(chargeRate) {}

    virtual ~ChargingStation() {}

public:
    // ############################ IAgentSignalHandler Implementation #################################
    /***************************************************************************************************
     * @brief Charges the resource with the bottleneck chargerate.
     ***************************************************************************************************/
    virtual void receive(IDepleting<Energy> &resource, const ChargeSignal &) const override
    {
        Energy maxCharge = std::min<Energy>(chargeRate, resource.getMaxChargeRate());
        resource.charge(maxCharge);
    }

    // ######################################### Getter|Setter ##########################################
    const Energy &getChargeRate() const { return chargeRate; }
    void setChargeRate(const Energy &chargeRate) { this->chargeRate = chargeRate; }
    // ############################################################################################### //

private:
    /****************************************************************************************************
     * @brief The amount of energy that the station is capable of charging
     * in one chargeSignal.
     ****************************************************************************************************/
    Energy chargeRate;
};

#endif /* CHARGING_STATION__H */
