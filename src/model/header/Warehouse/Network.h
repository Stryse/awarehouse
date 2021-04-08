#ifndef NETWORK__H
#define NETWORK__H

#include <map>
#include <memory>

// ################# FORWARD DECLARATIONS #####################
class NetworkAdapter;
class NetworkMessage;
// ############################################################

class Network
{
public:
    Network();
    virtual ~Network();

public:
    int connect(NetworkAdapter& networkAdapter);
    bool connectWithAddress(NetworkAdapter& networkAdapter, int address);
    void disconnect(NetworkAdapter& networkAdapter);
    bool sendMessage(std::unique_ptr<NetworkMessage>&& message, int recipientAddress) const;
    void reset();

private:
    int getNewAddress();

private:
    std::map<int,NetworkAdapter*> address2adapter;
    std::map<NetworkAdapter*,int> adapter2address;

    int addressCounter;
};
#endif /* NETWORK__H */