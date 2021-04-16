#ifndef MOTOR_DRIVE__H
#define MOTOR_DRIVE__H

#include "MotorCommand.h"
#include <vector>

/****************************************************************************
 * @brief A configuration of motors and motordirections which can be executed
 * in a motorAction along with a sum of time and energy costs.
 ****************************************************************************/
class MotorDrive
{
public:
    MotorDrive(Body &body, std::vector<MotorCommand> &&motorCommands, int energySum, int timeSum)
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
    int getEnergySum() const { return energySum; }

    /************************************************************
     * @brief Returns the total amount of time required to
     * perform this MotorDrive (all motor action)
     ************************************************************/
    int getTimeSum() const { return timeSum; }

    /************************************************************
     * @brief Returns the body associated with the motorDrive
     ************************************************************/
    const Body &getBody() const { return body; }

private:
    Body &body;
    std::vector<MotorCommand> motorCommands;
    int energySum;
    int timeSum;
};

#endif /* MOTOR_DRIVE__H */