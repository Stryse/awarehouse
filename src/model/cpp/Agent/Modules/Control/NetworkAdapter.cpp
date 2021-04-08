#include "NetworkAdapter.h"
#include "Network.h"

NetworkAdapter::NetworkAdapter()
    : network(nullptr), address(0)
{
}

NetworkAdapter::~NetworkAdapter()
{
    disconnect();
}

bool NetworkAdapter::send(std::unique_ptr<NetworkMessage>&& message, int recipientAddress)
{
    if(network)
        return network->sendMessage(std::move(message),recipientAddress);

    return false;
}

void NetworkAdapter::receive(std::unique_ptr<NetworkMessage>&& message)
{
    messageQueue.push(std::move(message));
}

std::unique_ptr<NetworkMessage> NetworkAdapter::poll()
{
    std::unique_ptr<NetworkMessage> message = std::move(messageQueue.front());
    messageQueue.pop();
    return std::move(message);
}

bool NetworkAdapter::connect(const std::shared_ptr<Network>& network)
{
    if(this->address = network->connect(*this))
    {
        this->network = network;
        return true;
    }
    return false;
}

bool NetworkAdapter::connectWithAddress(const std::shared_ptr<Network>& network, int address)
{
    if(network->connectWithAddress(*this,address))
    {
        this->network = network;
        return true;
    }
    return false;
}

void NetworkAdapter::disconnect()
{
    if(network)
    {
        network->disconnect(*this);
        address = -1;
        network = nullptr;
    }
}

int NetworkAdapter::getAddress() const
{
    return address;
}

bool NetworkAdapter::isConnected() const
{
    return network.get() != nullptr;
}