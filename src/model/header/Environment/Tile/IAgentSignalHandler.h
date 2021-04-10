#ifndef I_AGENT_SIGNAL_HANDLER__H
#define I_AGENT_SIGNAL_HANDLER__H

#include "Agent.h"
#include "AgentSignals.h"
#include "IContaining.h"
#include "LibConfig.h"
#include "OrderModel.h"
#include "Pod.h"
#include <memory>
// #################### FORWARD DECLARATIONS ######################
template <typename TEnergy>
class IDepleting;
// ################################################################

/**************************************************************************
 * @brief An Interface which is responsible for reacting to agent signals.
 **************************************************************************/
template <typename Environment>
class IAgentSignalHandler
{
public:
    virtual ~IAgentSignalHandler() = default;
    virtual void receive(IDepleting<config::agent::DefaultEnergy> &resource, const ChargeSignal &chargeSignal) const {}
    virtual void receive(IContaining<std::unique_ptr<Pod<OrderModel, Environment>>> &carrier, const PickupPodSignal &pickupSignal) {}

protected:
    IAgentSignalHandler() = default;
};

#endif /* I_AGENT_SIGNAL_HANDLER__H */
