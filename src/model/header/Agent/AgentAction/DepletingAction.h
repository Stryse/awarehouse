#ifndef DEPLETING_ACTION__H
#define DEPLETING_ACTION__H

#include "AgentAction.h"
#include "IDepleting.h"
#include "LibConfig.h"

/********************************************************************************
 * @brief A kind of Agent Action that needs energy to be performed.
 * It is neccessary to provide an Energy resource of IDepleting interface to
 * consume the amount of energy needed.
 *
 ********************************************************************************/
template <typename TEnergy = config::agent::DefaultEnergy>
class DepletingAction : public AgentAction
{
public:
    using Energy = TEnergy;
    using IDepleting = ::IDepleting<Energy>;
    using EnergyDepletedException = typename IDepleting::EnergyDepletedException;

public:
    DepletingAction(IDepleting &resource, const Energy &energyCost, int duration)
        : AgentAction(duration), resource(resource), energyCost(energyCost) {}

protected:
    virtual void action() override
    {
        try
        {
            resource.deplete(energyCost);
            depletingAction();
        }
        catch (const EnergyDepletedException &excpt)
        {
            throw AgentAction::ActionFailedException("Action failed due to not enough energy in resource");
        }
    }

    virtual bool canExecute() const override = 0;
    virtual void depletingAction() = 0;

private:
    IDepleting &resource;
    Energy energyCost;
};

#endif /* DEPLETING_ACTION__H */
