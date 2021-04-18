#ifndef A_CONTROLLER__H
#define A_CONTROLLER__H

// ######################## FORWARD DECLARATIONS ##########################
class NetworkAdapter;
// ########################################################################

class AController
{
public:
    virtual void tick(int timeStamp) = 0;
    virtual const NetworkAdapter &getNetworkAdapter() const = 0;
    virtual NetworkAdapter &getNetworkAdapter() = 0;
};

#endif