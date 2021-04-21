#include "State.h"
#include "PathFinder.h"
#include <iostream>

State::State(const std::shared_ptr<ObservableNavEnvironment> &environment,
             const std::vector<std::shared_ptr<ChargingStation>> &chStations,
             const std::vector<std::shared_ptr<DeliveryStation>> &deliveryStations,
             const std::vector<std::shared_ptr<PodDock>> &podDocks,
             const std::vector<Pod<OrderModel> *> &pods,
             const std::vector<std::shared_ptr<DeliveryRobot>> &robots,
             int rowCount, int colCount)

    : rowCount(rowCount), colCount(colCount),
      environment(environment),
      chStations(chStations),
      deliveryStations(deliveryStations),
      podDocks(podDocks),
      pods(pods),
      robots(robots),
      pathFinder(*this),
      taskManager(this->podDocks, this->deliveryStations)
{
}

State::State(std::shared_ptr<ObservableNavEnvironment> &&environment,
             std::vector<std::shared_ptr<ChargingStation>> &&chStations,
             std::vector<std::shared_ptr<DeliveryStation>> &&deliveryStations,
             std::vector<std::shared_ptr<PodDock>> &&podDocks,
             std::vector<Pod<OrderModel> *> &&pods,
             std::vector<std::shared_ptr<DeliveryRobot>> &&robots,
             int rowCount, int colCount)

    : rowCount(rowCount), colCount(colCount),
      environment(std::move(environment)),
      chStations(std::move(chStations)),
      deliveryStations(std::move(deliveryStations)),
      podDocks(std::move(podDocks)),
      pods(std::move(pods)),
      robots(std::move(robots)),
      pathFinder(*this),
      taskManager(this->podDocks, this->deliveryStations)
{
}

State::~State() {}

void State::tick(int timeStamp)
{
  std::cerr << "State::tick()" << std::endl;
  for (auto &robot : robots)
    robot->tick(timeStamp);
}

int State::getRowCount() const { return rowCount; }
int State::getColCount() const { return colCount; }

const std::vector<std::shared_ptr<ChargingStation>> &State::getChargingStations() const { return chStations; }
const std::vector<std::shared_ptr<PodDock>> &State::getPodDocks() const { return podDocks; }
const std::vector<Pod<OrderModel> *> &State::getPods() const { return pods; }
const std::vector<std::shared_ptr<DeliveryStation>> &State::getDeliveryStations() const { return deliveryStations; }
const std::vector<std::shared_ptr<DeliveryRobot>> &State::getRobots() const { return robots; }

const PathFinder &State::getPathFinder() const { return pathFinder; }
PathFinder &State::getPathFinder() { return pathFinder; }

const TaskManager &State::getTaskManager() const { return taskManager; }
TaskManager &State::getTaskManager() { return taskManager; }