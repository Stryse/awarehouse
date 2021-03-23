#ifndef I_SIMULATOR__H
#define I_SIMULATOR__H

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
};
#endif /* I_SIMULATOR__H */
