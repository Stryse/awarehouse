#ifndef RIGHT_TRACK_MOTOR__H
#define RIGHT_TRACK_MOTOR__H

#include "AMotor.h"
#include "Matrix.h"
#include <stdexcept>

/*******************************************************
 * @brief An implementation of an Agent's right track motor.
 * It must be used with a LeftTrackMotor in order to function
 * properly, because the two are assymetrical so they don't
 * need to use floating point transformations, but integers.
 *
 *******************************************************/
class RightTrackMotor : public AMotor
{
public:
    using Pose = typename Body::Pose;
    using MotorDirection = AMotor::MotorDirection;
    using DirectionVector = typename Body::DirectionVector;

public:
    explicit RightTrackMotor(Body &body)
        : AMotor(body)
    {
    }

public:
    /***********************************************************************
     * @brief This motor drives asymmetrically in sync with the Left Track Motor.
     * Configuration:
     * 
     * CLOCWISE          -> Rotates the body CounterClockwise
     * COUNTER_CLOCKWISE -> Moves the body Backwards
     ***********************************************************************/
    virtual void activate(const MotorDirection &motorDirection) override
    {
        switch (motorDirection)
        {
        case MotorDirection::CLOCKWISE:
            RotateCounterClockWise(this->body.getPose());
            break;

        case MotorDirection::COUNTER_CLOCKWISE:
            MoveBackWards(this->body);
            break;

        default:
            throw std::runtime_error("Unhandled enum in RightTrackMotor::activate()");
        }
    }

private:
    static Matrix<> rotation;

    void MoveBackWards(Body &body) { body.moveBodyOutsideEnvironment(body.getPose().getOrientation().muledWithScalar(-1)); }
    void RotateCounterClockWise(Pose &pose) { rotation.transform(pose.getOrientation()); }
};

Matrix<> RightTrackMotor::rotation = Matrix<>::ROTATE_Z_90_COUNTERCLOCKWISE();
#endif /* RIGHT_TRACK_MOTOR__H */
