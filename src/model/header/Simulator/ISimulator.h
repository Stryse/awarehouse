#ifndef I_SIMULATOR__H
#define I_SIMULATOR__H

#include "boost/signals2.hpp"

template <typename SimType>
class ISimulator
{

public:
    virtual ~ISimulator() = default;
    virtual bool isAvailable() const = 0;
    virtual void bindSimTypeData(SimType *) = 0;
    virtual void startSimulation() = 0;
    virtual void stopSimulation() = 0;
    virtual void setTickRate(int tickRate) = 0;
    virtual void setWarehouseStateAt(int timeStamp) = 0;

public:
    mutable boost::signals2::signal<void()> onTick;

};
#endif /* I_SIMULATOR__H */
