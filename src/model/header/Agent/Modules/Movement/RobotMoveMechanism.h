#ifndef ROBOT_MOVE_MECHANISM__H
#define ROBOT_MOVE_MECHANISM__H

#include "IMoveMechanism.h"
#include "LibConfig.h"
#include "MotorAction.h"
#include "MotorCommand.h"
#include "MotorDrive.h"
#include "TrackMotor.h"

template <typename TBody, typename TEnergy = config::agent::DefaultEnergy>
class RobotMoveMechanism : public IMoveMechanism<TBody, TEnergy>
{
public:
    //Body Related
    using Body = TBody;
    using DirectionVector = typename Body::DirectionVector;
    using AMotor = AMotor<Body>;
    using TrackMotor = TrackMotor<Body>;
    using MotorDirection = typename AMotor::MotorDirection;

    //Energy related
    using Energy = TEnergy;
    using IDepleting = IDepleting<Energy>;
    using MotorAction = MotorAction<Body, Energy>;
    using MotorDrive = MotorDrive<AMotor, Energy>;
    using MotorCommand = MotorCommand<AMotor>;

    explicit RobotMoveMechanism(Body &body, IDepleting &resource)
        : IMoveMechanism<TBody, TEnergy>(getNewRobotMotors(body), resource)
    {
    }

public:
    virtual bool canMove(const DirectionVector &direction) const override
    {
        return true;
    }
    virtual MotorAction *move(const DirectionVector &direction) override
    {
        MotorAction *forward = new MotorAction(forwardMotorDrive(this->motors), this->resource);
        return forward;
    }
    virtual Energy getEnergyCost(const DirectionVector &direction) const override
    {
        return 0;
    }
    virtual int getTimeCost(const DirectionVector &direction) const override
    {
        return 0;
    }

public:
    static std::vector<AMotor *> getNewRobotMotors(Body &body)
    {
        std::vector<AMotor *> motors;
        motors.reserve(2);
        motors.push_back(new TrackMotor(body, TrackMotor::MotorSide::LEFT));
        motors.push_back(new TrackMotor(body, TrackMotor::MotorSide::RIGHT));
        return motors;
    }

    static std::unique_ptr<MotorDrive> forwardMotorDrive(const std::vector<AMotor *> &motors)
    {
        std::vector<MotorCommand> commands;
        commands.reserve(2);
        commands.emplace_back(*motors[0], MotorDirection::CLOCKWISE);
        commands.emplace_back(*motors[1], MotorDirection::CLOCKWISE);
        return std::move(std::make_unique<MotorDrive>(1, 1, std::move(commands)));
    }

private:
};

#endif /* ROBOT_MOVE_MECHANISM__H */