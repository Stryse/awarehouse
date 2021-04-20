#ifndef POD__H
#define POD__H

#include "Body.h"
#include "BodyShapeFactory.h"
#include "IContaining.h"
#include "ObservableEnvironment.h"
#include "OrderModel.h"
#include "Tile.h"
#include "boost/signals2.hpp"
#include <functional>
#include <memory>
#include <set>
#include <stdexcept>

// ################### Forward Declarations ######################
class PodDock;
class Body;
class ObservableNavEnvironment;
// ###############################################################

// Ensuring set has unique elements inside unique_ptr ////////////
template <typename T>
struct pointer_element_comparator
{
    bool operator()(const std::unique_ptr<T> &lhs,
                    const std::unique_ptr<T> &rhs) const
    {
        if (!lhs || !rhs)
            return true;
        return *lhs < *rhs;
    }
};

/*************************************************************************
 * @brief An entity which has a body in an environment and can
 * store items in its inventory
 * 
 * @tparam TItemType The type of item that the pod can store.
 * @tparam TEnvironment The type of environment in which the Pod's body
 * resides.
 **************************************************************************/
template <typename TItemType>
class Pod : public IContaining<std::unique_ptr<TItemType>>
{
public:
    using Point = Body::Point;
    using DirectionVector = Body::DirectionVector;
    using ItemType = std::unique_ptr<TItemType>;

public:
    boost::signals2::signal<void(const Body &)> onBodyMoved;

public:
    Pod(const Point &position,
        const std::shared_ptr<ObservableNavEnvironment> &environment,
        const PodDock &parentDock)
        : body(std::make_unique<Body>(position, DirectionVector::UP(), environment, BodyShapeFactory<Point>::onlyOrigin())),
          parentDock(parentDock)
    {
    }

    Pod(const Pod &other) = delete;

    Pod &operator=(const Pod &other) = delete;

    Pod(Pod &&other)
        : body(std::move(other.body)),
          inventory(std::move(other.inventory)),
          parentDock(std::move(other.parentDock))
    {
    }

    virtual ~Pod() {}

public:
    // ##################### IContaining Interface implementation ##########################
    /***************************************************************************************
     * @brief Don't use this function because unique pointer cant be reassigned without move
     ***************************************************************************************/
    virtual void push(const ItemType &item) override
    {
        throw std::runtime_error("Pod::push(const ItemType &item) is not supported please use Pod::push(const ItemType &&item)");
    }

    /**********************************************************************
     * @brief Places an item on the pod. Each item must be unique
     * Invalidates &&item
     **********************************************************************/
    virtual void push(ItemType &&item) override { inventory.insert(std::move(item)); }

    /**********************************************************************
     * @brief Returns an item which satisfies the provided condition
     **********************************************************************/
    virtual std::optional<ItemType> pop(const ItemType &item) override { return std::nullopt; }

    /***********************************************************************
     * @brief Returns whether the Pod has items
     ***********************************************************************/
    virtual bool empty() const override { return inventory.empty(); }

    /************************************************************************
     * @brief Returns the pods parentDock to which it shall be
     * carried back.
     ************************************************************************/
    const PodDock &getParentDock() const { return parentDock; }

    /************************************************************************
     * @brief Returns the Pod's inventory
     ************************************************************************/
    const std::set<ItemType, pointer_element_comparator<TItemType>> &getInventory() const
    {
        return inventory;
    }

    /************************************************************************
     * @brief Returns the body of the pod
     ************************************************************************/
    const Body *getBody() const { return body.get(); }
    Body *getBody() { return body.get(); }

private:
    std::unique_ptr<Body> body;
    std::set<ItemType, pointer_element_comparator<TItemType>> inventory;
    const PodDock &parentDock;
};

#endif /* POD__H */