#ifndef CONTROLLER_IMPL__H
#define CONTROLLER_IMPL__H

#include "AController.h"
#include "NetworkAdapter.h"
#include <memory>
#include <unordered_map>

// ###################### FORWARD DECLARATIONS ####################### //
class AbstractNetworkMessage;
// ################################################################### //

class ControllerImpl
{
public:
    ControllerImpl();
    virtual ~ControllerImpl();

public:
    void tick(int timeStamp);

    const NetworkAdapter &getNetworkAdapter() const;
    NetworkAdapter &getNetworkAdapter();

private:
    void broadcastMessages(int timeStamp);

private:
    NetworkAdapter networkAdapter;
    std::unordered_multimap<int, TargetedMessage> controlMessages;

private:
    // ################## Messages #############################
    std::shared_ptr<AgentControlGrantedMessage> MControlGranted;
    std::shared_ptr<MoveAgentMessage> MMoveAgentUp;
    std::shared_ptr<MoveAgentMessage> MMoveAgentDown;
    std::shared_ptr<MoveAgentMessage> MMoveAgentLeft;
    std::shared_ptr<MoveAgentMessage> MMoveAgentRight;
    std::shared_ptr<PickupPodMessage> MPickupPod;
    std::shared_ptr<PutDownPodMessage> MPutdownPod;
    std::shared_ptr<PutDownOrderMessage> MPutdownOrder;
    // #########################################################
};

#endif /* CONTROLLER_IMPL__H */