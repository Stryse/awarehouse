#ifndef TRACK_MOTOR__H
#define TRACK_MOTOR__H

#include "AMotor.h"
#include "Matrix.h"
template <typename TBody>
class TrackMotor : public AMotor<TBody>
{
public:
    using Body = TBody;
    using AMotor = AMotor<Body>;
    using MotorDirection = typename AMotor::MotorDirection;
    using DirectionVector = typename Body::DirectionVector;
    using TransformMatrix = Matrix<typename Body::DirectionVector::CoordinateT>;
    enum class MotorSide
    {
        LEFT = 0,
        RIGHT = 1
    };

public:
    explicit TrackMotor(Body &body, const MotorSide &motorSide)
        : AMotor(body),
          motorSide(motorSide)
    {
    }

public:
    virtual void activate(const MotorDirection &motorDirection) override
    {
    }

private:
    MotorSide motorSide;

private:
    static TransformMatrix MTranform;
};

// Transformation -> Rotation matrix
template <typename TBody>
typename TrackMotor<TBody>::TransformMatrix TrackMotor<TBody>::MTranform =
    typename TrackMotor<TBody>::TransformMatrix(3, 3,
                                                {0, 1, 0,
                                                 -1, 0, 0,
                                                 0, 0, 1});
#endif