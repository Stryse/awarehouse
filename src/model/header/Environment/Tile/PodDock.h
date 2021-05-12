#ifndef POD_DOCK__H
#define POD_DOCK__H

#include "IContaining.h"
#include "PodHolder.h"
#include "Tile.h"
#include <memory>
#include <stdexcept>

// ######################### Forward Declarations ##########################
template <typename TItemType>
class Pod;
class OrderModel;
class ObservableNavEnvironment;
// #########################################################################

/***********************************************************************************
 * @brief This entity is a type of tile (also NavigationalVolume,AgentSignalHandler)
 * that can store up to one pod.
 * You can add an associated pod with method 
 * @code void addAssociatedPod(std::shared_ptr<Environment> &env) @endcode
 * and it automatically sets this Dock as the freshly created pod's parent.
 ***********************************************************************************/
class PodDock : public Tile
{
public:
    using Point = Tile::Point;
    using OccupantType = Tile::OccupantType;
    using Pod_Order = Pod<OrderModel>;
    using OwnedPod = std::unique_ptr<Pod_Order>;

public:
    explicit PodDock(const Point &pos);
    explicit PodDock(PodDock &&other);
    explicit PodDock(const PodDock &other) = delete;
    virtual ~PodDock();
    PodDock &operator=(const PodDock &other) = delete;

public:
    // ################## IAgentSignalHandler Implementation ########################
    /********************************************************************************
     * @brief The PodDock releases its child pod and gives it to another pod carrier.
     ********************************************************************************/
    virtual void receive(IContaining<OwnedPod> &carrier,
                         const PickupPodSignal &pickupSignal) override;

    /**********************************************************************************
     * @brief Takes the pod from the carrier and puts it inside the PodDock's pod holder.
     **********************************************************************************/
    virtual void receive(OwnedPod &pod, const PutDownPodSignal &putdownSignal) override;

    // ############################## Functionality ##################################
    /*********************************************************************************
     * @brief Adds an associated pod and places it on top of this
     * podDock in the environment in which the dock resides.
     *********************************************************************************/
    void addAssociatedPod(std::shared_ptr<ObservableNavEnvironment> &env);

public:
    // ################################# Getter ######################################/
    const PodHolder &getPodHolder() const;
    PodHolder &getPodHolder();

private:
    PodHolder podHolder;
};

#endif