#ifndef DELIVERY_ROBOT__H
#define DELIVERY_ROBOT__H

#include "Agent.h"
#include "Battery.h"
#include "Body.h"
#include "BodyShapeFactory.h"
#include "DRobotMCU.h"
#include "LibConfig.h"
#include "MotorAction.h" // TODO REMOVE
#include "RobotMoveMechanism.h"
#include <queue> // TODO REMOVE
// ######################## Forward Declarations #########################
class DRobotMCU;
template <typename TBody, typename TEnergy>
class IMoveMechanism;
template <typename TEnergy>
class IDepleting;
// #######################################################################

template <typename TEnvironment = config::navigation::DefaultEnvironment,
          typename TEnergy = config::agent::DefaultEnergy>
class DeliveryRobot : public Agent<TEnvironment>
{
public:
    using Environment = TEnvironment;
    using Body = Body<Environment>;
    using Energy = TEnergy;
    using IDepleting = IDepleting<Energy>;
    using Point = typename Body::Point;
    using DirectionVector = typename Body::DirectionVector;

public:
    explicit DeliveryRobot(const std::shared_ptr<Environment> &env, const Point &position, const DirectionVector &orientation)
        // Init Agent BaseClass
        : Agent<Environment>("DELIVERY_ROBOT",
                             env,
                             getNewRobotBody(position, orientation, env),
                             getNewRobotMCU()),
          // Init Members
          battery(Battery<Energy>(100)),
          robotMovement(getNewRobotMovement(*(this->body), battery))
    {
    }

public:
    const Battery<Energy> &getBattery() const { return battery; }
    const IMoveMechanism<Body, Energy> &getMoveMechanism() const { return *(robotMovement->get()); }

    /*Todo Remove*/
    void move(const DirectionVector &direction)
    {
        std::queue<MotorAction<Body, Energy> *> moveActions = robotMovement->move(direction);
        while (!moveActions.empty())
        {
            auto moveAction = moveActions.front();
            (*moveAction)();
            moveActions.pop();
        }
    }

private:
    Battery<Energy> battery;
    std::unique_ptr<IMoveMechanism<Body, Energy>> robotMovement;

private:
    // ########################## Static functions ###########################
    /************************************************************************
     * @brief Returns a new Robot body (Bodyshape is only origin)
     ************************************************************************/
    static std::unique_ptr<Body> getNewRobotBody(const Point &position,
                                                 const DirectionVector &orientation,
                                                 const std::shared_ptr<Environment> &environment)
    {
        return std::move(std::make_unique<Body>(position,
                                                orientation,
                                                environment,
                                                BodyShapeFactory<Point>::onlyOrigin()));
    }

    /************************************************************************
     * @brief Returns a new Robot Micro Controller Unit
     ************************************************************************/
    static std::unique_ptr<DRobotMCU> getNewRobotMCU()
    {
        return std::move(std::make_unique<DRobotMCU>());
    }

    /************************************************************************
     * @brief Returns a new Robot Movement Mechanism
     ************************************************************************/
    static std::unique_ptr<RobotMoveMechanism<Body, Energy>> getNewRobotMovement(
        Body &body, IDepleting &resource)
    {
        return std::move(std::make_unique<RobotMoveMechanism<Body, Energy>>(body, resource));
    }
};

#endif /* DELIVERY_ROBOT__H */