#ifndef ROBOT_MOVE_MECHANISM__H
#define ROBOT_MOVE_MECHANISM__H

#include "IMoveMechanism.h"
#include "LeftTrackMotor.h"
#include "LibConfig.h"
#include "MotorAction.h"
#include "MotorCommand.h"
#include "MotorDrive.h"
#include "RightTrackMotor.h"

template <typename TBody, typename TEnergy = config::agent::DefaultEnergy>
class RobotMoveMechanism : public IMoveMechanism<TBody, TEnergy>
{
public:
    // ########################## Body Related ###############################
    using Body = TBody;
    using DirectionVector = typename Body::DirectionVector;
    using AMotor = AMotor<Body>;
    using LeftTrackMotor = LeftTrackMotor<Body>;
    using RightTrackMotor = RightTrackMotor<Body>;
    using MotorDirection = typename AMotor::MotorDirection;

    // ######################### Energy related ##############################
    using Energy = TEnergy;
    using IDepleting = IDepleting<Energy>;
    using MotorAction = MotorAction<Body, Energy>;
    using MotorDrive = MotorDrive<AMotor, Energy>;
    using MotorCommand = MotorCommand<AMotor>;
    // #######################################################################

public:
    explicit RobotMoveMechanism(Body &body, IDepleting &resource)
        : IMoveMechanism<TBody, TEnergy>(getNewRobotMotors(body), getNewRobotMoveSet(), resource),

          ForwardMotorAction(std::make_unique<MotorAction>(forwardMotorDrive(this->motors), resource)),
          LeftTurnMotorAction(std::make_unique<MotorAction>(leftTurnMotorDrive(this->motors), resource)),
          RightTurnMotorAction(std::make_unique<MotorAction>(leftTurnMotorDrive(this->motors), resource))
    {
    }

public:
    virtual bool canMove(const DirectionVector &direction) const override
    {
        return true;
    }
    virtual std::queue<MotorAction *> move(const DirectionVector &direction) override
    {
        return std::queue<MotorAction *>{{ForwardMotorAction.get()}};
    }
    virtual Energy getEnergyCost(const DirectionVector &direction) const override
    {
        return 0;
    }
    virtual int getTimeCost(const DirectionVector &direction) const override
    {
        return 0;
    }

private:
    std::unique_ptr<MotorAction> ForwardMotorAction;
    std::unique_ptr<MotorAction> LeftTurnMotorAction;
    std::unique_ptr<MotorAction> RightTurnMotorAction;

public:
    /*************************************************************
     * @brief Constructs the motors of this MoveMechanism
     * This mechanism has 2 track motors, a left and a right.
     *************************************************************/
    static std::vector<std::unique_ptr<AMotor>> getNewRobotMotors(Body &body)
    {
        std::vector<std::unique_ptr<AMotor>> motors;
        motors.reserve(2);
        motors.push_back(std::make_unique<LeftTrackMotor>(body));
        motors.push_back(std::make_unique<RightTrackMotor>(body));
        return motors;
    }

    static std::set<DirectionVector> getNewRobotMoveSet()
    {
        return std::set<DirectionVector>{{DirectionVector::UP(),
                                          DirectionVector::DOWN(),
                                          DirectionVector::LEFT(),
                                          DirectionVector::RIGHT()}};
    }

    /****************************************************************
     * @brief Returns a MotorDrive which describes Forward Movement
     * Both left and the right motor moves clockwise.
     * This object can be passed to a MotorAction for execution
     ****************************************************************/
    static std::unique_ptr<MotorDrive> forwardMotorDrive(const std::vector<std::unique_ptr<AMotor>> &motors)
    {
        std::vector<MotorCommand> commands;
        commands.reserve(2);
        commands.emplace_back(*motors[0], MotorDirection::CLOCKWISE);
        commands.emplace_back(*motors[1], MotorDirection::CLOCKWISE);
        return std::move(std::make_unique<MotorDrive>(1, 1, std::move(commands)));
    }

    /****************************************************************
     * @brief Returns a MotorDrive which describes Right Turn Movement
     * The left Track motor moves CLOCKWISE
     * The right Track motor moves COUNTER_CLOCKWISE
     * This object can be passed to a MotorAction for execution
     ****************************************************************/
    static std::unique_ptr<MotorDrive> rightTurnMotorDrive(const std::vector<std::unique_ptr<AMotor>> &motors)
    {
        std::vector<MotorCommand> commands;
        commands.reserve(2);
        commands.emplace_back(*motors[1], MotorDirection::COUNTER_CLOCKWISE);
        commands.emplace_back(*motors[0], MotorDirection::CLOCKWISE);
        return std::move(std::make_unique<MotorDrive>(1, 1, std::move(commands)));
    }

    /****************************************************************
     * @brief Returns a MotorDrive which describes Left Turn Movement
     * The left Track motor moves COUNTER_CLOCKWISE
     * The right Track motor moves CLOCKWISE
     * This object can be passed to a MotorAction for execution
     ****************************************************************/
    static std::unique_ptr<MotorDrive> leftTurnMotorDrive(const std::vector<std::unique_ptr<AMotor>> &motors)
    {
        std::vector<MotorCommand> commands;
        commands.reserve(2);
        commands.emplace_back(*motors[0], MotorDirection::COUNTER_CLOCKWISE);
        commands.emplace_back(*motors[1], MotorDirection::CLOCKWISE);
        return std::move(std::make_unique<MotorDrive>(1, 1, std::move(commands)));
    }
};

#endif /* ROBOT_MOVE_MECHANISM__H */