#ifndef MOTOR_ACTION__H
#define MOTOR_ACTION__H

#include "DepletingAction.h"
#include "boost/signals2.hpp"
#include <memory>
#include <vector>

// ############################ Forward Declarations ###############################
class MotorDrive;
class Body;
// #################################################################################
/***********************************************************************************
 * @brief A kind of Action which includes the use of motors.
 * This is an energy depleting action so it needs an energy resource in order
 * to be performed.
 * It also needs a MotorDrive which is a set of motors configured to run in a
 * provided direction with a sum of energy and time cost.
 *
 **********************************************************************************/
class MotorAction : public DepletingAction
{
public:
    MotorAction(std::unique_ptr<MotorDrive> &&motorDrive, IDepleting &resource,
                const boost::signals2::signal<void(const Body &)> *event = nullptr);

    MotorAction(const MotorAction &other) = delete;
    MotorAction(MotorAction &&other) = delete;
    MotorAction &operator=(const MotorAction &other) = delete;
    virtual ~MotorAction();

protected:
    virtual void depletingAction() override;
    virtual bool canExecute() const override;

private:
    std::unique_ptr<MotorDrive> motorDrive;
    const boost::signals2::signal<void(const Body &)> *event_;
};

#endif
