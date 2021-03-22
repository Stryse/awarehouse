#ifndef POD_DOCK__H
#define POD_DOCK__H

#include "IContaining.h"
#include "LibConfig.h"
#include "Tile.h"
#include <memory>
#include <stdexcept>

// ######################### Forward Declarations ##########################
template <typename TItemType, typename TEnvironment>
class Pod;
class Order;
// #########################################################################

/***********************************************************************************
 * @brief This entity is a type of tile (also NavigationalVolume,AgentSignalHandler)
 * that can store up to one pod.
 * You can add an associated pod 
 * with method @code void addAssociatedPod(std::shared_ptr<Environment> &env) @endcode
 * and it automatically sets this Dock as the freshly created pod's parent.
 ***********************************************************************************/
template <typename TEnvironment = config::navigation::DefaultEnvironment>
class PodDock : public Tile,
                public IContaining<std::unique_ptr<Pod<Order, TEnvironment>>>
{
public:
    using Environment = TEnvironment;
    using Point = Tile::Point;
    using OccupantType = Tile::OccupantType;
    using Pod_Order = Pod<Order, Environment>;
    using OwnedPod = std::unique_ptr<Pod_Order>;

public:
    explicit PodDock(const Point &pos)
        : Tile(pos)
    {
    }

public:
    /***************************************************************************************
     * @brief Don't use this function because unique pointer cant be reassigned without move
     ***************************************************************************************/
    virtual void push(const OwnedPod &item) override
    {
        throw std::runtime_error("PodDock::push(const ItemType &item) is not supported please use PodDock::push(const ItemType &&item)");
    }

    /**************************************************************
     * @brief The Dock acquires the provided pod
     * item will be set to nullptr
     * @throws runtime error if Dock already has a pod
     **************************************************************/
    virtual void push(OwnedPod &&item) override
    {
        if (childPod == nullptr)
            childPod.swap(item);
        else
            throw std::runtime_error("PodDock should only have one child pod");
    }

    /******************************************************************
     * @brief Removes and returns a pod if the holder has one.
     * std::nullopt returned when there's no associated pod.
     ******************************************************************/
    virtual std::optional<OwnedPod> pop(const OwnedPod &item) override
    {
        return (childPod == nullptr) ? std::nullopt : std::optional(std::move(childPod));
    }

    /**************************************************************
     * @brief Returns whether the PodHolder has an associated pod
     **************************************************************/
    virtual bool empty() const override
    {
        return childPod == nullptr;
    }

    // IAgentSignalHandler Implementation
    virtual void receive(IContaining<OwnedPod> &carrier, const PickupPodSignal &pickupSignal) override
    {
        if (childPod != nullptr)
            carrier.push(std::move(this->childPod));
    }

    // Functionality
    void addAssociatedPod(std::shared_ptr<Environment> &env)
    {
        if (childPod == nullptr)
            childPod = std::make_unique<Pod_Order>(this->getPosition(), env, *this);
        else
            throw std::runtime_error("Dock already has associated pod");
    }

public:
    const OwnedPod &getChildPod() const { return childPod; }
    OwnedPod &getChildPod() { return childPod; }

private:
    OwnedPod childPod;
};

#endif