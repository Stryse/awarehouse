#include "DeliveryRobot.h"
#include "Battery.h"
#include "Body.h"
#include "BodyShapeFactory.h"
#include "DRobotMCU.h"
#include "PodHolder.h"
#include "RobotMoveMechanism.h"
#include <memory>

DeliveryRobot::DeliveryRobot(const std::shared_ptr<ObservableNavEnvironment> &env,
                             const DeliveryRobot::Point &position,
                             const DeliveryRobot::DirectionVector &orientation,
                             int maxEnergy)

    : EnergyModule(std::make_unique<Battery>(maxEnergy, (maxEnergy / 5))),

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

DeliveryRobot::~DeliveryRobot() {}

std::shared_ptr<Body> DeliveryRobot::getNewRobotBody(const Point &position,
                                                     const DirectionVector &orientation,
                                                     const std::shared_ptr<ObservableNavEnvironment> &environment)
{
  return std::make_shared<Body>(position,
                                orientation,
                                environment,
                                BodyShapeFactory<Point>::onlyOrigin());
}

std::unique_ptr<DRobotMCU> DeliveryRobot::getNewRobotMCU(IMoveMechanism &moveMechanism,
                                                         NetworkAdapter &networkAdapter,
                                                         RackMotor &rackMotor,
                                                         PodHolder &podHolder,
                                                         IDepleting &energySource,
                                                         ObservableNavEnvironment &env)
{
  return std::make_unique<DRobotMCU>(moveMechanism, networkAdapter, rackMotor, podHolder, energySource, env);
}

std::unique_ptr<RobotMoveMechanism> DeliveryRobot::getNewRobotMovement(
    const std::shared_ptr<Body> &body, IDepleting &resource)
{
  return std::make_unique<RobotMoveMechanism>(body, resource);
}