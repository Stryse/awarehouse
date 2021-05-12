#ifndef NETWORK__H
#define NETWORK__H

#include <map>
#include <memory>

// ################# FORWARD DECLARATIONS #####################
class NetworkAdapter;
class AbstractNetworkMessage;
// ############################################################

/**************************************************************
 * @brief Entities with network adapter can connect to a network
 * and can send AbstractNetworkMessage subclass objects to each
 * other through this network.
 * They receive a address (a number (int)) which identifies a
 * connected network adapter and messages can be sent by knowing
 * only the target addresses.
 **************************************************************/
class Network
{
public:
    Network();
    virtual ~Network();

public:
    /**********************************************************************
     * @brief Registers an adapter in the network and gives it an address.
     **********************************************************************/
    int connect(NetworkAdapter &networkAdapter);

    /**********************************************************************
     * @brief Attempts to register an adapter in the network with the provided
     * address. Succeeds if address is not already claimed.
     **********************************************************************/
    bool connectWithAddress(NetworkAdapter &networkAdapter, int address);

    /**********************************************************************
     * @brief Removes a network adapter from the set of registered adapters
     * thus disconnecting it from the network.
     **********************************************************************/
    void disconnect(NetworkAdapter &networkAdapter);

    /**********************************************************************************
     * @brief Attempts to send a message to the adapter which has the provided address
     **********************************************************************************/
    bool sendMessage(std::shared_ptr<AbstractNetworkMessage> message, int recipientAddress) const;

    /********************************************************
     * @brief Disconnects all adapters from the network.
     ********************************************************/
    void reset();

private:
    /***************************************************************
     * @brief Returns an address for a freshly connecting adapter.
     ***************************************************************/
    int getNewAddress();

private:
    std::map<int, NetworkAdapter *> address2adapter;
    std::map<NetworkAdapter *, int> adapter2address;

    int addressCounter;
};
#endif /* NETWORK__H */