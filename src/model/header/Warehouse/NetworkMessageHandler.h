#ifndef NETWORK_MESSAGE_HANDLER__H
#define NETWORK_MESSAGE_HANDLER__H

// ###################### FORWARD DECLARATIONS ############################
class AbstractNetworkMessage;
class MoveAgentMessage;
// ########################################################################

class NetworkMessageHandler
{
public:
    virtual void receive(const AbstractNetworkMessage &message) const {}
    virtual void receive(const MoveAgentMessage &message) const
    { /* receive(static_cast<const AbstractNetworkMessage &>(message)); */
    }

protected:
    NetworkMessageHandler() = default;
    virtual ~NetworkMessageHandler() = default;
    NetworkMessageHandler(const NetworkMessageHandler &) = default;
    NetworkMessageHandler(NetworkMessageHandler &&) = default;
};

#endif /* NETWORK_MESSAGE_HANDLER__H */