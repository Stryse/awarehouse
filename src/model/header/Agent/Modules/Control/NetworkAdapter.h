#ifndef NETWORK_ADAPTER__H
#define NETWORK_ADAPTER__H

#include <queue>
#include <memory>
#include "NetworkMessage.h"

// ################## FORWARD DECLARATIONS #####################
class Network;
// #############################################################

class NetworkAdapter
{
public:
    NetworkAdapter();
    virtual ~NetworkAdapter();

public:

    bool connectWithAddress(const std::shared_ptr<Network>& network, int address);
    bool connect(const std::shared_ptr<Network>& network);
    void disconnect();
    bool send(std::unique_ptr<NetworkMessage>&& message, int recipientAddress);
    void receive(std::unique_ptr<NetworkMessage>&& message);
    std::unique_ptr<NetworkMessage> poll();
    int getAddress() const;
    bool isConnected() const;

private:
    std::queue<std::unique_ptr<NetworkMessage>> messageQueue;
    std::shared_ptr<Network> network;
    int address;
};

#endif /* NETWORK_ADAPTER__H */ 