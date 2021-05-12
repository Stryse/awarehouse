#ifndef NETWORK_MESSAGE__H
#define NETWORK_MESSAGE__H

#include "DirectionVector.h"
#include "NetworkMessageHandler.h"
#include <memory>

// ####################### FORWARD DECLARATIONS ######################### //
class IDepleting;
class IMoveMechanism;
class ObservableNavEnvironment;
// ###################################################################### //

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

/******************************************************************
 * @brief An abstract network message wrapper which can
 * hold an AbstractNetworkMessage subclass and a target address.
 * Used for storing messages which are to be sent to different
 * targets.
 *******************************************************************/
struct TargetedMessage
{
    TargetedMessage(int address, std::shared_ptr<AbstractNetworkMessage> message)
        : address(address), message(std::move(message)) {}

    int address;
    std::shared_ptr<AbstractNetworkMessage> message;
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
public:
    explicit ChargeAgentMessage(int senderAddress)
        : NetworkMessage<ChargeAgentMessage>(senderAddress) {}

    virtual ~ChargeAgentMessage() = default;
};

/*****************************************************************
 * @brief Message to an agent to pick up a pod at its position.
 *****************************************************************/
class PickupPodMessage : public NetworkMessage<PickupPodMessage>
{
public:
    explicit PickupPodMessage(int senderAddress)
        : NetworkMessage<PickupPodMessage>(senderAddress) {}

    virtual ~PickupPodMessage() = default;
};

/******************************************************************
 * @brief Message to an agent to put down a pod at its position.
 ******************************************************************/
class PutDownPodMessage : public NetworkMessage<PutDownPodMessage>
{
public:
    explicit PutDownPodMessage(int senderAddress)
        : NetworkMessage<PutDownPodMessage>(senderAddress) {}

    virtual ~PutDownPodMessage() = default;
};

/*********************************************************************
 * @brief Message to an agent to put down an order at its position.
 *********************************************************************/
class PutDownOrderMessage : public NetworkMessage<PutDownOrderMessage>
{
public:
    explicit PutDownOrderMessage(int senderAddress)
        : NetworkMessage<PutDownOrderMessage>(senderAddress) {}

    virtual ~PutDownOrderMessage() = default;
};

/**********************************************************************
 * @brief Containing the neccessary data to Agent Control
 **********************************************************************/
struct AgentControlData
{
    using IMoveMechanism = ::IMoveMechanism;

    AgentControlData(const IDepleting &energySource,
                     const IMoveMechanism &moveMechanism,
                     const std::string &ID = "UNKNOWN",
                     int address = 0)

        : energySource(energySource),
          moveMechanism(moveMechanism),
          ID(ID),
          address(address)
    {
    }

    const IDepleting &energySource;
    const IMoveMechanism &moveMechanism;
    const std::string ID;
    int address;
};

/*********************************************************************
 * @brief Message from an agent requesting control from an Agent
 * controlling entity.
 *********************************************************************/
class AgentControlRequestMessage : public NetworkMessage<AgentControlRequestMessage>
{
public:
    explicit AgentControlRequestMessage(const AgentControlData *controlData, int senderAddress)
        : NetworkMessage<AgentControlRequestMessage>(senderAddress),
          controlData(controlData)
    {
    }

    virtual ~AgentControlRequestMessage() = default;
    const AgentControlData *controlData;
};

/*********************************************************************
 * @brief Message from an agent controlling entity signalling
 * it has taken control of the Agent
 *********************************************************************/
class AgentControlGrantedMessage : public NetworkMessage<AgentControlGrantedMessage>
{
public:
    explicit AgentControlGrantedMessage(int senderAddress)
        : NetworkMessage<AgentControlGrantedMessage>(senderAddress) {}

    virtual ~AgentControlGrantedMessage() = default;
};

/**********************************************************************
 * @brief Message from an agent controlling entity signalling it
 * has given up control of the Agent
 **********************************************************************/
class AgentControlGiveUpMessage : public NetworkMessage<AgentControlGiveUpMessage>
{
public:
    explicit AgentControlGiveUpMessage(int senderAddress)
        : NetworkMessage<AgentControlGiveUpMessage>(senderAddress) {}

    virtual ~AgentControlGiveUpMessage() = default;
};
#endif /* NETWORK_MESSAGE__H */