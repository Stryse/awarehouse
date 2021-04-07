#ifndef AMOTOR__H
#define AMOTOR__H

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
template <typename TBody>
class AMotor
{
public:
    using Body = TBody;
    using MotorDirection = motor_util::MotorDirection;

public:
    explicit AMotor(Body &body)
        : body(body) {}

public:
    virtual void activate(const MotorDirection &motorDirection) = 0;
    const Body &getBody() const { return body; }
    Body &getBody() { return body; }

protected:
    Body &body;
};

#endif /* A_MOTOR */