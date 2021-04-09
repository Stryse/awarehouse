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
    bool connectWithAddress(const std::shared_ptr<Network> &network, int address);
    bool connect(const std::shared_ptr<Network> &network);
    void disconnect();
    bool send(std::unique_ptr<AbstractNetworkMessage> &&message, int recipientAddress);
    void receive(std::unique_ptr<AbstractNetworkMessage> &&message);
    std::unique_ptr<AbstractNetworkMessage> poll();
    int getAddress() const;
    bool isConnected() const;

private:
    std::queue<std::unique_ptr<AbstractNetworkMessage>> messageQueue;
    std::shared_ptr<Network> network;
    int address;
};

#endif /* NETWORK_ADAPTER__H */