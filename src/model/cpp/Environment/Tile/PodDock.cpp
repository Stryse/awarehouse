#include "PodDock.h"

PodDock::PodDock(const PodDock::Point &pos)
    : Tile(pos)
{
}

PodDock::PodDock(PodDock &&other)
    : Tile(std::move(other.getPosition())),
      podHolder(std::move(other.podHolder))
{
}

PodDock::~PodDock() {}

void PodDock::receive(IContaining<PodDock::OwnedPod> &carrier, const PickupPodSignal &pickupSignal)
{
    if (podHolder.getChildPod() != nullptr)
        carrier.push(std::move(getPodHolder().getChildPod()));
}

void PodDock::receive(PodDock::OwnedPod &pod, const PutDownPodSignal &putdownSignal)
{
    if (pod && &(pod->getParentDock()) == this)
        podHolder.push(std::move(pod));
}

void PodDock::addAssociatedPod(std::shared_ptr<ObservableNavEnvironment> &env)
{
    if (podHolder.getChildPod() == nullptr)
        podHolder.getChildPod() = std::make_unique<PodDock::Pod_Order>(this->getPosition().moved(DirectionVector<>(0, 0, 1)), env, *this);
    else
        throw std::runtime_error("Dock already has associated pod");
}

const PodHolder &PodDock::getPodHolder() const { return podHolder; }
PodHolder &PodDock::getPodHolder() { return podHolder; }