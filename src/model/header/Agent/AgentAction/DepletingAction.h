#ifndef DEPLETING_ACTION__H
#define DEPLETING_ACTION__H

#include "AgentAction.h"
#include "IDepleting.h"

/********************************************************************************
 * @brief A kind of Agent Action that needs energy to be performed.
 * It is neccessary to provide an Energy resource of IDepleting interface to
 * consume the amount of energy needed.
 *
 ********************************************************************************/
class DepletingAction : public AgentAction
{
public:
    using EnergyDepletedException = typename IDepleting::EnergyDepletedException;

public:
    DepletingAction(IDepleting &resource, const int &energyCost, int duration)
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
    int energyCost;
};

#endif /* DEPLETING_ACTION__H */
