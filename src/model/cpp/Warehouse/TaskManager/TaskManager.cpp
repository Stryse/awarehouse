#include "TaskManager.h"

TaskManager::TaskManager(const std::vector<std::shared_ptr<PodDock>> &podDocks,
                         const std::vector<std::shared_ptr<DeliveryStation>> &deliveryStations)
    : podDocks(&podDocks)
{
    setDeliveryStations(&deliveryStations);
    createDeliveryTasks();
}

const std::vector<std::unique_ptr<Task>> &TaskManager::getTasks() const
{
    return tasks;
}

void TaskManager::setPodDocks(const std::vector<std::shared_ptr<PodDock>> *podDocks)
{
    this->podDocks = podDocks;
}

void TaskManager::setDeliveryStations(const std::vector<std::shared_ptr<DeliveryStation>> *deliveryStations)
{
    orderIDToDeliveryStation.clear();
    this->deliveryStations = deliveryStations;

    for (const auto &deliveryStation : *(this->deliveryStations))
        orderIDToDeliveryStation[deliveryStation->getAcceptedOrderID()] = deliveryStation.get();
}

void TaskManager::createDeliveryTasks()
{
    tasks.clear();
    if (podDocks != nullptr && deliveryStations != nullptr)
    {
        for (const auto &podDock : *podDocks)
        {
            if (podDock->getPodHolder().getChildPod() != nullptr)
                createTask(*podDock);
        }
    }
}

void TaskManager::addOrder(const OrderModel &order)
{
}

void TaskManager::createTask(const PodDock &podDock)
{
    Pod &pod = *podDock.getPodHolder().getChildPod();
    if (pod.getInventory().empty())
        return;

    std::vector<Point> wayPoints;

    // First Waypoint is podDock position
    wayPoints.push_back(podDock.getPosition());

    for (auto &order : pod.getInventory())
    {
        // Find associated delivery staion
        auto it = orderIDToDeliveryStation.find(order->getCategory());

        // Middle Waypoints are delivery station positions
        if (it != orderIDToDeliveryStation.end())
            wayPoints.push_back(it->second->getPosition());
    }

    // Do not create task if there are no appropriate delivery stations
    if (wayPoints.size() < 2)
        return;

    int sumDistance = OptimizeWayPoints(wayPoints);

    // Update with last Waypoint
    sumDistance += Point::manhattanNorm(wayPoints.back(), podDock.getPosition());

    // Last Waypoint is also podDock position
    wayPoints.push_back(podDock.getPosition());

    // Register Delivery Task
    tasks.emplace_back(std::make_unique<DeliveryTask>(std::move(wayPoints), sumDistance));
}

int TaskManager::OptimizeWayPoints(std::vector<Point> &wayPoints)
{
    int sumDistance = 0;
    for (int i = 1; i < wayPoints.size(); ++i)
    {
        int minDistance = Point::manhattanNorm(wayPoints[i - 1], wayPoints[i]);

        for (int j = i + 1; j < wayPoints.size(); ++j)
        {
            int distance = Point::manhattanNorm(wayPoints[i - 1], wayPoints[j]);
            if (distance < minDistance)
            {
                std::swap(wayPoints[i], wayPoints[j]);
                minDistance = distance;
            }
        }
        sumDistance += minDistance;
    }
    return sumDistance;
}