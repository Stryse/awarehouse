#ifndef ROBOT_MOVE_MECHANISM__H
#define ROBOT_MOVE_MECHANISM__H

#include "Body.h"
#include "IMoveMechanism.h"
#include "LeftTrackMotor.h"
#include "MotorAction.h"
#include "MotorCommand.h"
#include "MotorDrive.h"
#include "RightTrackMotor.h"
#include <cstddef>

/****************************************************************************
 * @brief Am implementation of a Move Mechanism which consists of two
 * assymetrical track motors.
 *
 * This move mechanism enables an agent to move on a 2D plane (ground)
 * with the following moves and costs:
 *
 * - Moving forward in the orientation direction (1 Energy 1 Time unit)
 * - Rotate 90 degrees clockwise/counterclockwise (1 Energy 1 Time unit)
 *
 ****************************************************************************/
class RobotMoveMechanism : public IMoveMechanism
{
public:
    // ########################## Body Related ###############################
    using DirectionVector = typename Body::DirectionVector;
    using MotorDirection = typename AMotor::MotorDirection;

public:
    explicit RobotMoveMechanism(const std::shared_ptr<Body> &body, IDepleting &resource)
        : IMoveMechanism(body, resource, getNewRobotMotors(*body), getNewRobotMoveSet()),

          ForwardMotorAction(std::make_unique<MotorAction>(
              forwardMotorDrive(*body, this->motors), resource, &(IMoveMechanism::onBodyMoved))),

          LeftTurnMotorAction(std::make_unique<MotorAction>(
              leftTurnMotorDrive(*body, this->motors), resource, &(IMoveMechanism::onBodyMoved))),

          RightTurnMotorAction(std::make_unique<MotorAction>(
              rightTurnMotorDrive(*body, this->motors), resource, &(IMoveMechanism::onBodyMoved)))
    {
    }

public:
    /**********************************************************************
     * @brief Returns whether a direction is reachable (by motors)
     **********************************************************************/
    virtual bool canMove(const DirectionVector &direction) const override
    {
        return this->moveSet.find(direction) != this->moveSet.end();
    }

    /******************************************************************************************
     * @brief Returns a sequence of MotorActions which lead to a provided reachable direction.
     ******************************************************************************************/
    virtual std::queue<MotorAction *> move(const DirectionVector &direction) override
    {
        std::queue<MotorAction *> motorActionQ;
        // We have to turn this much times
        int rotationTimes = (-DirectionVector::dot(direction, this->body->getPose().getOrientation()) + 1);

        // Angle of the rotation (clockwise or counterclockwise) when rotating 90 degrees
        int rotationAngle = (this->body->getPose().getOrientation().getX() * direction.getY()) -
                            (this->body->getPose().getOrientation().getY() * direction.getX());

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

    /*************************************************************************************************
     * @brief Sum of energy cost of a motorAction that leads to the provided (reachable!) direction.
     *************************************************************************************************/
    virtual int getEnergyCost(const DirectionVector &startOrientation, const DirectionVector &direction) const override
    {
        return RobotMoveMechanism::turnCost * (-DirectionVector::dot(direction, startOrientation) + 1) +
               RobotMoveMechanism::moveCost;
    }

    /*************************************************************************************************
     * @brief Sum of time cost of a motorAction that leads to the provided (reachable!) direction.
     *************************************************************************************************/
    virtual int getTimeCost(const DirectionVector &startOrientation, const DirectionVector &direction) const override
    {
        return RobotMoveMechanism::turnDuration * (-DirectionVector::dot(direction, startOrientation) + 1) +
               RobotMoveMechanism::moveDuration;
    }

private:
    std::unique_ptr<MotorAction> ForwardMotorAction;
    std::unique_ptr<MotorAction> LeftTurnMotorAction;
    std::unique_ptr<MotorAction> RightTurnMotorAction;
    static const int turnDuration;
    static const int turnCost;
    static const int moveDuration;
    static const int moveCost;

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

    /**************************************************************
     * @brief Returns a set of directions in which the
     * mechanism can move to. (Reachable directions)
     **************************************************************/
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
    static std::unique_ptr<MotorDrive> forwardMotorDrive(Body &body, const std::vector<std::unique_ptr<AMotor>> &motors)
    {
        std::vector<MotorCommand> commands;
        commands.reserve(2);
        commands.emplace_back(*motors[0], MotorDirection::CLOCKWISE);
        commands.emplace_back(*motors[1], MotorDirection::CLOCKWISE);
        return std::move(std::make_unique<MotorDrive>(body,
                                                      std::move(commands),
                                                      RobotMoveMechanism::moveCost,
                                                      RobotMoveMechanism::moveDuration));
    }

    /****************************************************************
     * @brief Returns a MotorDrive which describes Right Turn Movement
     * The left Track motor moves CLOCKWISE
     * The right Track motor moves COUNTER_CLOCKWISE
     * This object can be passed to a MotorAction for execution
     ****************************************************************/
    static std::unique_ptr<MotorDrive> rightTurnMotorDrive(Body &body, const std::vector<std::unique_ptr<AMotor>> &motors)
    {
        std::vector<MotorCommand> commands;
        commands.reserve(2);
        commands.emplace_back(*motors[1], MotorDirection::COUNTER_CLOCKWISE);
        commands.emplace_back(*motors[0], MotorDirection::CLOCKWISE);
        return std::move(std::make_unique<MotorDrive>(body,
                                                      std::move(commands),
                                                      RobotMoveMechanism::turnCost,
                                                      RobotMoveMechanism::turnDuration));
    }

    /****************************************************************
     * @brief Returns a MotorDrive which describes Left Turn Movement
     * The left Track motor moves COUNTER_CLOCKWISE
     * The right Track motor moves CLOCKWISE
     * This object can be passed to a MotorAction for execution
     ****************************************************************/
    static std::unique_ptr<MotorDrive> leftTurnMotorDrive(Body &body, const std::vector<std::unique_ptr<AMotor>> &motors)
    {
        std::vector<MotorCommand> commands;
        commands.reserve(2);
        commands.emplace_back(*motors[0], MotorDirection::COUNTER_CLOCKWISE);
        commands.emplace_back(*motors[1], MotorDirection::CLOCKWISE);
        return std::move(std::make_unique<MotorDrive>(body,
                                                      std::move(commands),
                                                      RobotMoveMechanism::turnCost,
                                                      RobotMoveMechanism::turnDuration));
    }
};

/************************* Settings ***************************/
const int RobotMoveMechanism::moveDuration = 1;
const int RobotMoveMechanism::moveCost = 1;
const int RobotMoveMechanism::turnDuration = 1;
const int RobotMoveMechanism::turnCost = 1;
/************************ Settings *****************************/

#endif /* ROBOT_MOVE_MECHANISM__H */
