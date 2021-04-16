#ifndef DELIVERY_ROBOT__H
#define DELIVERY_ROBOT__H

#include "Agent.h"
#include "Battery.h"
#include "Body.h"
#include "BodyShapeFactory.h"
#include "DRobotMCU.h"
#include "EnergyModule.h"
#include "MovementModule.h"
#include "NetworkModule.h"
#include "PodHolder.h"
#include "PodHolderModule.h"
#include "RackMotorModule.h"
#include "RobotMoveMechanism.h"

class ObservableNavEnvironment;

class DeliveryRobot : public EnergyModule,
                      public MovementModule,
                      public NetworkModule,
                      public PodHolderModule,
                      public RackMotorModule,
                      public Agent
{
public:
    using Point = typename Body::Point;
    using DirectionVector = typename Body::DirectionVector;

public:
    // TODO: MIXIN TMP -vel megoldani hogy szép legyen
    explicit DeliveryRobot(const std::shared_ptr<ObservableNavEnvironment> &env, const Point &position, const DirectionVector &orientation)
        : EnergyModule(std::make_unique<Battery>(100)),

          MovementModule(getNewRobotMovement(getNewRobotBody(position, orientation, env),
                                             *EnergyModule::getEnergySource())),

          RackMotorModule(*(*MovementModule::getMoveMechanism()).getBody(),
                          *EnergyModule::getEnergySource(),
                          PodHolderModule::getPodHolder()),

          Agent("DELIVERY_ROBOT",
                env,
                MovementModule::getMoveMechanism()->getBody(),
                getNewRobotMCU(*MovementModule::getMoveMechanism(),
                               NetworkModule::getNetworkAdapter(),
                               RackMotorModule::getRackMotor(),
                               PodHolderModule::getPodHolder(),
                               *EnergyModule::getEnergySource(),
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
                                                 const std::shared_ptr<ObservableNavEnvironment> &environment)
    {
        return std::make_shared<Body>(position,
                                      orientation,
                                      environment,
                                      BodyShapeFactory<Point>::onlyOrigin());
    }

    /************************************************************************
     * @brief Returns a new Robot Micro Controller Unit
     ************************************************************************/
    static std::unique_ptr<DRobotMCU> getNewRobotMCU(IMoveMechanism &moveMechanism,
                                                     NetworkAdapter &networkAdapter,
                                                     RackMotor &rackMotor,
                                                     PodHolder &podHolder,
                                                     IDepleting &energySource,
                                                     ObservableNavEnvironment &env)
    {
        return std::make_unique<DRobotMCU>(moveMechanism, networkAdapter, rackMotor, podHolder, energySource, env);
    }

    /************************************************************************
     * @brief Returns a new Robot Movement Mechanism
     ************************************************************************/
    static std::unique_ptr<RobotMoveMechanism> getNewRobotMovement(
        const std::shared_ptr<Body> &body, IDepleting &resource)
    {
        return std::make_unique<RobotMoveMechanism>(body, resource);
    }
};

#endif /* DELIVERY_ROBOT__H */