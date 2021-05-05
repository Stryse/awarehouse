#ifndef I_AGENT_SIGNAL_HANDLER__H
#define I_AGENT_SIGNAL_HANDLER__H

#include "AgentSignals.h"
#include <memory>
#include <set>
// #################### FORWARD DECLARATIONS ######################
template <typename TItemType>
class IContaining;

class IDepleting;

template <typename TItemType>
class Pod;

class OrderModel;

template <typename T>
class pointer_element_comparator;
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
    virtual void receive(IDepleting &resource, const ChargeSignal &chargeSignal) const {}
    virtual void receive(IContaining<OwnedPod> &carrier, const PickupPodSignal &pickupSignal) {}
    virtual void receive(OwnedPod &pod, const PutDownPodSignal &putdownSignal) {}
    virtual void receive(std::set<std::unique_ptr<OrderModel>, pointer_element_comparator<OrderModel>> &orderInventory, const PutDownOrderSignal &putdownOrderSignal) {}
    virtual void receive(const ClaimChStationSignal&) {}
    virtual void receive(const UnClaimChStationSignal&) {}

protected:
    IAgentSignalHandler() = default;
};

#endif /* I_AGENT_SIGNAL_HANDLER__H */
