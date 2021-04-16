#ifndef LEFT_TRACK_MOTOR__H
#define LEFT_TRACK_MOTOR__H

#include "AMotor.h"
#include "Body.h"
#include "Matrix.h"

/*******************************************************
 * @brief An implementation of an Agent's left track motor.
 * It must be used with a RightTrackMotor in order to function
 * properly, because the two are assymetrical so they don't
 * need to use floating point transformations, but integers.
 *
 *******************************************************/
class LeftTrackMotor : public AMotor
{
public:
    using Pose = typename Body::Pose;
    using MotorDirection = typename AMotor::MotorDirection;
    using DirectionVector = typename Body::DirectionVector;

public:
    explicit LeftTrackMotor(Body &body);
    explicit LeftTrackMotor(const LeftTrackMotor &other);
    explicit LeftTrackMotor(LeftTrackMotor &&other);
    virtual ~LeftTrackMotor();
    LeftTrackMotor &operator=(const LeftTrackMotor &other);

public:
    virtual void activate(const MotorDirection &motorDirection) override;

private:
    static Matrix<> rotation;

    void MoveForward(Body &body);
    void RotateClockWise(Pose &pose);
};
#endif /* LEFT_TRACK_MOTOR__H */
