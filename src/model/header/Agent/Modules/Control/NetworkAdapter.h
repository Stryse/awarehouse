#ifndef NETWORK_ADAPTER__H
#define NETWORK_ADAPTER__H

#include <queue>
#include <memory>

// ################## FORWARD DECLARATIONS #####################
class NetworkMessage;
class Network;
// #############################################################

class NetworkAdapter
{
public:
    NetworkAdapter();
    virtual ~NetworkAdapter();

public:
    bool connect(Network& network);
    void disconnect();
    bool send(std::unique_ptr<NetworkMessage>&& message, int recipientAddress);
    void receive(std::unique_ptr<NetworkMessage>&& message);
    std::unique_ptr<NetworkMessage> poll();
    int getAddress() const;
    bool isConnected() const;

private:
    std::queue<std::unique_ptr<NetworkMessage>> messageQueue;
    Network* network;
    int address;
};

#endif /* NETWORK_ADAPTER__H */ 