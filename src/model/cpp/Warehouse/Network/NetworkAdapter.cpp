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

bool NetworkAdapter::send(std::unique_ptr<AbstractNetworkMessage> &&message, int recipientAddress)
{
    if (network)
        return network->sendMessage(std::move(message), recipientAddress);

    return false;
}

void NetworkAdapter::receive(std::unique_ptr<AbstractNetworkMessage> &&message)
{
    messageQueue.push(std::move(message));
}

std::unique_ptr<AbstractNetworkMessage> NetworkAdapter::poll()
{
    std::unique_ptr<AbstractNetworkMessage> message = std::move(messageQueue.front());
    messageQueue.pop();
    return std::move(message);
}

bool NetworkAdapter::connect(const std::shared_ptr<Network> &network)
{
    if (this->address = network->connect(*this))
    {
        this->network = network;
        return true;
    }
    return false;
}

bool NetworkAdapter::connectWithAddress(const std::shared_ptr<Network> &network, int address)
{
    if (network->connectWithAddress(*this, address))
    {
        this->network = network;
        this->address = address;
        return true;
    }
    return false;
}

void NetworkAdapter::disconnect()
{
    if (network)
    {
        network->disconnect(*this);
        address = 0;
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

bool NetworkAdapter::isMessageQueueEmpty() const
{
    return messageQueue.empty();
}