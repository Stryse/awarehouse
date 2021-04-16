#ifndef RIGHT_TRACK_MOTOR__H
#define RIGHT_TRACK_MOTOR__H

#include "AMotor.h"
#include "Body.h"
#include "Matrix.h"

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
    explicit RightTrackMotor(Body &body);
    explicit RightTrackMotor(const RightTrackMotor &other);
    explicit RightTrackMotor(RightTrackMotor &&other);
    virtual ~RightTrackMotor();
    RightTrackMotor &operator=(const RightTrackMotor &other);

public:
    /***********************************************************************
     * @brief This motor drives asymmetrically in sync with the Left Track Motor.
     * Configuration:
     * 
     * CLOCWISE          -> Rotates the body CounterClockwise
     * COUNTER_CLOCKWISE -> Moves the body Backwards
     ***********************************************************************/
    virtual void activate(const MotorDirection &motorDirection);

private:
    static Matrix<> rotation;

    void MoveBackWards(Body &body);
    void RotateCounterClockWise(Pose &pose);
};
#endif /* RIGHT_TRACK_MOTOR__H */
