#ifndef NETWORK_MESSAGE_HANDLER__H
#define NETWORK_MESSAGE_HANDLER__H

// ###################### FORWARD DECLARATIONS ############################
class AbstractNetworkMessage;
class MoveAgentMessage;
class ChargeAgentMessage;
class PickupPodMessage;
class PutDownPodMessage;
class PutDownOrderMessage;
class AgentControlGrantedMessage;
class AgentControlRequestMessage;
class AgentControlGiveUpMessage;

class ObservableNavEnvironment;
// ########################################################################
/*****************************************************************************
 * @brief An entity which can handle received AbstractNetworkMessage subtypes.
 *****************************************************************************/
class NetworkMessageHandler
{
public:
    virtual void receive(const AbstractNetworkMessage &message) {}
    virtual void receive(const MoveAgentMessage &message) {}
    virtual void receive(const ChargeAgentMessage &message) {}
    virtual void receive(const PickupPodMessage &message) {}
    virtual void receive(const PutDownPodMessage &message) {}
    virtual void receive(const PutDownOrderMessage &message) {}
    virtual void receive(const AgentControlRequestMessage &message) {}
    virtual void receive(const AgentControlGrantedMessage &message) {}
    virtual void receive(const AgentControlGiveUpMessage &message) {}

protected:
    NetworkMessageHandler() = default;
    virtual ~NetworkMessageHandler() = default;
    NetworkMessageHandler(const NetworkMessageHandler &) = default;
    NetworkMessageHandler(NetworkMessageHandler &&) = default;
};

#endif /* NETWORK_MESSAGE_HANDLER__H */