#ifndef DELIVERY_ROBOT__H
#define DELIVERY_ROBOT__H

#include "Agent.h"
#include "EnergyModule.h"
#include "MovementModule.h"
#include "NetworkModule.h"
#include "PodHolderModule.h"
#include "RackMotorModule.h"

// ###################### FORWARD DECLARATIONS ######################## //
class ObservableNavEnvironment;
class DRobotMCU;
class RobotMoveMechanism;
class Battery;
// #################################################################### //
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
    explicit DeliveryRobot(const std::shared_ptr<ObservableNavEnvironment> &env,
                           const Point &position,
                           const DirectionVector &orientation,
                           int maxEnergy = 100);

    explicit DeliveryRobot(const DeliveryRobot &other) = delete;
    explicit DeliveryRobot(DeliveryRobot &&other) = delete;
    DeliveryRobot &operator=(const DeliveryRobot &other) = delete;

    virtual ~DeliveryRobot();

private:
    // ########################## Static functions ###########################
    /************************************************************************
     * @brief Returns a new Robot body (Bodyshape is only origin)
     ************************************************************************/
    static std::shared_ptr<Body> getNewRobotBody(const Point &position,
                                                 const DirectionVector &orientation,
                                                 const std::shared_ptr<ObservableNavEnvironment> &environment);

    /************************************************************************
     * @brief Returns a new Robot Micro Controller Unit
     ************************************************************************/
    static std::unique_ptr<DRobotMCU> getNewRobotMCU(IMoveMechanism &moveMechanism,
                                                     NetworkAdapter &networkAdapter,
                                                     RackMotor &rackMotor,
                                                     PodHolder &podHolder,
                                                     IDepleting &energySource,
                                                     ObservableNavEnvironment &env);

    /************************************************************************
     * @brief Returns a new Robot Movement Mechanism
     ************************************************************************/
    static std::unique_ptr<RobotMoveMechanism> getNewRobotMovement(
        const std::shared_ptr<Body> &body, IDepleting &resource);
};

#endif /* DELIVERY_ROBOT__H */