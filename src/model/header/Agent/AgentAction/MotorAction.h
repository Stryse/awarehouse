#ifndef MOTOR_ACTION__H
#define MOTOR_ACTION__H

#include "AMotor.h"
#include "DepletingAction.h"
#include "LibConfig.h"
#include "MotorDrive.h"
#include <memory>
#include <vector>

// ############################ Forward Declarations ###############################
// #################################################################################

template <typename TBody, typename TEnergy = config::agent::DefaultEnergy>
class MotorAction : public DepletingAction<TEnergy>
{
public:
    //Body related
    using Body = TBody;
    using Motor = AMotor<Body>;

    //Energy related
    using Energy = TEnergy;
    using IDepleting = IDepleting<Energy>;
    using MotorDrive = MotorDrive<Motor, Energy>;

public:
    MotorAction(std::unique_ptr<MotorDrive> motorDrive, IDepleting &resource)
        : DepletingAction<Energy>(resource, motorDrive->getEnergySum(), motorDrive->getTimeSum()),
          motorDrive(std::move(motorDrive)) {}

protected:
    virtual void depletingAction() override { motorDrive->executeMovement(); }
    virtual bool canExecute() const override { return true; }

private:
    std::unique_ptr<MotorDrive> motorDrive;
};

#endif