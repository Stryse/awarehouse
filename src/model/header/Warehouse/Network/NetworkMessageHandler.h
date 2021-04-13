#ifndef NETWORK_MESSAGE_HANDLER__H
#define NETWORK_MESSAGE_HANDLER__H

// ###################### FORWARD DECLARATIONS ############################
class AbstractNetworkMessage;
class MoveAgentMessage;
class ChargeAgentMessage;
class PickupPodMessage;
class PutDownPodMessage;
class PutDownOrderMessage;
// ########################################################################

class NetworkMessageHandler
{
public:
    virtual void receive(const AbstractNetworkMessage &message) {}
    virtual void receive(const MoveAgentMessage &message) {}
    virtual void receive(const ChargeAgentMessage &message) {}
    virtual void receive(const PickupPodMessage &message) {}
    virtual void receive(const PutDownPodMessage &message) {}
    virtual void receive(const PutDownOrderMessage &message) {}

protected:
    NetworkMessageHandler() = default;
    virtual ~NetworkMessageHandler() = default;
    NetworkMessageHandler(const NetworkMessageHandler &) = default;
    NetworkMessageHandler(NetworkMessageHandler &&) = default;
};

#endif /* NETWORK_MESSAGE_HANDLER__H */