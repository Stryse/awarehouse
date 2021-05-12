#ifndef RACK_MOTOR__MODULE__H
#define RACK_MOTOR__MODULE__H

#include "IDepleting.h"
#include "RackMotor.h"

// #################### FORWARD DECLARATIONS ######################
class PodHolder;
class Body;
// ################################################################

/*******************************************************
 * @brief A module which contains a rack motor.
 * An agent can derive from this class so it can use
 * and forward a rack motor.
 * A rack motor is able to pick up and put down a pod
 * from and into the body's environment.
 *******************************************************/
class RackMotorModule
{
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