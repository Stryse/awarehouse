#ifndef TRACK_MOTOR__H
#define TRACK_MOTOR__H

#include "AMotor.h"

template <typename TBody>
class TrackMotor : public AMotor<TBody>
{
public:
    using Body = TBody;
    using AMotor = AMotor<Body>;
    using MotorDirection = typename AMotor::MotorDirection;
    enum class MotorSide
    {
        LEFT = 0,
        RIGHT = 1
    };

public:
    explicit TrackMotor(Body &body, const MotorSide &motorSide)
        : AMotor(body),
          side(motorSide)
    {
    }

public:
    virtual void activate(const MotorDirection &motorDirection) override
    {
    }

private:
    MotorSide side;
};

#endif