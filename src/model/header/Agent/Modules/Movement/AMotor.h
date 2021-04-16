#ifndef AMOTOR__H
#define AMOTOR__H

#include <functional>

// ########################### FORWARD DECLRATIONS ######################
class Body;
// ######################################################################

namespace motor_util
{
    /***********************************************************************
     * @brief A Motor can be instructed to move in two directions.
     * The motor affects the body in different ways when driven in different
     * directions.
     ***********************************************************************/
    enum MotorDirection
    {
        CLOCKWISE = 1,
        COUNTER_CLOCKWISE = -1
    };
};

/**********************************************************************
 * @brief A motor is an entity which can transform an associated body.
 * It can be driven in two directions. (CLOCKWISE,COUNTERCLOCKWISE)
 **********************************************************************/
class AMotor
{
public:
    using MotorDirection = motor_util::MotorDirection;

public:
    explicit AMotor(Body &body)
        : body(body) {}

public:
    virtual void activate(const MotorDirection &motorDirection) = 0;
    const Body &getBody() const { return body; }
    Body &getBody() { return body; }

protected:
    std::reference_wrapper<Body> body;
};

#endif /* A_MOTOR */