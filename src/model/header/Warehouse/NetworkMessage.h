#ifndef NETWORK_MESSAGE__H
#define NETWORK_MESSAGE__H

#include "DirectionVector.h"
#include "NetworkMessageHandler.h"

/************************************************
 * @brief Message that can be transferred in a network.
 * The massage can be dispatched to an appropriate handler
 * for further processing.
 ************************************************/
class AbstractNetworkMessage
{
public:
    virtual ~AbstractNetworkMessage() = default;
    virtual void dispatch(NetworkMessageHandler &networkMessageHandler) = 0;
};

/**************************************************
 * @brief Message that can be transferred in a network.
 * We use CRTP to regain subclass type information,
 * so we can dispatch it to an appropriate handler
 * based on solely on type.
 **************************************************/
template <typename Derived> // CRTP
class NetworkMessage : public AbstractNetworkMessage
{
public:
    virtual ~NetworkMessage() = default;
    virtual void dispatch(NetworkMessageHandler &networkMessageHandler) override
    {
        networkMessageHandler.receive(static_cast<const Derived &>(*this));
    }

protected:
    NetworkMessage() = default;
    NetworkMessage(const NetworkMessage &) = default;
    NetworkMessage(NetworkMessage &&) = default;
};

/**************************************************
 * @brief Message for an agent ot move to an adjacent
 * tile in a provided direction.
 **************************************************/
class MoveAgentMessage : public NetworkMessage<MoveAgentMessage>
{

public:
    explicit MoveAgentMessage(const DirectionVector<> &moveDirection);
    virtual ~MoveAgentMessage();
    const DirectionVector<> &getMoveDirection() const;

private:
    DirectionVector<> moveDirection;
};

#endif /* NETWORK_MESSAGE__H */