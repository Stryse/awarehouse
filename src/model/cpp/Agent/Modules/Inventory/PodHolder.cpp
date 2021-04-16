#include "PodHolder.h"
#include "OrderModel.h"
#include "Pod.h"

void PodHolder::push(const std::unique_ptr<Pod<OrderModel>> &)
{
    throw std::runtime_error("PodHolder::push(const ItemType &item) is not supported please use PodHolder::push(const ItemType &&item)");
}

void PodHolder::push(std::unique_ptr<Pod<OrderModel>> &&item)
{
    if (pod == nullptr)
        pod.swap(item);
    else
        throw std::runtime_error("PodHolder should only have one pod");
}

std::optional<std::unique_ptr<Pod<OrderModel>>> PodHolder::pop(const std::unique_ptr<Pod<OrderModel>> &)
{
    if (pod)
        return std::make_optional<std::unique_ptr<Pod<OrderModel>>>(std::move(pod));
    else
        return std::nullopt;
}

bool PodHolder::empty() const
{
    return pod == nullptr;
}

const std::unique_ptr<Pod<OrderModel>> &PodHolder::getChildPod() const { return pod; }
std::unique_ptr<Pod<OrderModel>> &PodHolder::getChildPod() { return pod; }