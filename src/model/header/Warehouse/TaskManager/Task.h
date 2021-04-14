#ifndef TASK__H
#define TASK__H

#include "Point.h"
#include <vector>

class Task
{
public:
    // ########################## CONSTRUCTORS ########################### //
    explicit Task(const std::vector<Point<>> &wayPoints, int sumDistance)
        : wayPoints(wayPoints), sumDistance(sumDistance) {}

    explicit Task(std::vector<Point<>> &&wayPoints, int sumDistance)
        : wayPoints(std::move(wayPoints)), sumDistance(sumDistance) {}

    Task(const Task &other)
        : wayPoints(other.wayPoints), sumDistance(other.sumDistance) {}

    Task(const Task &&other)
        : wayPoints(std::move(other.wayPoints)), sumDistance(other.sumDistance) {}

    virtual ~Task() {}

protected:
    std::vector<Point<>> wayPoints;
    int sumDistance;
};

class DeliveryTask : public Task
{
public:
    // ########################## CONSTRUCTORS ########################### //
    explicit DeliveryTask(const std::vector<Point<>> &wayPoints, int sumDistance)
        : Task(wayPoints, sumDistance) {}

    explicit DeliveryTask(std::vector<Point<>> &&wayPoints, int sumDistance)
        : Task(std::move(wayPoints), sumDistance) {}

    DeliveryTask(const DeliveryTask &other)
        : Task(other.wayPoints, sumDistance) {}

    DeliveryTask(const DeliveryTask &&other)
        : Task(std::move(other.wayPoints), sumDistance) {}

    virtual ~DeliveryTask() {}
};

#endif