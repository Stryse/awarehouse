#ifndef SCHEDULER_IMPL__H
#define SCHEDULER_IMPL__H

#include "AScheduler.h"
#include "IDepleting.h"
#include "NetworkAdapter.h"
#include "NetworkMessage.h"
#include "NetworkMessageHandler.h"
#include <queue>

// ############################ FORWARD DECLARATIONS ############################## //
class ObservableNavEnvironment;
class TaskManager;
class ControllerImpl;
class Task;
// ################################################################################ //
struct TaskAssignment
{
    TaskAssignment(Task *task, const AgentControlData *controlData, int costHeuristic)
        : task(task), controlData(controlData), costHeuristic(costHeuristic)
    {
    }

    Task *task;
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
    bool operator()(const TaskAssignment *lhs, const TaskAssignment *rhs)
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
    explicit SchedulerImpl(TaskManager *taskManager = nullptr, ControllerImpl *controller = nullptr);
    explicit SchedulerImpl(const SchedulerImpl &other) = delete;
    explicit SchedulerImpl(SchedulerImpl &&other) = delete;
    SchedulerImpl &operator=(const SchedulerImpl &other) = delete;
    virtual ~SchedulerImpl();

    const NetworkAdapter &getNetworkAdapter() const;
    NetworkAdapter &getNetworkAdapter();
    void setTaskManager(TaskManager *taskManager);
    void setController(ControllerImpl *controller);

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
    ControllerImpl *controller;

    NetworkAdapter networkAdapter;

    std::priority_queue<const AgentControlData *,
                        std::vector<const AgentControlData *>,
                        EnergyResourceComparator>
        sortedAgentData;

    std::priority_queue<TaskAssignment *,
                        std::vector<TaskAssignment *>,
                        TaskAssignmentComparator>
        sortedAssignmentData;

    static const int SOFT_TEST_BIAS = 5;
    static const int SOFT_TEST_MIN_LEFT = 10;
};

#endif /* SCHEDULER_IMPL__H */
