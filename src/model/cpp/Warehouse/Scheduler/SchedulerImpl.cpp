#include "SchedulerImpl.h"
#include "AMotor.h"
#include "IMoveMechanism.h"
#include "Task.h"
#include "TaskManager.h"
#include <iostream>
#include <limits>

SchedulerImpl::SchedulerImpl(TaskManager *taskManager)
    : taskManager(taskManager)
{
}

SchedulerImpl::~SchedulerImpl()
{
    networkAdapter.disconnect();
}

void SchedulerImpl::tick(int timeStamp)
{
    processMessages();
    doTaskAssignment();
    forwardAssignments();
}

const NetworkAdapter &SchedulerImpl::getNetworkAdapter() const { return networkAdapter; }
NetworkAdapter &SchedulerImpl::getNetworkAdapter() { return networkAdapter; }
void SchedulerImpl::setTaskManager(TaskManager *taskManager) { this->taskManager = taskManager; }

void SchedulerImpl::doTaskAssignment()
{
    // Clear all incoming requests if there are no tasks
    if (taskManager->getUnAssignedTasks().empty())
    {
        // Clear Priority Queue
        std::priority_queue<const AgentControlData *,
                            std::vector<const AgentControlData *>,
                            EnergyResourceComparator>()
            .swap(sortedAgentData);

        return;
    }

    // Try to assign a task to each agent
    while (!sortedAgentData.empty())
    {
        const AgentControlData *controlData = sortedAgentData.top();
        std::unique_ptr<TaskAssignment> assignment(tryAssignTask(*controlData));

        if (assignment != nullptr)
            sortedAssignmentData.push(std::move(assignment));
        else
        {
        }
        sortedAgentData.pop();
    }
}

void SchedulerImpl::forwardAssignments()
{
}

void SchedulerImpl::processMessages()
{
    while (!networkAdapter.isMessageQueueEmpty())
    {
        std::shared_ptr<AbstractNetworkMessage> message = networkAdapter.poll();
        message->dispatch(*this);
    }
}

void SchedulerImpl::receive(const AgentControlRequestMessage &message)
{
    sortedAgentData.push(message.controlData);
}

bool SchedulerImpl::softTest(const Task &task, const AgentControlData &controlData, int &out_costHeuristic) const
{
    int agentToTask = TaskManager::Point::manhattanNorm(controlData.moveMechanism.getBody()->getPose().getPosition(),
                                                        task.getWayPoints()[0]);

    out_costHeuristic = agentToTask + task.getSumDistance() + (task.getWayPoints().size() * SOFT_TEST_BIAS);

    return controlData.energySource.getCharge() >= out_costHeuristic;
}

TaskAssignment *SchedulerImpl::tryAssignTask(const AgentControlData &controlData)
{
    int minCostHeuristic = INT_MAX;

    int assignedTaskInd = -1;
    for (size_t i = 0; i < taskManager->getUnAssignedTasks().size(); ++i)
    {
        int costHeuristic;

        bool success = softTest(*taskManager->getUnAssignedTasks()[i], controlData, costHeuristic);
        if (success && costHeuristic < minCostHeuristic)
        {
            assignedTaskInd = i;
            minCostHeuristic = costHeuristic;
        }
    }

    if (assignedTaskInd != -1)
    {
        TaskAssignment *assignment = new TaskAssignment(taskManager->getUnAssignedTasks()[assignedTaskInd],
                                                        &controlData, minCostHeuristic);

        auto it = taskManager->getUnAssignedTasks().begin() + assignedTaskInd;
        taskManager->getUnAssignedTasks().erase(it);
        return assignment;
    }
    else
        return nullptr;
}