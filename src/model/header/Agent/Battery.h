#ifndef BATTERY__H
#define BATTERY__H

#include "IDepleting.h"

template <typename TEnergy = int>
class Battery : public IDepleting<TEnergy>
{
public:
    using Energy = TEnergy;

private:
    Energy maxCharge;
    Energy currentCharge;

public:
    explicit Battery(const Energy &maxCharge) : maxCharge(maxCharge), currentCharge(maxCharge)
    {
        if (maxCharge < 0)
            this->maxCharge = 0;
    }

    virtual void charge(const Energy &energy) override
    {
        if (currentCharge + energy >= maxCharge)
            currentCharge = maxCharge;
        else
            currentCharge += energy;
    }

    virtual void deplete(const Energy &energy) override
    {
        if (currentCharge - energy <= 0)
            currentCharge = 0;
        else
            currentCharge -= energy;

        return;
    }

    virtual const Energy &getCharge() const override
    {
        return currentCharge;
    }

    virtual const Energy &getMaxCharge() const override
    {
        return maxCharge;
    }
};
#endif /* BATTERY__H */