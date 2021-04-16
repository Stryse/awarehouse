#include "LeftTrackMotor.h"

LeftTrackMotor::LeftTrackMotor(Body &body)
    : AMotor(body)
{
}

LeftTrackMotor::LeftTrackMotor(const LeftTrackMotor &other)
    : AMotor(other.body)
{
}

LeftTrackMotor::LeftTrackMotor(LeftTrackMotor &&other)
    : AMotor(std::move(other.body))
{
}

LeftTrackMotor::~LeftTrackMotor() {}

LeftTrackMotor &LeftTrackMotor::operator=(const LeftTrackMotor &other)
{
    this->body = other.body;
    return *this;
}

void LeftTrackMotor::activate(const LeftTrackMotor::MotorDirection &motorDirection)
{
    Pose &pose = this->body.get().getPose();
    switch (motorDirection)
    {
    case LeftTrackMotor::MotorDirection::CLOCKWISE:
        MoveForward(this->body.get());
        RotateClockWise(pose);
        break;

    case LeftTrackMotor::MotorDirection::COUNTER_CLOCKWISE:
        break;

    default:
        throw std::runtime_error("Unhandled enum in LeftTrackmotor::activate()");
    }
}

void LeftTrackMotor::MoveForward(Body &body)
{
    body.moveBodyOutsideEnvironment(body.getPose().getOrientation());
}

void LeftTrackMotor::RotateClockWise(Pose &pose)
{
    rotation.transform(pose.getOrientation());
}

Matrix<> LeftTrackMotor::rotation = Matrix<>::ROTATE_Z_90_CLOCKWISE();