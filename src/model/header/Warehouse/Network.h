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
    void disconnect(NetworkAdapter& networkAdapter);
    bool sendMessage(std::unique_ptr<NetworkMessage>&& message, int recipientAddress) const;

private:
    int getNewAddress() const;

private:
    std::map<int,NetworkAdapter*> address2adapter;
    std::map<NetworkAdapter*,int> adapter2address;

    static int addressCounter;
};
#endif /* NETWORK__H */