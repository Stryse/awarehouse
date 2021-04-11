#ifndef MOTOR_DRIVE__H
#define MOTOR_DRIVE__H

#include "LibConfig.h"
#include "MotorCommand.h"
#include <vector>

/****************************************************************************
 * @brief A configuration of motors and motordirections which can be executed
 * in a motorAction along with a sum of time and energy costs.
 ****************************************************************************/
template <typename TMotor, typename TEnergy = config::agent::DefaultEnergy>
class MotorDrive
{
public:
    using Motor = TMotor;
    using Body = typename Motor::Body;
    using MotorCommand = ::MotorCommand<Motor>;
    using Energy = TEnergy;

public:
    MotorDrive(Body &body, std::vector<MotorCommand> &&motorCommands, const Energy &energySum, int timeSum)
        : body(body), motorCommands(std::move(motorCommands)), energySum(energySum), timeSum(timeSum)
    {
    }

public:
    /***********************************************************
     * @brief Each motor in the command sequence is activated
     * with the provided motor direction.
     ***********************************************************/
    void executeMovement()
    {
        // Free body
        body.freeV();

        // Transform body
        for (MotorCommand &mc : motorCommands)
            mc.motor.activate(mc.motorDirection);

        // Reseat body
        body.occupyV(body.getEnvironment().getVolume(body.getPose().getPosition()));
    }

    /************************************************************
     * @brief Returns the total amount of energy required to
     * perform this MotorDrive (all motor actions)
     ************************************************************/
    const Energy &getEnergySum() const { return energySum; }

    /************************************************************
     * @brief Returns the total amount of time required to
     * perform this MotorDrive (all motor action)
     ************************************************************/
    const int getTimeSum() const { return timeSum; }

    /************************************************************
     * @brief Returns the body associated with the motorDrive
     ************************************************************/
    const Body &getBody() const { return body; }

private:
    Body &body;
    std::vector<MotorCommand> motorCommands;
    Energy energySum;
    int timeSum;
};

#endif /* MOTOR_DRIVE__H */