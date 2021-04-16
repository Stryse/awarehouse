#ifndef SCHEDULER_IMPL__H
#define SCHEDULER_IMPL__H

#include "AScheduler.h"
#include "NetworkAdapter.h"
#include "NetworkMessageHandler.h"
#include <queue>

// ############################ FORWARD DECLARATIONS ############################## //
template <typename TEnvironment>
class TaskManager;

class ObservableNavEnvironment;
// ################################################################################ //

class SchedulerImpl : public AScheduler, NetworkMessageHandler
{
public:
    explicit SchedulerImpl(TaskManager<ObservableNavEnvironment> &taskManager);
    virtual ~SchedulerImpl();

public:
    virtual void tick(int timeStamp) override;
    virtual void doTaskAssignment() override;
    virtual const NetworkAdapter &getNetworkAdapter() const override;
    virtual NetworkAdapter &getNetworkAdapter() override;

public:
    // ############## NetworkMessageHandler Implementation ###############
    virtual void receive(const AgentControlRequestMessage &message) override;

public:
    /*******************************************************************
     * @brief Poll and dispatch all messages from the NetworkAdapters 
     * messageQueue to the appropriate NetworkMessagehandler.
     *******************************************************************/
    void processMessages();

private:
    NetworkAdapter networkAdapter;
    TaskManager<ObservableNavEnvironment> &taskManager;
};

#endif /* SCHEDULER_IMPL__H */
