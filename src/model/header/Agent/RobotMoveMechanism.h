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
    using Energy = TEnergy;
    using IDepleting = IDepleting<Energy>;
    using Body = Body<Environment>;
    using DirectionVector = typename Body::DirectionVector;

public:
    RobotMoveMechanism(IDepleting &resource, const Energy &turnCost, const Energy &moveCost)
        : resource(resource), turnCost(turnCost), moveCost(moveCost) {}

public:
    // ################### IMoveMechanism Interface implementation #############################
    virtual void move(Body &body, const DirectionVector &direction) override
    {
        if (isValidMoveDirection(body, direction))
        {
            body.moveBody(direction);
            resource.deplete(moveCost);
        }
    }

    virtual bool isValidMoveDirection(Body &body, const DirectionVector &direction) const override
    {
        return body.getPose().getOrientation() == direction;
    }

    virtual void rotate(Body &body, const DirectionVector &targetOrientation) override
    {
        if (isValidRotation(body, targetOrientation))
        {
            body.rotate(targetOrientation);
            resource.deplete(turnCost);
        }
    }

    virtual bool isValidRotation(Body &body, const DirectionVector &targetOrientation) const override
    {
        return DirectionVector::dot(targetOrientation, body.getPose().getOrientation()) == 0;
    }

    virtual Energy getMovementCost(Body &body, const DirectionVector &direction) const override
    {
        return turnCost * (-DirectionVector::dot(direction, body.getPose().getOrientation()) + 1) + moveCost;
    }

private:
    IDepleting &resource;
    Energy turnCost;
    Energy moveCost;
};

#endif /* ROBOT_MOVE_MECHANISM__H */