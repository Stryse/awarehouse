#include "DepletingAction.h"

DepletingAction::DepletingAction(IDepleting &resource, const int &energyCost, int duration)
    : AgentAction(duration), resource(resource), energyCost(energyCost)
{
}

DepletingAction::~DepletingAction() {}

void DepletingAction::action()
{
    try
    {
        resource.get().deplete(energyCost);
        depletingAction();
    }
    catch (const EnergyDepletedException &excpt)
    {
        throw AgentAction::ActionFailedException("Action failed due to not enough energy in resource");
    }
}