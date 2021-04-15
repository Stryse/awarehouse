#include "Body.h"

Body::Body(const Body::Point &position,
           const Body::DirectionVector &orientation,
           const std::shared_ptr<ObservableNavEnvironment> &environment,
           std::vector<Body::Point> &&bodyShape)

    : pose(position, orientation), shape(std::move(bodyShape)), parentBody(nullptr), environment(environment)
{
    //Place body in environment on creation.
    occupyV(environment->getVolume(position));
}

Body::~Body()
{
    // Destroy child record in parent
    if (parentBody != nullptr)
        parentBody->detachBody(*this);
    // Destroy parent record in child
    for (auto &child : childBodies)
        child->parentBody = nullptr;
    childBodies.clear();
    // Leave Environment, children remain
    Body::freeV();
}

void Body::occupyV(const std::shared_ptr<Tile> &originVolume)
{
    freeV();

    // Self origin - occupation
    originVolume->occupyV(this);
    containerVolume.push_back(originVolume);

    // Self bodyparts -- occupation
    for (auto &bodypart : shape)
    {
        Body::Point newPos = originVolume->getPosition().moved(Body::DirectionVector(bodypart));
        environment->getVolume(newPos)->occupyV(this);
        containerVolume.push_back(environment->getVolume(newPos));
    }

    // All children - occupation
    for (auto &child : childBodies)
    {
        Body::DirectionVector this2child = child->getPose().getPosition() - pose.getPosition();
        Body::Point newChildPos = originVolume->getPosition().moved(this2child);
        child->occupyV(environment->getVolume(newChildPos));
    }
    pose.setPosition(originVolume->getPosition());
}

void Body::freeV()
{
    // Free children
    for (auto &child : childBodies)
        child->freeV();

    // Free self
    for (auto &occupied : containerVolume)
        occupied->freeV();

    containerVolume.clear();
}

void Body::moveBody(const Body::DirectionVector &direction)
{
    occupyV(environment->getVolume(pose.getPosition().moved(direction)));
}

void Body::moveBodyOutsideEnvironment(const Body::DirectionVector &direction)
{
    Body::Point newOriginPos = pose.getPosition().moved(direction);

    //Move children
    for (auto &child : childBodies)
    {
        Body::DirectionVector this2child = child->getPose().getPosition() - pose.getPosition();
        child->getPose().setPosition(newOriginPos.moved(this2child));
    }
    //Move origin
    pose.setPosition(newOriginPos);
}

void Body::rotate(const Body::DirectionVector &targetOrientation)
{
    pose.setOrientation(targetOrientation);
}

void Body::attachBody(Body &body)
{
    if (&body == this || body.getParent() != nullptr)
        return;

    childBodies.insert(&body);
    body.parentBody = this;
}

void Body::detachBody(Body &body)
{
    childBodies.erase(&body);
    body.parentBody = nullptr;
}

const Body::Pose &Body::getPose() const { return pose; }
Body::Pose &Body::getPose() { return pose; }

const std::set<Body *> &Body::getChildren() const { return childBodies; }
std::set<Body *> &Body::getChildren() { return childBodies; }

const Body *Body::getParent() const { return parentBody; }
Body *Body::getParent() { return parentBody; }

const std::vector<std::shared_ptr<Tile>> &Body::getContainerVolume() const { return containerVolume; }

const ObservableNavEnvironment &Body::getEnvironment() const { return *environment; }
ObservableNavEnvironment &Body::getEnvironment() { return *environment; }