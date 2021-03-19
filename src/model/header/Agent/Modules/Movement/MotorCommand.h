#ifndef MOTOR_COMMAND__H
#define MOTOR_COMMAND__H

template <typename TMotor>
struct MotorCommand
{
    using Motor = TMotor;
    using MotorDirection = typename Motor::MotorDirection;

    MotorCommand(Motor &motor, const MotorDirection &motorDirection)
        : motor(motor), motorDirection(motorDirection) {}

    Motor &motor;
    MotorDirection motorDirection;
};

#endif /* MOTOR_COMMAND__H */