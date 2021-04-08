#ifndef LEFT_TRACK_MOTOR__H
#define LEFT_TRACK_MOTOR__H

#include "AMotor.h"
#include "Matrix.h"


/*******************************************************
 * @brief An implementation of an Agent's left track motor.
 * It must be used with a RightTrackMotor in order to function
 * properly, because the two are assymetrical so they don't
 * need to use floating point transformations, but integers.
 *
 *******************************************************/
template <typename TBody>
class LeftTrackMotor : public AMotor<TBody>
{
public:
    using Body = TBody;
    using Pose = typename Body::Pose;
    using AMotor = ::AMotor<Body>;
    using MotorDirection = typename AMotor::MotorDirection;
    using DirectionVector = typename Body::DirectionVector;

public:
    explicit LeftTrackMotor(Body &body)
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
            MoveForward(pose);
            RotateClockWise(pose);
            break;

        case MotorDirection::COUNTER_CLOCKWISE:
            break;

        default:
            throw std::runtime_error("Unhandled enum in LeftTrackmotor::activate()");
        }
    }

private:
    static Matrix<> rotation;

    void MoveForward(Pose &pose) { pose.getPosition().move(pose.getOrientation()); }
    void RotateClockWise(Pose &pose) { rotation.transform(pose.getOrientation()); }
};

template <typename TBody>
Matrix<> LeftTrackMotor<TBody>::rotation = Matrix<>::ROTATE_Z_90_CLOCKWISE();
#endif /* LEFT_TRACK_MOTOR__H */
