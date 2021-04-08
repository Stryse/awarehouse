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
    // IAgentSignalHandler Implementation
    virtual void receive(IDepleting<config::agent::DefaultEnergy> &resource, const ChargeSignal &) const override
    {
        doCharge(resource);
    }

    // Getter|Setter
    const Energy &getChargeRate() const { return chargeRate; }
    void setChargeRate(const Energy &chargeRate) { this->chargeRate = chargeRate; }

private:
    /**************************************************************
     * @brief Charges the resource with the bottleneck chargerate.
     **************************************************************/
    void doCharge(IDepleting<Energy> &resource) const
    {
        Energy maxCharge = std::min<Energy>(chargeRate, resource.getMaxChargeRate());
        resource.charge(maxCharge);
    }

private:
    /**********************************************************************
     * @brief The amount of energy that the station is capable of charging
     * in one chargeSignal.
     **********************************************************************/
    Energy chargeRate;
};

#endif /* CHARGING_STATION__H */
