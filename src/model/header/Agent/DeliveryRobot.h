#ifndef DELIVERY_ROBOT__H
#define DELIVERY_ROBOT__H

#include "Agent.h"
#include "Battery.h"
#include "Body.h"
#include "BodyShapeFactory.h"
#include "DRobotMCU.h"
#include "IDepleting.h"
#include "LibConfig.h"

// ######################## Forward Declarations #########################
class DRobotMCU;
// #######################################################################

template <typename TEnvironment = config::navigation::DefaultEnvironment,
          typename TEnergy = config::agent::DefaultEnergy>
class DeliveryRobot : public Agent<TEnvironment>
{
public:
    using Environment = TEnvironment;
    using Energy = TEnergy;
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
          battery(Battery<Energy>(100))
    {
    }

private:
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

private:
    Battery<Energy> battery;
};

#endif /* DELIVERY_ROBOT__H */