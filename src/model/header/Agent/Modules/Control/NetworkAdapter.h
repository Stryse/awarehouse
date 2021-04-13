#ifndef NETWORK_ADAPTER__H
#define NETWORK_ADAPTER__H

#include "NetworkMessage.h"
#include <memory>
#include <queue>

// ################## FORWARD DECLARATIONS #####################
class Network;
// #############################################################

/**************************************************************************************
 * @brief Part of an agent which is responsible for sending,receiving, and dispatching
 * incoming messages from a connected network.
 * (eg. receive instructions from a Controller and dispatches it to
 * the MicroControllerUnit.
 *
 **************************************************************************************/
class NetworkAdapter
{
public:
    NetworkAdapter();
    virtual ~NetworkAdapter();

public:
    /**********************************************************************************
     * @brief Attemps to connect to a Network with the provided address.
     **********************************************************************************/
    bool connectWithAddress(const std::shared_ptr<Network> &network, int address);

    /**********************************************************************************
     * @brief Attempts to connect to a Network. Address is acquired from the network.
     **********************************************************************************/
    bool connect(const std::shared_ptr<Network> &network);

    /**********************************************************************************
     * @brief Disconnects from the network if it was connected to one.
     **********************************************************************************/
    void disconnect();

    /**********************************************************************************
     * @brief Attempts to send a message to another NetworkAdapter with the provided
     * address.
     **********************************************************************************/
    bool send(std::unique_ptr<AbstractNetworkMessage> &&message, int recipientAddress);

    /***********************************************************************************
     * @brief A received message is pushed into the adapter's message queue.
     ***********************************************************************************/
    void receive(std::unique_ptr<AbstractNetworkMessage> &&message);

    /***********************************************************************************
     * @brief Returns and removes the first Message from the message queue.
     ***********************************************************************************/
    std::unique_ptr<AbstractNetworkMessage> poll();

    /************************************************************************************
     * @brief Returns whether the message queue has any messages.
     ************************************************************************************/
    bool isMessageQueueEmpty() const;

    /************************************************************************************
     * @brief Returns the address of the NetworkAdapter. 
     * If it is not connected to a Network it returns 0.
     ************************************************************************************/
    int getAddress() const;

    /************************************************************************************
     * @brief Returns whether the Adapter is connected to a Network.
     ************************************************************************************/
    bool isConnected() const;

private:
    std::queue<std::unique_ptr<AbstractNetworkMessage>> messageQueue;
    std::shared_ptr<Network> network;
    int address;
};

#endif /* NETWORK_ADAPTER__H */