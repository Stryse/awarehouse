#ifndef A_SCHEDULER__H
#define A_SCHEDULER__H

// ########################## FORWARD DECLARATIONS ##########################
class NetworkAdapter;
// ##########################################################################

class AScheduler
{
public:
    virtual void tick(int timeStamp) = 0;
    virtual void doTaskAssignment() = 0;
    virtual const NetworkAdapter &getNetworkAdapter() const = 0;
    virtual NetworkAdapter &getNetworkAdapter() = 0;
};

#endif /* A_SHCEDULER__H */