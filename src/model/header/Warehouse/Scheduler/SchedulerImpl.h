#ifndef SCHEDULER_IMPL__H
#define SCHEDULER_IMPL__H

#include "AScheduler.h"
#include "IDepleting.h"
#include "NetworkAdapter.h"
#include "NetworkMessage.h"
#include "NetworkMessageHandler.h"
#include <queue>

#include <iostream> // TODO REMOVE

// ############################ FORWARD DECLARATIONS ############################## //
class ObservableNavEnvironment;
class TaskManager;
class Task;
// ################################################################################ //
struct TaskAssignment
{
    TaskAssignment(const Task *task, const AgentControlData *controlData, int costHeuristic)
        : task(task), controlData(controlData), costHeuristic(costHeuristic)
    {
    }

    const Task *task;
    const AgentControlData *controlData;
    int costHeuristic;
};

/**********************************************************************************
 * @brief Returns whether the left agent control data has less current energy
 **********************************************************************************/
struct EnergyResourceComparator
{
    bool operator()(const AgentControlData *lhs, const AgentControlData *rhs)
    {
        return lhs->energySource.getCharge() >= rhs->energySource.getCharge();
    }
};

struct TaskAssignmentComparator
{
    bool operator()(const std::unique_ptr<TaskAssignment> &lhs, const std::unique_ptr<TaskAssignment> &rhs)
    {
        int lhsNormalizedCost = (static_cast<double>(lhs->controlData->energySource.getCharge()) / lhs->costHeuristic) * 1000;
        int rhsNormalizedCost = (static_cast<double>(rhs->controlData->energySource.getCharge()) / rhs->costHeuristic) * 1000;

        return lhsNormalizedCost >= rhsNormalizedCost;
    }
};

/**********************************************************************
 * @brief A Component which accepts AgentControlRequestMessage-s
 * And tries to assign a task to an agent.
 * If it thinks it has found a suitable task forwards a
 * TaskPlanRequest to the controller.
 * If it thinks there is no suitable task it forward a
 * ChargePlanRequest to the controller.
 **********************************************************************/
class SchedulerImpl : public NetworkMessageHandler
{
public:
    explicit SchedulerImpl(TaskManager *taskManager = nullptr);
    explicit SchedulerImpl(const SchedulerImpl &other) = delete;
    explicit SchedulerImpl(SchedulerImpl &&other) = delete;
    SchedulerImpl &operator=(const SchedulerImpl &other) = delete;
    virtual ~SchedulerImpl();

    const NetworkAdapter &getNetworkAdapter() const;
    NetworkAdapter &getNetworkAdapter();
    void setTaskManager(TaskManager *taskManager);

public:
    /****************************************************************************************
     * @brief 
     ***************************************************************************************/
    void tick(int timeStamp);

    /****************************************************************************************
     * @brief 
     ***************************************************************************************/
    void doTaskAssignment();

    /****************************************************************************************
     * @brief 
     ****************************************************************************************/
    void forwardAssignments();

public:
    // ##################### NetworkMessageHandler Implementation ###########################
    /****************************************************************************************
    * @brief Registers an AgentControlData entity on receiving AgentControlRequestMessage
    ****************************************************************************************/
    virtual void receive(const AgentControlRequestMessage &message) override;

public:
    /*******************************************************************
     * @brief Poll and dispatch all messages from the NetworkAdapters 
     * messageQueue to the appropriate NetworkMessagehandler.
     *******************************************************************/
    void processMessages();

private:
    /*****************************************************************************************
    * @brief A Heuristic that decides whether an agent may be able to perform a task by only
    * distances. An agent may be able to perform a task if it has enough energy to:
    * - Reach the task
    * - Perform it with task cost heuristic
    * - For each waypoint of the task there's a heuristical cost so we underestimate less.
    ******************************************************************************************/
    bool softTest(const Task &task, const AgentControlData &controlData, int &out_costHeuristic) const;

    /******************************************************************************************
     * @brief Tests all tasks with soft test and finds the lowest heuristic cost.
     * If all soft tests fail it returns false indicating there is no suitable task.
     *****************************************************************************************/
    TaskAssignment *tryAssignTask(const AgentControlData &controlData);

private:
    TaskManager *taskManager;
    NetworkAdapter networkAdapter;

    std::priority_queue<const AgentControlData *,
                        std::vector<const AgentControlData *>,
                        EnergyResourceComparator>
        sortedAgentData;

    std::priority_queue<std::unique_ptr<TaskAssignment>,
                        std::vector<std::unique_ptr<TaskAssignment>>,
                        TaskAssignmentComparator>
        sortedAssignmentData;

    static const int SOFT_TEST_BIAS = 5;
};

#endif /* SCHEDULER_IMPL__H */
