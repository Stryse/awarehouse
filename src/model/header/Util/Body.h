#ifndef BODY__H
#define BODY__H

#include "INavigationVolume.h"
#include "IVolumeOccupant.h"
#include "Pose.h"
#include <memory>
#include <set>
#include <vector>

// ##################################### FORWARD DECLARATIONS ################################################
template <typename VolumeType>
class INavigationalEnvironment;
// ###########################################################################################################

/*************************************************************************************************************
 * @brief A connected set of Volume Occupant placed in an Environment which must be derived from
 * INavigationalEnvironment. The Environment must have a member type defined as
 * @code VolumeType @endcode which must derive from @code INavigationVolume @endcode
 *************************************************************************************************************/
template <typename TEnvironment>
class Body : public IVolumeOccupant<typename TEnvironment::VolumeType>
{
public:
    /****************** TYPES ********************
     * @brief Type of environment in which the body resides
     *********************************************/
    using Environment = TEnvironment;

    /******************
     * @brief The type of the environment's volume.
     * Must be derived from @code INavigationVolume @endcode
     ******************/
    using VolumeType = typename Environment::VolumeType;

    /******************
     * @brief The Type of the environment's point unit.
     ******************/
    using Point = typename Environment::Point;

    /******************
     * @brief Directions are the same type as environment's points
     ******************/
    using DirectionVector = DirectionVector<typename Environment::Point::CoordinateType>;

    /******************
     * @brief Position an direction aggregate
     ******************/
    using Pose = Pose<typename Environment::Point::CoordinateType, typename Environment::Point::CoordinateType>;

public:
    /*************************************** Constructors ****************************************************
     * @brief Construct a new Body object
     * 
     * @param position Position of the body's origin point
     * @param orientation Orientation of the body's origin point
     * @param environment The reference to the environment in which the body is placed
     * @param bodyShape Holds the non-origin points of the body
     *********************************************************************************************************/
    Body(const Point &position,
         const DirectionVector &orientation,
         Environment &environment,
         std::vector<Point> &&bodyShape = {})
        : pose(position, orientation), environment(environment), shape(std::move(bodyShape))
    {
        //Place body in environment on creation.
        occupyV(environment.getVolume(position));
    }

    virtual ~Body() = default;

    //############################## IVolumeOccupant implementation ##########################################
    /*********************************************************************************************************
     * @brief Places the body in it's environment.
     * The body's origin point will be placed in the provided argument.
     * All associated child bodies will be placed as well.
     * 
     * @param originVolume Occupied volume of the body's origin point
     * @throws BodyCollisionException thrown when any part of the body collides.
     *********************************************************************************************************/
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

    /*******************************************************************
     * @brief The body and all of its child bodies leave the environment
     * making it possible for other entities to occupy that space.
     *******************************************************************/
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
    /******************************************************************************
     * @brief Moves the body in the environment in the provided direction.
     * All child bodies will be moved as well.
     * 
     * @param direction Direction of the movement
     * @throws BodyCollisionException thrown when any part of the body collides.
     ******************************************************************************/
    void moveBody(const DirectionVector &direction)
    {
    }

    /******************************************************************************
     * @brief Attaches another body as child body.
     * A body cannot be self attached.
     * 
     * Child bodies behave accordingly to the parent body but can behave
     * independently if operation is called on child body reference directly.
     * @param body Child body to attach.
     ******************************************************************************/
    void attachBody(Body<Environment> &body)
    {
        if (&body == this)
            return;

        childBodies.insert(&body);
    }

    /******************************************************************************
     * @brief Detaches the provided body 
     * if it's attached to this as child.
     * 
     * @param body The child body to detach.
     ******************************************************************************/
    void detachBody(Body<Environment> &body)
    {
        childBodies.erase(&body);
    }

    //Getter
    /*******************************************************************************
     * @brief Returns the position and orientation of the body's origin point
     *******************************************************************************/
    const Pose &getPose() const
    {
        return pose;
    }

private:
    //####################################### Private Members ###############################################
    /********************
     * @brief Position and orientation of the body's origin point.
     ********************/
    Pose pose;

    /*******************************************************************************
     * @brief Contains the shape of the body in object coordinate system
     * Each body consists of an origin point at (0,0,0) in ocs.  
     * Each point that is not the origin must be registered in the shape set.
     * 
     * eg. A block of (1 length, 1 width, 3 height)
     * {Point<>{0,0,1},Point<>{0,0,2}}  (origin = {0,0,0} mustn't be included)
     *******************************************************************************/
    std::vector<Point> shape;

    /**********************
     * @brief Associated other body instance that controls this body and its children.
     **********************/
    std::shared_ptr<Body<Environment>> parentBody;

    /**********************
     * @brief Associated other body instances that can behave (eg. move) with this body.
     **********************/
    std::set<Body<Environment> *> childBodies;

    /***********************
     * @brief Set of volumes that the body occupies in the environment.
     * Elements point to "environment" class member.
     ***********************/
    std::vector<VolumeType *> containerVolume;

    /***********************
     * @brief Parts of the body and it's children
     * which are collided with the last operation.
     ***********************/
    //std::vector<> collidedBuffer;

    /***********************
     * @brief Reference to the 3D environment in which the body resides.
     * Owner of parentVolume's elements.
     ***********************/
    Environment &environment;
};
#endif /* BODY__H */