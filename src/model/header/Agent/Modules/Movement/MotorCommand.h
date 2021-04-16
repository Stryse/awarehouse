#ifndef MOTOR_COMMAND__H
#define MOTOR_COMMAND__H

#include "AMotor.h"
/*******************************************************************
 * @brief A pair of motor and motordirection which needs to be
 * activated in a motorDrive.
 *******************************************************************/
struct MotorCommand
{
    using MotorDirection = AMotor::MotorDirection;

    MotorCommand(AMotor &motor, const MotorDirection &motorDirection)
        : motor(motor), motorDirection(motorDirection) {}

    AMotor &motor;
    MotorDirection motorDirection;
};

#endif /* MOTOR_COMMAND__H */
