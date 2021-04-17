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
// ################################################################################ //

struct EnergyResourceComparator
{
    bool operator()(const AgentControlData &lhs, const AgentControlData &rhs)
    {
        return lhs.energySource.getCharge() <= rhs.energySource.getCharge();
    }
};

class SchedulerImpl : public AScheduler, NetworkMessageHandler
{
public:
    explicit SchedulerImpl();
    explicit SchedulerImpl(TaskManager *taskManager);
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
    TaskManager *taskManager;

    std::priority_queue<AgentControlData,
                        std::vector<AgentControlData>,
                        EnergyResourceComparator>
        sortedAgentData;
};

#endif /* SCHEDULER_IMPL__H */
