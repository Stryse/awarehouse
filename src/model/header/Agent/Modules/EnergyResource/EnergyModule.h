#ifndef ENERGY_MODULE__H
#define ENERGY_MODULE__H

#include "IDepleting.h"
#include <memory>

/*******************************************************
 * @brief A module which contains an energy resource.
 * An agent can derive from this class so it can use
 * and forward an energy resource.
 * All subclasses of IDepleting interface can be injected.
 *******************************************************/
class EnergyModule
{

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