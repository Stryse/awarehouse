#ifndef RIGHT_TRACK_MOTOR__H
#define RIGHT_TRACK_MOTOR__H

#include "AMotor.h"
#include "Matrix.h"
#include <stdexcept>

template <typename TBody>
class RightTrackMotor : public AMotor<TBody>
{
public:
    using Body = TBody;
    using Pose = typename Body::Pose;
    using AMotor = AMotor<Body>;
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
            MoveBackWards(pose);
            break;

        default:
            throw std::runtime_error("Unhandled enum");
        }
    }

private:
    static Matrix<> rotation;

    void MoveForward(Pose &pose)
    {
        pose.getPosition().move(pose.getOrientation());
    }

    void RotateCounterClockWise(Pose &pose)
    {
        rotation.transform(pose.getOrientation());
    }

    void MoveBackWards(Pose &pose)
    {
        pose.getPosition().move(pose.getOrientation().muledWithScalar(-1));
    }
};

template <typename TBody>
Matrix<> RightTrackMotor<TBody>::rotation = Matrix<>::ROTATE_Z_90_COUNTERCLOCKWISE();
#endif /* RIGHT_TRACK_MOTOR__H */