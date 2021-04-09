#ifndef RACK_MOTOR__MODULE__H
#define RACK_MOTOR__MODULE__H

#include "IDepleting.h"
#include "LibConfig.h"
#include "RackMotor.h"

template <typename TBody, typename TEnergy = config::agent::DefaultEnergy>
class RackMotorModule
{
public:
    using Body = TBody;
    using Energy = TEnergy;
    using IDepleting = ::IDepleting<Energy>;
    using RackMotor = ::RackMotor<Body, Energy>;

public:
    explicit RackMotorModule(Body &body, IDepleting &energySource)
        : rackMotor(body, energySource) {}

    virtual ~RackMotorModule() = default;
    RackMotorModule(const RackMotorModule &other) = default;
    RackMotorModule(RackMotorModule &&other) = default;

    const RackMotor &getRackMotor() const { return rackMotor; }
    RackMotor &getRackMotor() { return rackMotor; }

private:
    RackMotor rackMotor;
};

#endif /* RACK_MOTOR__MODULE__H */