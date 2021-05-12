#ifndef I_DEPLETING__H
#define I_DEPLETING__H

#include <stdexcept>
#include "boost/signals2.hpp"

namespace idepleting_util
{
    /*****************************************************
     * @brief Thrown when a resource is tried to be used
     * when it is already depleted enough.

     *****************************************************/
    class EnergyDepletedException : public std::runtime_error
    {
    public:
        explicit EnergyDepletedException(const char *message) : std::runtime_error(message) {}
    };
};

/*********************************************************
 * @brief Interface of a depleting energy resource.
 *
 *********************************************************/
class IDepleting
{
public:
    using EnergyDepletedException = idepleting_util::EnergyDepletedException;

public:
    // ###################### SIGNALS ###########################
    /***********************************************************
     * @brief Notifies currentCharge value changes
     ***********************************************************/
    mutable boost::signals2::signal<void(int)> onChargeChanged;

    /***********************************************************
     * @brief Notifies depletion.
     ***********************************************************/
    mutable boost::signals2::signal<void(int)> onDepleted;

public:
    virtual void charge(int energy) = 0;
    virtual void deplete(int energy) = 0;
    virtual double degrade(int timesUsed) const = 0;
    virtual int getCharge() const = 0;
    virtual int getMaxCharge() const = 0;
    virtual int getMaxChargeRate() const = 0;
    virtual int getTimesUsed() const = 0;
    virtual double getCondition() const = 0;
    virtual bool isDepleted() const { return getCharge() < 1e-8f; }

    static constexpr double NoDegradation = 1;
};
#endif /* I_DEPLETING__H */
