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

// #################### FORWARD DECLARATIONS ######################## //
class TaskManager;
// ################################################################## //

class TaskManager
{
public:
    using Pod = ::Pod<OrderModel>;
    using Point = ::Point<int, 3>;

public:
    TaskManager(const std::vector<std::shared_ptr<PodDock>> &podDocks,
                const std::vector<std::shared_ptr<DeliveryStation>> &deliveryStations);

    TaskManager(const TaskManager &other) = delete;
    TaskManager(TaskManager &&other) = delete;
    TaskManager &operator=(const TaskManager &other) = delete;
    virtual ~TaskManager() = default;

public:
    /***************************************************************
     * @brief Returns all tasks in the warehouse.
     ***************************************************************/
    const std::vector<std::unique_ptr<Task>> &getTasks() const;

    /***************************************************************
     * @brief Sets which Pod Docks are scanned for possible tasks.
     ***************************************************************/
    void setPodDocks(const std::vector<std::shared_ptr<PodDock>> *podDocks);

    /*****************************************************************
     * @brief Sets which Delivery stations are for possible delivery
     * task targets.
     *****************************************************************/
    void setDeliveryStations(const std::vector<std::shared_ptr<DeliveryStation>> *deliveryStations);

    /***************************************************************
     * @brief Scans All PodDocks for Pods and creates the
     * associated DeliveryTasks if Pod dock has pod and it has orders
     ***************************************************************/
    void createDeliveryTasks();

    /*****************************************************************
     * Adds a new order and refreshes a associated task
     * or creates one if there is none.
     *****************************************************************/
    void addOrder(const OrderModel &order);

private:
    void createTask(const PodDock &podDock);

    /******************************************************************************
     * @brief Sorts the waypoints by distance so that always the closest
     * WayPoint is followed by an another.
     ******************************************************************************/
    int OptimizeWayPoints(std::vector<Point> &wayPoints);

private:
    std::vector<std::unique_ptr<Task>> tasks;
    std::vector<std::unique_ptr<Task>> finishedTasks;

    const std::vector<std::shared_ptr<PodDock>> *podDocks;
    const std::vector<std::shared_ptr<DeliveryStation>> *deliveryStations;

    std::map<int, DeliveryStation *> orderIDToDeliveryStation;
};

#endif