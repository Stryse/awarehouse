#ifndef POD__H
#define POD__H

#include "Body.h"
#include "BodyShapeFactory.h"
#include "IContaining.h"
#include "LibConfig.h"
#include <functional>
#include <memory>
#include <set>
#include <stdexcept>

// ################### Forward Declarations ######################
template <typename TEnvironment>
class PodDock;
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
template <typename TItemType,
          typename TEnvironment = config::navigation::DefaultEnvironment>

class Pod : public IContaining<std::unique_ptr<TItemType>>
{
public:
    using Environment = TEnvironment;
    using Body = Body<Environment>;
    using Point = typename Body::Point;
    using DirectionVector = typename Body::DirectionVector;
    using PodDock = PodDock<Environment>;
    using ItemType = std::unique_ptr<TItemType>;

public:
    Pod(const Point &position,
        const std::shared_ptr<Environment> &environment,
        const PodDock &parentDock)
        : body(position, DirectionVector::ABOVE(), environment, BodyShapeFactory<Point>::twoBlockHeigh()),
          parentDock(parentDock)
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

private:
    Body body;
    std::set<ItemType, pointer_element_comparator<TItemType>> inventory;
    const PodDock &parentDock;
};

#endif /* POD__H */