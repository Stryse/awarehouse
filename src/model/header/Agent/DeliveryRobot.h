#ifndef DELIVERY_ROBOT__H
#define DELIVERY_ROBOT__H

#include "Agent.h"
#include "Battery.h"
#include "Body.h"
#include "BodyShapeFactory.h"
#include "DRobotMCU.h"
#include "LibConfig.h"
#include "RobotMoveMechanism.h"

// ######################## Forward Declarations #########################
class DRobotMCU;
template <typename Env, typename Energy>
class IMoveMechanism;
template <typename Energy>
class IDepleting;
// #######################################################################

template <typename TEnvironment = config::navigation::DefaultEnvironment,
          typename TEnergy = config::agent::DefaultEnergy>
class DeliveryRobot : public Agent<TEnvironment>
{
public:
    using Environment = TEnvironment;
    using Energy = TEnergy;
    using IDepleting = IDepleting<Energy>;
    using Point = typename Body<Environment>::Point;
    using DirectionVector = typename Body<Environment>::DirectionVector;

public:
    explicit DeliveryRobot(Environment &env, const Point &position, const DirectionVector &orientation)
        // Init Agent BaseClass
        : Agent<Environment>("DELIVERY_ROBOT",
                             env,
                             getNewRobotBody(position, orientation, env),
                             getNewRobotMCU()),
          // Init Members
          battery(Battery<Energy>(100)),
          robotMovement(getNewRobotMovement(battery, 1, 1))
    {
    }

public:
    const Battery<Energy> &getBattery() const
    {
        return battery;
    }

    const IMoveMechanism<Environment, Energy> &getMoveMechanism() const
    {
        return *(robotMovement->get());
    }

    /*Todo Remove*/
    void move(const DirectionVector &direction)
    {
        robotMovement->move(*(this->body), direction);
    }

private:
    Battery<Energy> battery;
    std::unique_ptr<IMoveMechanism<Environment, Energy>> robotMovement;

private:
    // ########################## Static functions ###########################
    /************************************************************************
     * @brief Returns a new Robot body (Bodyshape is only origin)
     ************************************************************************/
    static std::unique_ptr<Body<Environment>> getNewRobotBody(const Point &position,
                                                              const DirectionVector &orientation,
                                                              Environment &environment)
    {
        return std::move(std::make_unique<Body<Environment>>(position,
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
    static std::unique_ptr<RobotMoveMechanism<Environment, Energy>> getNewRobotMovement(
        IDepleting &resource, const Energy &turnCost, const Energy &moveCost)
    {
        return std::move(std::make_unique<RobotMoveMechanism<Environment, Energy>>(resource, turnCost, moveCost));
    }
};

#endif /* DELIVERY_ROBOT__H */