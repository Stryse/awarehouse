#ifndef ROBOT_MOVE_MECHANISM__H
#define ROBOT_MOVE_MECHANISM__H

#include "Body.h"
#include "DirectionVector.h"
#include "IDepleting.h"
#include "IMoveMechanism.h"
#include "LibConfig.h"

template <typename TEnvironment = config::navigation::DefaultEnvironment,
          typename TEnergy = config::agent::DefaultEnergy>

class RobotMoveMechanism : public IMoveMechanism<TEnvironment, TEnergy>
{
public:
    using Environment = TEnvironment;
    using Body = Body<Environment>;
    using Energy = TEnergy;
    using IDepleting = IDepleting<Energy>;
    using DirectionVector = typename Body::DirectionVector;

public:
    RobotMoveMechanism(Body &body, IDepleting &resource, const Energy &turnCost, const Energy &moveCost)
        : IMoveMechanism<TEnvironment, TEnergy>(body),
          resource(resource), turnCost(turnCost), moveCost(moveCost) {}

public:
    // ################### IMoveMechanism Interface implementation #############################

    /**************************************************************
     * @brief Moves physically the robot's body if the
     * provided direction is valid
     **************************************************************/
    virtual void move(const DirectionVector &direction) override
    {
        if (isValidMoveDirection(direction))
        {
            this->body.moveBody(direction);
            resource.deplete(moveCost);
        }
    }

    /****************************************************************
     * @brief Valid direction is when the robot is going forward
     ****************************************************************/
    virtual bool isValidMoveDirection(const DirectionVector &direction) const override
    {
        return this->body.getPose().getOrientation() == direction;
    }

    /*****************************************************************
     * @brief Rotates the robot to a valid orientation.
     * 90 degrees rotations are accepted only.
     *****************************************************************/
    virtual void rotate(const DirectionVector &targetOrientation) override
    {
        if (isValidRotation(targetOrientation))
        {
            this->body.rotate(targetOrientation);
            resource.deplete(turnCost);
        }
    }

    /************************************************************************************
     * @brief Valid rotations of a robot are 90 degree rotations
     ************************************************************************************/
    virtual bool isValidRotation(const DirectionVector &targetOrientation) const override
    {
        return DirectionVector::dot(targetOrientation, this->body.getPose().getOrientation()) == 0;
    }

    /*************************************************************************************
     * @brief Returns the sum of energy cost of moving to an adjacent tile,
     * including rotation and move costs.
     *************************************************************************************/
    virtual Energy getMovementCost(const DirectionVector &direction) const override
    {
        return turnCost * (-DirectionVector::dot(direction, this->body.getPose().getOrientation()) + 1) + moveCost;
    }

    /**************************************************************************************
     * @brief Returns a sequence of actions (function calls) that is needed
     * to reach an adjacent Tile. (Move and rotate)
     **************************************************************************************/
    virtual std::vector<std::function<void(const DirectionVector &)>> getMoveActionSeq(const DirectionVector &direction) const override
    {
        std::vector<std::function<void(const DirectionVector &)>> actions(3);
        return actions;
    }

private:
    IDepleting &resource;
    Energy turnCost;
    Energy moveCost;
};

#endif /* ROBOT_MOVE_MECHANISM__H */