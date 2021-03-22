#ifndef CHARGING_STATION__H
#define CHARGING_STATION__H

#include "IDepleting.h"
#include "LibConfig.h"
#include "Tile.h"
#include <algorithm>

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
    virtual void receive(IDepleting<config::agent::DefaultEnergy> &resource, const ChargeSignal &chargeSignal) const
    {
        doCharge(resource);
    }

    // Getter
    const Energy &getChargeRate() const { return chargeRate; }
    void setChargeRate(const Energy &chargeRate) { this->chargeRate = chargeRate; }

private:
    void doCharge(IDepleting<Energy> &resource) const
    {
        Energy maxCharge = std::min<Energy>(chargeRate, resource.getMaxChargeRate());
        resource.charge(maxCharge);
    }

private:
    Energy chargeRate;
};

#endif /* CHARGING_STATION__H */