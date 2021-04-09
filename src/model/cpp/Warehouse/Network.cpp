#include "Network.h"
#include "NetworkAdapter.h"

Network::Network()
    : addressCounter(100)
{
}

Network::~Network()
{
    for (auto &adapter : adapter2address)
        adapter.first->disconnect();
}

int Network::connect(NetworkAdapter &networkAdapter)
{
    auto it = adapter2address.find(&networkAdapter);
    if (it == adapter2address.end())
    {
        int address = getNewAddress();
        adapter2address.insert({&networkAdapter, address});
        address2adapter.insert({address, &networkAdapter});
        return address;
    }
    return 0;
}

bool Network::connectWithAddress(NetworkAdapter &networkAdapter, int address)
{
    auto it = address2adapter.find(address);
    if (it == address2adapter.end())
    {
        adapter2address.insert({&networkAdapter, address});
        address2adapter.insert({address, &networkAdapter});
        return true;
    }
    return false;
}

void Network::disconnect(NetworkAdapter &networkAdapter)
{
    auto it = adapter2address.find(&networkAdapter);
    if (it != adapter2address.end())
    {
        int address = adapter2address[&networkAdapter];
        adapter2address.erase(&networkAdapter);
        address2adapter.erase(address);
    }
}

bool Network::sendMessage(std::unique_ptr<AbstractNetworkMessage> &&message, int recipientAddress) const
{
    auto recipient = address2adapter.find(recipientAddress);
    if (recipient != address2adapter.end())
    {
        recipient->second->receive(std::move(message));
        return true;
    }
    return false;
}

int Network::getNewAddress()
{
    return addressCounter++;
}

void Network::reset()
{
    for (auto &adapter : adapter2address)
        adapter.first->disconnect();

    adapter2address.clear();
    address2adapter.clear();
    addressCounter = 100;
}