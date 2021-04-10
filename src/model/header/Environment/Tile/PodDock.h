#ifndef POD_DOCK__H
#define POD_DOCK__H

#include "IContaining.h"
#include "LibConfig.h"
#include "PodHolder.h"
#include "Tile.h"
#include <memory>
#include <stdexcept>

// ######################### Forward Declarations ##########################
template <typename TItemType, typename TEnvironment>
class Pod;
class OrderModel;
// #########################################################################

/***********************************************************************************
 * @brief This entity is a type of tile (also NavigationalVolume,AgentSignalHandler)
 * that can store up to one pod.
 * You can add an associated pod with method 
 * @code void addAssociatedPod(std::shared_ptr<Environment> &env) @endcode
 * and it automatically sets this Dock as the freshly created pod's parent.
 ***********************************************************************************/
template <typename TEnvironment = config::navigation::DefaultEnvironment>
class PodDock : public Tile
{
public:
    using Environment = TEnvironment;
    using Point = Tile::Point;
    using OccupantType = Tile::OccupantType;
    using Pod_Order = Pod<OrderModel, Environment>;
    using OwnedPod = std::unique_ptr<Pod_Order>;

public:
    explicit PodDock(const Point &pos)
        : Tile(pos)
    {
    }

    virtual ~PodDock() {}

public:
    // IAgentSignalHandler Implementation
    /********************************************************************************
     * @brief The PodDock releases its child pod and gives it to another pod carrier.
     ********************************************************************************/
    virtual void receive(IContaining<OwnedPod> &carrier, const PickupPodSignal &pickupSignal) override
    {
        if (podHolder.getChildPod() != nullptr)
            carrier.push(std::move(getPodHolder().getChildPod()));
    }

    // Functionality
    /*************************************************************
     * @brief Adds an associated pod and places it on top of this
     * podDock in the environment in which the dock resides.
     *************************************************************/
    void addAssociatedPod(std::shared_ptr<Environment> &env)
    {
        if (podHolder.getChildPod() == nullptr)
            podHolder.getChildPod() = std::make_unique<Pod_Order>(this->getPosition().moved(DirectionVector<>(0, 0, 1)), env, *this);
        else
            throw std::runtime_error("Dock already has associated pod");
    }

public:
    const PodHolder<Environment> &getPodHolder() const { return podHolder; }
    PodHolder<Environment> &getPodHolder() { return podHolder; }

private:
    PodHolder<Environment> podHolder;
};

#endif