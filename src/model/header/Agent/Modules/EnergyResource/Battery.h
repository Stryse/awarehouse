#ifndef BATTERY__H
#define BATTERY__H

#include "IDepleting.h"

template <typename TEnergy = int>
class Battery : public IDepleting<TEnergy>
{
public:
    using Energy = TEnergy;

private:
    /**************************************************************
     * @brief Maximum amount of energy that the resource can store.
     *
     **************************************************************/
    Energy maxCharge;

    /********************************************************************
     * @brief The amount of energy that the resource is currently storing.
     *
     ********************************************************************/
    Energy currentCharge;

    /********************************************************************
     * @brief The maximum amount of energy that the battery can be charged
     * one tick.
     *
     ********************************************************************/
    Energy maxChargeRate;

    /**************************************************************
     * @brief The number of times that the resource is used.
     * The resource degrade proportionally to this quantity.
     *
     **************************************************************/
    int timesUsed;

    /***********************************************************************
     * @brief The percentage of the maximum energy that the resource is now
     * capable of storing.
     *
     ***********************************************************************/
    double condition;

public:
    /************************************************************************
     * @brief Constructs a battery which can be used as an energy resoure.
     * 
     * @param maxCharge The maximum amound of energy the battery can hold
     *  in perfect condition.
     * @param timesUsed The times the battery was used. (default = brand new)
     *
     ************************************************************************/
    explicit Battery(const Energy &maxCharge, const Energy &maxChargeRate = 1, int timesUsed = 0)
        : maxCharge(maxCharge),
          maxChargeRate(maxChargeRate),
          currentCharge(maxCharge),
          timesUsed(timesUsed)
    {
        if (maxCharge < 0)
            this->maxCharge = 0;

        condition = 1; // TODO FIX WITH FACTORY
        this->maxCharge *= condition;
    }

    /*********************************************************
     * @brief The resource is refilled by the provided amount
     * of energy.
     *
     * This action counts as usage so it degrades the
     * battery if the battery has degradation policy.
     *
     ********************************************************/
    virtual void charge(const Energy &energy) override
    {
        if (currentCharge + energy >= maxCharge)
            currentCharge = maxCharge;
        else
            currentCharge += energy;

        condition = degrade(++timesUsed);
        maxCharge *= condition;
    }

    /*****************************************************
     * @brief The resource is depleted due to usage.
     * This action counts as usage so it degrades the
     * battery if the battery has degradation policy.
     *
     * @throws EnergyDepletedException is thrown when
     * there's not enough energy in the battery.
     *
     *****************************************************/
    virtual void deplete(const Energy &energy) override
    {
        if (currentCharge - energy < 0)
        {
            currentCharge = 0;
            throw typename IDepleting<Energy>::EnergyDepletedException("No energy left in battery.");
        }
        else
            currentCharge -= energy;

        condition = degrade(++timesUsed);
        maxCharge *= condition;
    }

    /*******************************************************
     * @brief This Battery has no degradation.
     *
     *******************************************************/
    virtual double degrade(int timesUsed) const override
    {
        return IDepleting<Energy>::NoDegradation;
    }

    // ############################### GETTER ####################################
    virtual const Energy &getCharge() const override { return currentCharge; }
    virtual const Energy &getMaxCharge() const override { return maxCharge; }
    virtual int getTimesUsed() const override { return timesUsed; }
    virtual double getCondition() const override { return condition; }
    virtual const Energy &getMaxChargeRate() const override { return maxChargeRate; }
    // ###########################################################################
};
#endif /* BATTERY__H */
