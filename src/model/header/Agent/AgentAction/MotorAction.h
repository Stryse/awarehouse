#ifndef MOTOR_ACTION__H
#define MOTOR_ACTION__H

#include "AMotor.h"
#include "DepletingAction.h"
#include "LibConfig.h"
#include "MotorDrive.h"
#include "boost/signals2.hpp"
#include <memory>
#include <vector>

// ############################ Forward Declarations ###############################
// #################################################################################
/***********************************************************************************
 * @brief A kind of Action which includes the use of motors.
 * This is an energy depleting action so it needs an energy resource in order
 * to be performed.
 * It also needs a MotorDrive which is a set of motors configured to run in a
 * provided direction with a sum of energy and time cost.
 *
 **********************************************************************************/
template <typename TBody, typename TEnergy = config::agent::DefaultEnergy>
class MotorAction : public DepletingAction<TEnergy>
{
public:
    //Body related
    using Body = TBody;
    using Motor = AMotor<Body>;

    //Energy related
    using Energy = TEnergy;
    using IDepleting = ::IDepleting<Energy>;
    using MotorDrive = ::MotorDrive<Motor, Energy>;

public:
    MotorAction(std::unique_ptr<MotorDrive> &&motorDrive, IDepleting &resource,
                const boost::signals2::signal<void(const Body &)> *event = nullptr)

        : DepletingAction<Energy>(resource, motorDrive->getEnergySum(), motorDrive->getTimeSum()),
          motorDrive(std::move(motorDrive)), event_(event)
    {
    }

protected:
    virtual void depletingAction() override
    {
        motorDrive->executeMovement();

        if (event_)
            (*event_)(motorDrive->getBody());
    }
    
    virtual bool canExecute() const override { return true; }

private:
    std::unique_ptr<MotorDrive> motorDrive;
    const boost::signals2::signal<void(const Body &)> *event_;
};

#endif
