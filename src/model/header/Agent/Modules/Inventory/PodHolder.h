#ifndef POD_HOLDER__H
#define POD_HOLDER__H

#include "IContaining.h"
#include "LibConfig.h"
#include "OrderModel.h"
#include "Pod.h"
#include <memory>

// ####################### FORWARD DECLARATIONS ########################
// #####################################################################

/******************************************************************************
 * @brief This entity can store One pod which stores orders.
 * eg. delivery robot has this module so it can pick up and
 * transfer pods and then put them down.
 ******************************************************************************/
template <typename TEnvironment = config::navigation::DefaultEnvironment>
class PodHolder : public IContaining<std::unique_ptr<Pod<OrderModel, TEnvironment>>>
{
public:
    using Environment = TEnvironment;

public:
    PodHolder(){};
    ~PodHolder(){};

public:
    /***************************************************************************************
     * @brief Don't use this function because unique pointer cant be reassigned without move.
     ***************************************************************************************/
    virtual void push(const std::unique_ptr<Pod<OrderModel, TEnvironment>> &) override
    {
        throw std::runtime_error("PodHolder::push(const ItemType &item) is not supported please use PodHolder::push(const ItemType &&item)");
    }

    /**************************************************************
     * @brief The PodHolder acquires the provided pod
     * item will be set to nullptr.
     * @throws runtime error if holder already has a pod.
     **************************************************************/
    virtual void push(std::unique_ptr<Pod<OrderModel, TEnvironment>> &&item) override
    {
        if (pod == nullptr)
            pod.swap(item);
        else
            throw std::runtime_error("PodHolder should only have one pod");
    }

    /******************************************************************
     * @brief Removes and returns a pod if the holder has one.
     * std::nullopt returned when there's no associated pod.
     ******************************************************************/
    virtual std::optional<std::unique_ptr<Pod<OrderModel, TEnvironment>>> pop(const std::unique_ptr<Pod<OrderModel, TEnvironment>> &) override
    {
        if (pod)
            return std::make_optional<std::unique_ptr<Pod<OrderModel, TEnvironment>>>(std::move(pod));
        else
            return std::nullopt;
    }

    /**************************************************************
     * @brief Returns whether the PodHolder has an associated pod
     **************************************************************/
    virtual bool empty() const override
    {
        return pod == nullptr;
    }

    /***************************************************************
     * @brief Returns the associated pod if the holder has one.
     * Returns nullptr if the holder has no pod.
     ***************************************************************/
    const std::unique_ptr<Pod<OrderModel, TEnvironment>> &getChildPod() const { return pod; } // TODO std optional
    std::unique_ptr<Pod<OrderModel, TEnvironment>> &getChildPod() { return pod; }

private:
    std::unique_ptr<Pod<OrderModel, TEnvironment>> pod;
};

#endif /* POD_HOLDER__H */
