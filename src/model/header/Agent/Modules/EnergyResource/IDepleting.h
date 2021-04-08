#ifndef I_DEPLETING__H
#define I_DEPLETING__H

#include <stdexcept>

namespace idepleting_util
{
    /*****************************************************
     * @brief Thrown when a resource is tried to be used
     * when it is already depleted enough.
     *
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
template <typename TEnergy>
class IDepleting
{
public:
    using Energy = TEnergy;
    using EnergyDepletedException = idepleting_util::EnergyDepletedException;

public:
    virtual void charge(const Energy &energy) = 0;
    virtual void deplete(const Energy &energy) = 0;
    virtual double degrade(int timesUsed) const = 0;
    virtual const Energy &getCharge() const = 0;
    virtual const Energy &getMaxCharge() const = 0;
    virtual const Energy &getMaxChargeRate() const = 0;
    virtual int getTimesUsed() const = 0;
    virtual double getCondition() const = 0;
    virtual bool isDepleted() const { return getCharge() < 1e-8f; }

    static constexpr double NoDegradation = 1;
};
#endif /* I_DEPLETING__H */
