#ifndef DELIVERY_ROBOT__H
#define DELIVERY_ROBOT__H

#include "Agent.h"
#include "Battery.h"
#include "Body.h"
#include "BodyShapeFactory.h"
#include "DRobotMCU.h"
#include "EnergyModule.h"
#include "LibConfig.h"
#include "MovementModule.h"
#include "NetworkModule.h"
#include "PodHolder.h"
#include "PodHolderModule.h"
#include "RobotMoveMechanism.h"
// ######################## Forward Declarations #########################
template <typename TEnvironment, typename TBody, typename TEnergy>
class DRobotMCU;
// #######################################################################

template <typename TEnvironment = config::navigation::DefaultEnvironment,
          typename TEnergy = config::agent::DefaultEnergy>

class DeliveryRobot : public EnergyModule<TEnergy>,
                      public MovementModule<Body<TEnvironment>, TEnergy>,
                      public NetworkModule,
                      public PodHolderModule<TEnvironment>,
                      public Agent<TEnvironment>
{
public:
    using Environment = TEnvironment;
    using Body = ::Body<Environment>;
    using Energy = TEnergy;
    using IDepleting = ::IDepleting<Energy>;
    using Point = typename Body::Point;
    using DirectionVector = typename Body::DirectionVector;
    using DRobotMCU = ::DRobotMCU<Environment, Body, Energy>;

public:
    explicit DeliveryRobot(const std::shared_ptr<Environment> &env, const Point &position, const DirectionVector &orientation)
        : EnergyModule<Energy>(std::make_unique<Battery<Energy>>(100)),
          MovementModule<Body, Energy>(getNewRobotMovement(getNewRobotBody(position, orientation, env), *EnergyModule<Energy>::getEnergySource())),
          Agent<Environment>("DELIVERY_ROBOT",
                             env,
                             MovementModule<Body, Energy>::getMoveMechanism()->getBody(),
                             getNewRobotMCU(*MovementModule<Body, Energy>::getMoveMechanism(),
                                            NetworkModule::getNetworkAdapter(),
                                            PodHolderModule<TEnvironment>::getPodHolder()))
    {
    }

private:
    // ########################## Static functions ###########################
    /************************************************************************
     * @brief Returns a new Robot body (Bodyshape is only origin)
     ************************************************************************/
    static std::shared_ptr<Body> getNewRobotBody(const Point &position,
                                                 const DirectionVector &orientation,
                                                 const std::shared_ptr<Environment> &environment)
    {
        return std::make_shared<Body>(position,
                                      orientation,
                                      environment,
                                      BodyShapeFactory<Point>::onlyOrigin());
    }

    /************************************************************************
     * @brief Returns a new Robot Micro Controller Unit
     ************************************************************************/
    static std::unique_ptr<DRobotMCU> getNewRobotMCU(IMoveMechanism<Body, Energy> &moveMechanism,
                                                     NetworkAdapter &networkAdapter,
                                                     PodHolder<Environment> &podHolder)
    {
        return std::make_unique<DRobotMCU>(moveMechanism, networkAdapter, podHolder);
    }

    /************************************************************************
     * @brief Returns a new Robot Movement Mechanism
     ************************************************************************/
    static std::unique_ptr<RobotMoveMechanism<Body, Energy>> getNewRobotMovement(
        const std::shared_ptr<Body> &body, IDepleting &resource)
    {
        return std::make_unique<RobotMoveMechanism<Body, Energy>>(body, resource);
    }
};

#endif /* DELIVERY_ROBOT__H */