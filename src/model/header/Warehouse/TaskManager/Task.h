#ifndef TASK__H
#define TASK__H

#include "Agent.h"
#include "Point.h"
#include <vector>

/***************************************************************************
 * @brief Data of a delivery task.
 * Contains the points which are needed to be visited and
 * the overall distance travelled in manhattan distance.
 ***************************************************************************/
class Task
{
public:
    // ########################## CONSTRUCTORS ########################### //
    explicit Task(const std::vector<Point<>> &wayPoints, int sumDistance)
        : wayPoints(wayPoints), sumDistance(sumDistance), assignedAgentID("N/A") {}

    explicit Task(const Task &other) = delete;
    explicit Task(Task &&other) = delete;
    Task &operator=(const Task &other) = delete;
    virtual ~Task() {}

    friend inline bool operator>(const Task &lhs, const Task &rhs) { return lhs.sumDistance > rhs.sumDistance; }

    //Setter
    void setAssignedAgentID(const std::string &assignedAgentID) { this->assignedAgentID = assignedAgentID; }

    //Getter
    const std::vector<Point<>> &getWayPoints() const { return wayPoints; }
    int getSumDistance() const { return sumDistance; }
    const std::string &getAssignedAgentID() const { return assignedAgentID; }

protected:
    std::vector<Point<>> wayPoints;
    int sumDistance;
    std::string assignedAgentID;
};
#endif
