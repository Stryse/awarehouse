#ifndef NETWORK_MESSAGE_HANDLER__H
#define NETWORK_MESSAGE_HANDLER__H

// ###################### FORWARD DECLARATIONS ############################
class AbstractNetworkMessage;
class MoveAgentMessage;
// ########################################################################

class NetworkMessageHandler
{
public:
    virtual void receive(const AbstractNetworkMessage &message) {}
    virtual void receive(const MoveAgentMessage &message) {}

protected:
    NetworkMessageHandler() = default;
    virtual ~NetworkMessageHandler() = default;
    NetworkMessageHandler(const NetworkMessageHandler &) = default;
    NetworkMessageHandler(NetworkMessageHandler &&) = default;
};

#endif /* NETWORK_MESSAGE_HANDLER__H */