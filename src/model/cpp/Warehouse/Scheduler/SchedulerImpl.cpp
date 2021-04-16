#include "SchedulerImpl.h"
#include "TaskManager.h"

SchedulerImpl::SchedulerImpl(TaskManager<ObservableNavEnvironment<Tile>> &taskManager)
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
}

void SchedulerImpl::doTaskAssignment()
{
}

void SchedulerImpl::processMessages()
{
    while (!networkAdapter.isMessageQueueEmpty())
    {
        std::unique_ptr<AbstractNetworkMessage> message = networkAdapter.poll();
        message->dispatch(*this);
    }
}

void SchedulerImpl::receive(const AgentControlRequestMessage &message)
{
}

const NetworkAdapter &SchedulerImpl::getNetworkAdapter() const { return networkAdapter; }
NetworkAdapter &SchedulerImpl::getNetworkAdapter() { return networkAdapter; }