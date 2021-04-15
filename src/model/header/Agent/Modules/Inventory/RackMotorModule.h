#ifndef RACK_MOTOR__MODULE__H
#define RACK_MOTOR__MODULE__H

#include "IDepleting.h"
#include "RackMotor.h"

// #################### FORWARD DECLARATIONS ######################
class PodHolder;
class Body;
// ################################################################

template <typename TEnergy = config::agent::DefaultEnergy>
class RackMotorModule
{
public:
    using Energy = TEnergy;
    using IDepleting = ::IDepleting<Energy>;
    using RackMotor = ::RackMotor<Energy>;

public:
    explicit RackMotorModule(Body &body, IDepleting &energySource, PodHolder &podHolder)
        : rackMotor(body, energySource, podHolder) {}

    virtual ~RackMotorModule() = default;
    RackMotorModule(const RackMotorModule &other) = default;
    RackMotorModule(RackMotorModule &&other) = default;

    const RackMotor &getRackMotor() const { return rackMotor; }
    RackMotor &getRackMotor() { return rackMotor; }

private:
    RackMotor rackMotor;
};

#endif /* RACK_MOTOR__MODULE__H */