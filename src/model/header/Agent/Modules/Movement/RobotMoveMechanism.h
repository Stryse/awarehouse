#ifndef ROBOT_MOVE_MECHANISM__H
#define ROBOT_MOVE_MECHANISM__H

#include "IMoveMechanism.h"
#include "LeftTrackMotor.h"
#include "LibConfig.h"
#include "MotorAction.h"
#include "MotorCommand.h"
#include "MotorDrive.h"
#include "RightTrackMotor.h"
#include <cstddef>

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
        : IMoveMechanism<TBody, TEnergy>(body, getNewRobotMotors(body), getNewRobotMoveSet(), resource),

          ForwardMotorAction(std::make_unique<MotorAction>(forwardMotorDrive(this->motors), resource)),
          LeftTurnMotorAction(std::make_unique<MotorAction>(leftTurnMotorDrive(this->motors), resource)),
          RightTurnMotorAction(std::make_unique<MotorAction>(rightTurnMotorDrive(this->motors), resource))
    {
    }

public:
    virtual bool canMove(const DirectionVector &direction) const override
    {
        return this->moveSet.find(direction) != this->moveSet.end();
    }
    virtual std::queue<MotorAction *> move(const DirectionVector &direction) override
    {
        std::queue<MotorAction *> motorActionQ;
        // We have to turn this much times
        int rotationTimes = (-DirectionVector::dot(direction, this->body.getPose().getOrientation()) + 1);
        int rotationAngle = (this->body.getPose().getOrientation().getX() * direction.getY()) -
                            (this->body.getPose().getOrientation().getY() * direction.getX());

        switch (rotationTimes)
        {
        case 0: // Travel direction
            motorActionQ.emplace(ForwardMotorAction.get());
            break;
        case 2: // Backward direction
            motorActionQ.emplace(LeftTurnMotorAction.get());
            motorActionQ.emplace(LeftTurnMotorAction.get());
            motorActionQ.emplace(ForwardMotorAction.get());
            break;
        case 1:
            // Left or Right direction
            // Angle if turn is 90 degrees

            switch (rotationAngle)
            {
            case 1: //CounterClockWise
                motorActionQ.emplace(LeftTurnMotorAction.get());
                motorActionQ.emplace(ForwardMotorAction.get());
                break;

            case -1: // ClockWise
                motorActionQ.emplace(RightTurnMotorAction.get());
                motorActionQ.emplace(ForwardMotorAction.get());
                break;
            default:
                throw std::runtime_error("Invalid value in RobotMoveMechanism move()");
            }
            break;
        default:
            throw std::runtime_error("Invalid value in RobotMoveMechanism move()");
        }

        return motorActionQ;
    }
    virtual Energy getEnergyCost(const DirectionVector &direction) const override
    {
        return RobotMoveMechanism<Body, Energy>::turnCost * (-DirectionVector::dot(direction, this->body.getPose().getOrientation()) + 1) +
               RobotMoveMechanism<Body, Energy>::moveCost;
    }
    virtual int getTimeCost(const DirectionVector &direction) const override
    {
        return RobotMoveMechanism<Body, Energy>::turnDuration * (-DirectionVector::dot(direction, this->body.getPose().getOrientation()) + 1) +
               RobotMoveMechanism<Body, Energy>::moveDuration;
    }

private:
    std::unique_ptr<MotorAction> ForwardMotorAction;
    std::unique_ptr<MotorAction> LeftTurnMotorAction;
    std::unique_ptr<MotorAction> RightTurnMotorAction;
    static const int turnDuration;
    static const Energy turnCost;
    static const int moveDuration;
    static const Energy moveCost;

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
        return std::move(std::make_unique<MotorDrive>(RobotMoveMechanism<Body, Energy>::moveCost,
                                                      RobotMoveMechanism<Body, Energy>::moveDuration,
                                                      std::move(commands)));
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
        return std::move(std::make_unique<MotorDrive>(RobotMoveMechanism<Body, Energy>::turnCost,
                                                      RobotMoveMechanism<Body, Energy>::turnDuration,
                                                      std::move(commands)));
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
        return std::move(std::make_unique<MotorDrive>(RobotMoveMechanism<Body, Energy>::turnCost,
                                                      RobotMoveMechanism<Body, Energy>::turnDuration,
                                                      std::move(commands)));
    }
};

/************************* Settings ***************************/
template <typename TBody, typename TEnergy>
const int RobotMoveMechanism<TBody, TEnergy>::moveDuration = 1;

template <typename TBody, typename TEnergy>
const TEnergy RobotMoveMechanism<TBody, TEnergy>::moveCost = 1;

template <typename TBody, typename TEnergy>
const int RobotMoveMechanism<TBody, TEnergy>::turnDuration = 1;

template <typename TBody, typename TEnergy>
const TEnergy RobotMoveMechanism<TBody, TEnergy>::turnCost = 1;
/************************ Settings *****************************/

#endif /* ROBOT_MOVE_MECHANISM__H */