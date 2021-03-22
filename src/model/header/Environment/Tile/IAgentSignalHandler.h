#ifndef I_AGENT_SIGNAL_HANDLER__H
#define I_AGENT_SIGNAL_HANDLER__H

#include "Agent.h"
#include "AgentSignals.h"
#include "LibConfig.h"
#include <memory>
// #################### FORWARD DECLARATIONS ######################
template <typename TEnergy>
class IDepleting;
template <typename TItemType>
class IContaining;
template <typename ItemType, typename Environment>
class Pod;
class Order;
// ################################################################

template <typename Environment>
class IAgentSignalHandler
{
public:
    using OwnedPod_Order = std::unique_ptr<Pod<Order, Environment>>;

public:
    virtual void receive(IDepleting<config::agent::DefaultEnergy> &resource, const ChargeSignal &chargeSignal) const {}
    virtual void receive(IContaining<OwnedPod_Order> &carrier, const PickupPodSignal &pickupSignal) const {}
};

#endif /* I_AGENT_SIGNAL_HANDLER__H */