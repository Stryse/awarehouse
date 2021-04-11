#ifndef ENERGY_MODULE__H
#define ENERGY_MODULE__H

#include "IDepleting.h"
#include "LibConfig.h"
#include <memory>

template <typename TEnergy = config::agent::DefaultEnergy>
class EnergyModule // TODO: EventBroadcaster, TODO: BatteryEventHandler
{
public:
    using Energy = TEnergy;
    using IDepleting = ::IDepleting<Energy>;

public:
    explicit EnergyModule(std::unique_ptr<IDepleting> &&energySource)
        : energySource(std::move(energySource))
    {
    }

    // GETTER
    const IDepleting *getEnergySource() const { return energySource.get(); }
    IDepleting *getEnergySource() { return energySource.get(); }

    // SETTER
    void setEnergySource(std::unique_ptr<IDepleting> &&energySource) { this->energySource = std::move(energySource); }

private:
    std::unique_ptr<IDepleting> energySource;
};

#endif /* ENERGY_MODULE__H */