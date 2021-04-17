#ifndef TASK__H
#define TASK__H

#include "Point.h"
#include "Agent.h"
#include <vector>

class Task
{
public:
    // ########################## CONSTRUCTORS ########################### //
    explicit Task(const std::vector<Point<>> &wayPoints, int sumDistance)
        : wayPoints(wayPoints), sumDistance(sumDistance), assignedAgent(nullptr) {}

    explicit Task(std::vector<Point<>> &&wayPoints, int sumDistance)
        : wayPoints(std::move(wayPoints)), sumDistance(sumDistance), assignedAgent(nullptr) {}

    Task(const Task &other)
        : wayPoints(other.wayPoints), sumDistance(other.sumDistance) {}

    Task(const Task &&other)
        : wayPoints(std::move(other.wayPoints)), sumDistance(other.sumDistance) {}

    virtual ~Task() {}

    friend inline bool operator>(const Task &lhs, const Task &rhs) { return lhs.sumDistance > rhs.sumDistance; }

    //Getter
    const std::vector<Point<>>& getWayPoints() const { return wayPoints; }
    int getSumDistance() const { return sumDistance; }
    const Agent* getAssignedAgent() const { return assignedAgent; }

protected:
    std::vector<Point<>> wayPoints;
    int sumDistance;
    Agent* assignedAgent;
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
