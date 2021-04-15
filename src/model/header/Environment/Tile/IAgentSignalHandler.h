#ifndef I_AGENT_SIGNAL_HANDLER__H
#define I_AGENT_SIGNAL_HANDLER__H

#include "AgentSignals.h"
#include <memory>
// #################### FORWARD DECLARATIONS ######################
template <typename TItemType>
class IContaining;

template <typename TEnergy>
class IDepleting;

template <typename TItemType>
class Pod;

class OrderModel;
// ################################################################

/**************************************************************************
 * @brief An Interface which is responsible for reacting to agent signals.
 **************************************************************************/
class IAgentSignalHandler
{
public:
    using OwnedPod = std::unique_ptr<Pod<OrderModel>>;
    using OwnedOrder = std::unique_ptr<OrderModel>;

public:
    virtual ~IAgentSignalHandler() = default;
    virtual void receive(IDepleting<int> &resource, const ChargeSignal &chargeSignal) const {}
    virtual void receive(IContaining<OwnedPod> &carrier, const PickupPodSignal &pickupSignal) {}
    virtual void receive(OwnedPod &pod, const PutDownPodSignal &putdownSignal) {}
    virtual void receive(OwnedOrder &order, const PutDownOrderSignal &putdownOrderSignal) {}

protected:
    IAgentSignalHandler() = default;
};

#endif /* I_AGENT_SIGNAL_HANDLER__H */
