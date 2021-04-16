#ifndef BATTERY__H
#define BATTERY__H

#include "IDepleting.h"
#include "boost/signals2.hpp"

class Battery : public IDepleting
{
public:
    // ###################### SIGNALS ###########################
    /***********************************************************
     * @brief Notifies currentCharge value changes
     ***********************************************************/
    boost::signals2::signal<void(int)> onChargeChanged;

private:
    /**************************************************************
     * @brief Maximum amount of energy that the resource can store.
     *
     **************************************************************/
    int maxCharge;

    /********************************************************************
     * @brief The amount of energy that the resource is currently storing.
     *
     ********************************************************************/
    int currentCharge;

    /********************************************************************
     * @brief The maximum amount of energy that the battery can be charged
     * one tick.
     *
     ********************************************************************/
    int maxChargeRate;

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
    explicit Battery(int maxCharge, int maxChargeRate = 1, int timesUsed = 0)
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
    virtual void charge(int energy) override
    {
        if (currentCharge + energy >= maxCharge)
        {
            currentCharge = maxCharge;
            onChargeChanged(currentCharge);
        }
        else
        {
            currentCharge += energy;
            onChargeChanged(currentCharge);
        }

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
    virtual void deplete(int energy) override
    {
        if (currentCharge - energy < 0)
        {
            currentCharge = 0;
            onChargeChanged(currentCharge);
            throw typename IDepleting::EnergyDepletedException("No energy left in battery.");
        }
        else
        {
            currentCharge -= energy;
            onChargeChanged(currentCharge);
        }

        condition = degrade(++timesUsed);
        maxCharge *= condition;
    }

    /*******************************************************
     * @brief This Battery has no degradation.
     *
     *******************************************************/
    virtual double degrade(int timesUsed) const override
    {
        return IDepleting::NoDegradation;
    }

    // ############################### GETTER ####################################
    virtual int getCharge() const override { return currentCharge; }
    virtual int getMaxCharge() const override { return maxCharge; }
    virtual int getTimesUsed() const override { return timesUsed; }
    virtual double getCondition() const override { return condition; }
    virtual int getMaxChargeRate() const override { return maxChargeRate; }
    // ###########################################################################
};
#endif /* BATTERY__H */
