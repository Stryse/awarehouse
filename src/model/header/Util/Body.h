#ifndef BODY__H
#define BODY__H

#include "INavigationVolume.h"
#include "IVolumeOccupant.h"
#include "Pose.h"
#include <memory>
#include <set>
#include <vector>

// ################# FORWARD DECLARATIONS ############### //
template <typename VolumeType>
class INavigationalEnvironment;
// #########################################################

/**
 * @brief A connected set of Volume Occupant placed in a 
 * @code INavigationalEnvironment @endcode.
 * 
 * @tparam VolumeType Type of the volume in which the body resides.
 * Must be a pointer type or pointer wrapper with "pointer" member type.
 * Must be derived from @code INavigationVolume<IVolumeOccupant*> @endcode interface.
 */
template <template <class> class Environment, class VolumeType>
class Body : public IVolumeOccupant<VolumeType>
{
public:
    Body(const Point<int> &position,
         const DirectionVector<int> &orientation,
         Environment<VolumeType> &environment)
        : pose(position, orientation), environment(environment)
    {
    }

    virtual ~Body() = default;

    //############################## IVolumeOccupant implementation ##########################################
    /**
     * @brief Places the body in it's environment.
     * The body's origin point will be placed in the provided argument.
     * All associated child bodies will be placed as well.
     * 
     * @param originVolume Occupied volume of the body's origin point
     * @throws BodyCollisionException thrown when any part of the body collides.
     */
    virtual void occupyV(VolumeType &originVolume) override
    {
        freeV();

        // Self origin - occupation
        originVolume.occupyV(this);
        containerVolume.push_back(&originVolume);

        // Self bodyparts -- occupation
        for (auto bodypart : shape)
        {
        }

        // All children - occupation
        for (auto child : childBodies)
        {
        }
    }

    virtual void freeV() override
    {
        // Free children
        for (auto child : childBodies)
            child->freeV();

        // Free self
        for (auto occupied : containerVolume)
            occupied->freeV();
    }

    //##################################### Public own Methods #################################################
    /**
     * @brief Moves the body in the environment in the provided direction.
     * All child bodies will be moved as well.
     * 
     * @param direction Direction of the movement
     * @throws BodyCollisionException thrown when any part of the body collides.
     */
    void moveBody(const DirectionVector<int> &direction)
    {
    }

    /**
     * @brief Attaches another body as child body.
     * A body cannot be self attached.
     * 
     * @param body Child body to attach.
     */
    void attachBody(Body<Environment, VolumeType> &body)
    {
        if (&body == this)
            return;

        childBodies.insert(&body);
    }

    /**
     * @brief Detaches the provided body 
     * if it's attached to this as child.
     * 
     * @param body The child body to detach.
     */
    void detachBody(Body<Environment, VolumeType> &body)
    {
        childBodies.erase(&body);
    }

    //Getter
    /**
     * @brief Returns the position and orientation of the body's origin point
     */
    const Pose<> &getPose() const
    {
        return pose;
    }

private:
    //####################################### Private Members ###############################################
    /**
     * @brief Position and orientation of the body's origin point.
     */
    Pose<> pose;

    /**
     * @brief Contains the shape of the body in object coordinate system
     * Each body consists of an origin point at (0,0,0) in ocs
     * Each point that is not the origin must be registered in the shape set.
     * 
     * eg. A block of (1 length, 1 width, 3 height)
     * {Point<>{0,0,1},Point<>{0,0,2}} - origin = {0,0,0} mustn't be included.
     */
    std::set<Point<>> shape; // TODO bodypart tipus

    /**
     * @brief Associated other body instance that controls this body and its children.
     */
    std::shared_ptr<Body<Environment, VolumeType>> parentBody;

    /**
     * @brief Associated other body instances that can behave (eg. move) with this body.
     */
    std::set<Body<Environment, VolumeType> *> childBodies;

    /**
     * @brief Set of volumes that the body occupies in the environment.
     * Elements point to "environment" class member.
     */
    std::vector<VolumeType *> containerVolume;

    /**
     * @brief Parts of the body and it's children
     * which are collided with the last operation.
     */
    //std::vector<> collidedBuffer;

    /**
     * @brief Reference to the 3D environment in which the body resides.
     * Owner of parentVolume's elements.
     */
    Environment<VolumeType> &environment;
};
#endif