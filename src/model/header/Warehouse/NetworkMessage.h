#ifndef NETWORK_MESSAGE__H
#define NETWORK_MESSAGE__H

#include "DirectionVector.h"
#include "NetworkMessageHandler.h"

/***************************************************************
 * @brief Message that can be transferred in a network.
 * The massage can be dispatched to an appropriate handler
 * for further processing.
 ***************************************************************/
class AbstractNetworkMessage
{
public:
    virtual ~AbstractNetworkMessage() = default;
    virtual void dispatch(NetworkMessageHandler &networkMessageHandler) = 0;
    int getSenderAddress() const { return senderAddress; }

protected:
    explicit AbstractNetworkMessage(int senderAddress)
        : senderAddress(senderAddress) {}

private:
    int senderAddress;
};

/*****************************************************************
 * @brief Message that can be transferred in a network.
 * We use CRTP to regain subclass type information,
 * so we can dispatch it to an appropriate handler
 * based on solely on type.
 *****************************************************************/
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
    explicit NetworkMessage(int senderAddress)
        : AbstractNetworkMessage(senderAddress) {}

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
    explicit MoveAgentMessage(const DirectionVector<> &moveDirection, int senderAddress);
    virtual ~MoveAgentMessage();
    const DirectionVector<> &getMoveDirection() const;

private:
    DirectionVector<> moveDirection;
};

/****************************************************************
 * @brief Message to an agent to begin charging at its position.
 ****************************************************************/
class ChargeAgentMessage : public NetworkMessage<ChargeAgentMessage>
{
    explicit ChargeAgentMessage(int senderAddress)
        : NetworkMessage(senderAddress) {}

    virtual ~ChargeAgentMessage() = default;
};

/*****************************************************************
 * @brief Message to an agent to pick up a pod at its position.
 *****************************************************************/
class PickupPodMessage : public NetworkMessage<PickupPodMessage>
{
    explicit PickupPodMessage(int senderAddress)
        : NetworkMessage(senderAddress) {}

    virtual ~PickupPodMessage() = default;
};

/******************************************************************
 * @brief Message to an agent to put down a pod at its position.
 ******************************************************************/
class PutDownPodMessage : public NetworkMessage<PutDownPodMessage>
{
    explicit PutDownPodMessage(int senderAddress)
        : NetworkMessage(senderAddress) {}

    virtual ~PutDownPodMessage() = default;
};

/*********************************************************************
 * @brief Message to an agent to put down an order at its position.
 *********************************************************************/
class PutDownOrderMessage : public NetworkMessage<PutDownOrderMessage>
{
    explicit PutDownOrderMessage(int senderAddress)
        : NetworkMessage(senderAddress) {}

    virtual ~PutDownOrderMessage() = default;
};

/*********************************************************************
 * @brief Message from an agent signalling it has finished charging.
 *********************************************************************/
class RechargedMessage : public NetworkMessage<RechargedMessage>
{
    explicit RechargedMessage(int senderAddress)
        : NetworkMessage(senderAddress) {}

    virtual ~RechargedMessage() = default;
};

#endif /* NETWORK_MESSAGE__H */