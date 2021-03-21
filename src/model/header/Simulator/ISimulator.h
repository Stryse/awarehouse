#ifndef I_SIMULATOR__H
#define I_SIMULATOR__H

namespace isimulator_util
{
    enum class TickRate
    {
        HALF_SPEED = 2000,
        NORMAL = 1000,
        TWICE = 500
    };
};

template <typename SimType>
class ISimulator
{
public:
    using TickRate = isimulator_util::TickRate;

public:
    virtual ~ISimulator() = default;
    virtual bool isAvailable() const = 0;
    virtual void bindSimTypeData(SimType *) = 0;
    virtual void startSimulation() = 0;
    virtual void stopSimulation() = 0;
    virtual void setTickRate(TickRate) = 0;
    virtual void setWarehouseStateAt(int timeStamp) = 0;
};
#endif /* I_SIMULATOR__H */
