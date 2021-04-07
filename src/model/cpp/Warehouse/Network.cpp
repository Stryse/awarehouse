#include "Network.h"
#include "NetworkAdapter.h"

Network::~Network()
{
    for(auto& adapter : adapter2address)
        adapter.first->disconnect();
}

int Network::connect(NetworkAdapter& networkAdapter)
{
    auto it = adapter2address.find(&networkAdapter);
    if(it == adapter2address.end())
    {
        int address = getNewAddress();
        adapter2address.insert({&networkAdapter,address});
        address2adapter.insert({address,&networkAdapter});
        return address;
    }
    return 0;
}

void Network::disconnect(NetworkAdapter& networkAdapter)
{
    auto it = adapter2address.find(&networkAdapter);
    if(it != adapter2address.end())
    {
        int address = adapter2address[&networkAdapter];
        adapter2address.erase(&networkAdapter);
        address2adapter.erase(address);
    }
}

bool Network::sendMessage(std::unique_ptr<NetworkMessage>&& message, int recipientAddress) const
{
    auto recipient = address2adapter.find(recipientAddress);
    if(recipient != address2adapter.end())
    {
        recipient->second->receive(std::move(message));
        return true;
    }
    return false;
}

int Network::getNewAddress() const
{
    return Network::addressCounter++;
}

int Network::addressCounter = 100;