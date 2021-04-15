#ifndef TASK_MANAGER__H
#define TASK_MANAGER__H

#include "DeliveryStation.h"
#include "OrderModel.h"
#include "Pod.h"
#include "PodDock.h"
#include "Point.h"
#include "Task.h"
#include <algorithm>
#include <map>
#include <memory>
#include <queue>
#include <vector>

template <typename TEnvironment>
class TaskManager
{
public:
    using Environment = TEnvironment;
    using Pod = ::Pod<OrderModel, Environment>;
    using PodDock = ::PodDock<Environment>;
    using Point = typename Environment::Point;

public:
    TaskManager()
        : podDocks(nullptr), deliveryStations(nullptr) {}

    virtual ~TaskManager() {}

public:
    /***************************************************************
     * @brief Returns all tasks in the warehouse.
     ***************************************************************/
    const std::vector<std::unique_ptr<Task>> &getTasks() const
    {
        return tasks;
    }

    /***************************************************************
     * @brief Sets which Pod Docks are scanned for possible tasks.
     ***************************************************************/
    void setPodDocks(const std::vector<std::shared_ptr<PodDock>> *podDocks)
    {
        this->podDocks = podDocks;
    }

    /*****************************************************************
     * @brief Sets which Delivery stations are for possible delivery
     * task targets.
     *****************************************************************/
    void setDeliveryStations(const std::vector<std::shared_ptr<DeliveryStation>> *deliveryStations)
    {
        orderIDToDeliveryStation.clear();
        this->deliveryStations = deliveryStations;

        for (const auto &deliveryStation : *(this->deliveryStations))
            orderIDToDeliveryStation[deliveryStation->getAcceptedOrderID()] = deliveryStation.get();
    }

    /***************************************************************
     * @brief Scans All PodDocks for Pods and creates the
     * associated DeliveryTasks if Pod dock has pod and it has orders
     ***************************************************************/
    void createDeliveryTasks()
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

    /*****************************************************************
     * Adds a new order and refreshes a associated task
     * or creates one if there is none.
     *****************************************************************/
    void addOrder(const OrderModel &order) {}

private:
    void createTask(const PodDock &podDock)
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
        sortedTasks.push(tasks.back().get());
    }

    /******************************************************************************
     * @brief Sorts the waypoints by distance so that always the closest
     * WayPoint is followed by an another.
     ******************************************************************************/
    int OptimizeWayPoints(std::vector<Point> &wayPoints)
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

    /*****************************************************************************
     * @brief Comparator for priority queue.
     *****************************************************************************/
    struct task_pointer_element_comparator
    {
        bool operator()(const Task *lhs,
                        const Task *rhs) const
        {
            if (!lhs || !rhs)
                return true;
            return *lhs > *rhs;
        }
    };

private:
    std::vector<std::unique_ptr<Task>> tasks;
    std::vector<std::unique_ptr<Task>> finishedTasks;
    std::priority_queue<Task *, std::vector<Task *>, task_pointer_element_comparator> sortedTasks;

    const std::vector<std::shared_ptr<PodDock>> *podDocks;
    const std::vector<std::shared_ptr<DeliveryStation>> *deliveryStations;

    std::map<int, DeliveryStation *> orderIDToDeliveryStation;
};

#endif