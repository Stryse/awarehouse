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
template <typename TBody>
class RightTrackMotor : public AMotor<TBody>
{
public:
    using Body = TBody;
    using Pose = typename Body::Pose;
    using AMotor = ::AMotor<Body>;
    using MotorDirection = typename AMotor::MotorDirection;
    using DirectionVector = typename Body::DirectionVector;

public:
    explicit RightTrackMotor(Body &body)
        : AMotor(body)
    {
    }

public:
    virtual void activate(const MotorDirection &motorDirection) override
    {
        Pose &pose = this->body.getPose();
        switch (motorDirection)
        {
        case MotorDirection::CLOCKWISE:
            RotateCounterClockWise(pose);
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

template <typename TBody>
Matrix<> RightTrackMotor<TBody>::rotation = Matrix<>::ROTATE_Z_90_COUNTERCLOCKWISE();
#endif /* RIGHT_TRACK_MOTOR__H */
