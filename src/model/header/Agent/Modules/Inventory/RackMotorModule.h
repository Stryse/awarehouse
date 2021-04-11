#ifndef RACK_MOTOR__MODULE__H
#define RACK_MOTOR__MODULE__H

#include "IDepleting.h"
#include "LibConfig.h"
#include "PodHolder.h"
#include "RackMotor.h"

template <typename TBody, typename TEnergy = config::agent::DefaultEnergy>
class RackMotorModule
{
public:
    using Body = TBody;
    using Energy = TEnergy;
    using IDepleting = ::IDepleting<Energy>;
    using RackMotor = ::RackMotor<Body, Energy>;
    using PodHolder = ::PodHolder<typename Body::Environment>;

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