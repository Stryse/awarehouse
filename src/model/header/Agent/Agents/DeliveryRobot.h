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
#include "RackMotorModule.h"
#include "RobotMoveMechanism.h"

class ObservableNavEnvironment;
template <typename TEnvironment = ObservableNavEnvironment,
          typename TEnergy = config::agent::DefaultEnergy>

class DeliveryRobot : public EnergyModule<TEnergy>,
                      public MovementModule<TEnergy>,
                      public NetworkModule,
                      public PodHolderModule,
                      public RackMotorModule<TEnergy>,
                      public Agent<TEnvironment>
{
public:
    using Environment = TEnvironment;
    using Energy = TEnergy;
    using IDepleting = ::IDepleting<Energy>;
    using Point = typename Body::Point;
    using DirectionVector = typename Body::DirectionVector;
    using DRobotMCU = ::DRobotMCU<Environment, Body, Energy>;

public:
    // TODO: MIXIN TMP -vel megoldani hogy szép legyen
    explicit DeliveryRobot(const std::shared_ptr<Environment> &env, const Point &position, const DirectionVector &orientation)
        : EnergyModule<Energy>(std::make_unique<Battery<Energy>>(100)),

          MovementModule<Energy>(getNewRobotMovement(getNewRobotBody(position, orientation, env),
                                                     *EnergyModule<Energy>::getEnergySource())),

          RackMotorModule<Energy>(*(*MovementModule<Energy>::getMoveMechanism()).getBody(),
                                  *EnergyModule<Energy>::getEnergySource(),
                                  PodHolderModule::getPodHolder()),

          Agent<Environment>("DELIVERY_ROBOT",
                             env,
                             MovementModule<Energy>::getMoveMechanism()->getBody(),
                             getNewRobotMCU(*MovementModule<Energy>::getMoveMechanism(),
                                            NetworkModule::getNetworkAdapter(),
                                            RackMotorModule<Energy>::getRackMotor(),
                                            PodHolderModule::getPodHolder(),
                                            *EnergyModule<Energy>::getEnergySource(),
                                            *env))
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
    static std::unique_ptr<DRobotMCU> getNewRobotMCU(IMoveMechanism<Energy> &moveMechanism,
                                                     NetworkAdapter &networkAdapter,
                                                     RackMotor<Energy> &rackMotor,
                                                     PodHolder &podHolder,
                                                     IDepleting &energySource,
                                                     Environment &env)
    {
        return std::make_unique<DRobotMCU>(moveMechanism, networkAdapter, rackMotor, podHolder, energySource, env);
    }

    /************************************************************************
     * @brief Returns a new Robot Movement Mechanism
     ************************************************************************/
    static std::unique_ptr<RobotMoveMechanism<Energy>> getNewRobotMovement(
        const std::shared_ptr<Body> &body, IDepleting &resource)
    {
        return std::make_unique<RobotMoveMechanism<Energy>>(body, resource);
    }
};

#endif /* DELIVERY_ROBOT__H */