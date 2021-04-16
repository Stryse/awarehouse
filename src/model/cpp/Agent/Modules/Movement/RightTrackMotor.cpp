#include "RightTrackMotor.h"

RightTrackMotor::RightTrackMotor(Body &body)
    : AMotor(body)
{
}

RightTrackMotor::RightTrackMotor(const RightTrackMotor &other)
    : AMotor(other.body)
{
}

RightTrackMotor::RightTrackMotor(RightTrackMotor &&other)
    : AMotor(std::move(other.body))
{
}

RightTrackMotor::~RightTrackMotor() {}

RightTrackMotor &RightTrackMotor::operator=(const RightTrackMotor &other)
{
    this->body = other.body;
    return *this;
}

void RightTrackMotor::activate(const RightTrackMotor::MotorDirection &motorDirection)
{
    switch (motorDirection)
    {
    case RightTrackMotor::MotorDirection::CLOCKWISE:
        RotateCounterClockWise(this->body.get().getPose());
        break;

    case RightTrackMotor::MotorDirection::COUNTER_CLOCKWISE:
        MoveBackWards(this->body.get());
        break;

    default:
        throw std::runtime_error("Unhandled enum in RightTrackMotor::activate()");
    }
}

void RightTrackMotor::MoveBackWards(Body &body)
{
    body.moveBodyOutsideEnvironment(body.getPose().getOrientation().muledWithScalar(-1));
}

void RightTrackMotor::RotateCounterClockWise(RightTrackMotor::Pose &pose)
{
    rotation.transform(pose.getOrientation());
}

Matrix<> RightTrackMotor::rotation = Matrix<>::ROTATE_Z_90_COUNTERCLOCKWISE();