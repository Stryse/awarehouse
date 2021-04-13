#ifndef TASK_MANAGER__H
#define TASK_MANAGER__H

#include "OrderModel.h"
#include "Pod.h"
#include "PodDock.h"
#include "Task.h"
#include <map>
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
        : podDocks(nullptr) {}

    virtual ~TaskManager() {}

public:
    /***************************************************************
     * @brief Returns all tasks in the warehouse.
     ***************************************************************/
    const std::vector<Task> &getTasks() const
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

    /***************************************************************
     * @brief Scans All PodDocks for Pods and creates the
     * associated DeliveryTasks.
     ***************************************************************/
    void createDeliveryTasks()
    {
        tasks.clear();

        if (podDocks != nullptr)
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
        std::vector<Point> wpoints;
        wpoints.push_back(podDock.getPosition());
        tasks.push_back(Task(std::move(wpoints)));
    }

private:
    std::vector<Task> tasks;
    const std::vector<std::shared_ptr<PodDock>> *podDocks;
};

#endif