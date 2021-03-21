#ifndef AMOTOR__H
#define AMOTOR__H

namespace motor_util
{
    enum MotorDirection
    {
        CLOCKWISE = 1,
        COUNTER_CLOCKWISE = -1
    };
};

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