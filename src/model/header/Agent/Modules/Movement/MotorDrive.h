#ifndef MOTOR_DRIVE__H
#define MOTOR_DRIVE__H

#include "LibConfig.h"
#include "MotorCommand.h"
#include <vector>

template <typename TMotor, typename TEnergy = config::agent::DefaultEnergy>
class MotorDrive
{
public:
    using Motor = TMotor;
    using MotorCommand = MotorCommand<Motor>;
    using Energy = TEnergy;

public:
    MotorDrive(const Energy &energySum, int timeSum, std::vector<MotorCommand> &&motorCommands)
        : motorCommands(std::move(motorCommands)), energySum(energySum), timeSum(timeSum)
    {
    }

public:
    void executeMovement()
    {
        for (MotorCommand &mc : motorCommands)
            mc.motor.activate(mc.motorDirection);
    }

    const Energy &getEnergySum() const { return energySum; }
    const int getTimeSum() const { return timeSum; }

private:
    std::vector<MotorCommand> motorCommands;
    Energy energySum;
    int timeSum;
};

#endif /* MOTOR_DRIVE__H */