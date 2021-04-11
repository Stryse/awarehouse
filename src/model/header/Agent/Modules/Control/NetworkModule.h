#ifndef NETWORK_MODULE__H
#define NETWORK_MODULE__H

#include "NetworkAdapter.h"

class NetworkModule
{
public:
    const NetworkAdapter &getNetworkAdapter() const { return networkAdapter; }
    NetworkAdapter &getNetworkAdapter() { return networkAdapter; }

private:
    NetworkAdapter networkAdapter;
};

#endif /* NETWORK_MODULE__H */