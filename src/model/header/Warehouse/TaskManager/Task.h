#ifndef TASK__H
#define TASK__H

#include "Point.h"
#include <vector>

class Task
{
public:
    // ########################## CONSTRUCTORS ########################### //
    explicit Task(const std::vector<Point<>> &wayPoints)
        : wayPoints(wayPoints) {}

    explicit Task(std::vector<Point<>> &&wayPoints)
        : wayPoints(std::move(wayPoints)) {}

    Task(const Task &other)
        : wayPoints(other.wayPoints) {}

    Task(const Task &&other)
        : wayPoints(std::move(other.wayPoints)) {}

    virtual ~Task() {}

protected:
    std::vector<Point<>> wayPoints;
};

class DeliveryTask : public Task
{
    // ########################## CONSTRUCTORS ########################### //
    explicit DeliveryTask(const std::vector<Point<>> &wayPoints)
        : Task(wayPoints) {}

    explicit DeliveryTask(std::vector<Point<>> &&wayPoints)
        : Task(std::move(wayPoints)) {}

    DeliveryTask(const DeliveryTask &other)
        : Task(other.wayPoints) {}

    DeliveryTask(const DeliveryTask &&other)
        : Task(std::move(other.wayPoints)) {}

    virtual ~DeliveryTask() {}
};

#endif