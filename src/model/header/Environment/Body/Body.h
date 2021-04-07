#ifndef BODY__H
#define BODY__H

#include "INavigationalEnvironment.h"
#include "IVolumeOccupant.h"
#include "Pose.h"
#include <memory>
#include <set>
#include <vector>

// ##################################### FORWARD DECLARATIONS ################################################
// ###########################################################################################################

/*************************************************************************************************************
 * @brief A connected set of Volume Occupant placed in an Environment which must be derived from
 * INavigationalEnvironment. The Environment must have a member type defined as
 * @code VolumeType @endcode which must derive from @code INavigationVolume @endcode
 *************************************************************************************************************/
template <typename TEnvironment = INavigationalEnvironment<>>
class Body : public IVolumeOccupant<typename TEnvironment::VolumeType>
{
public:
    /****************** TYPES ******************************
     * @brief Type of environment in which the body resides.
     *
     *******************************************************/
    using Environment = TEnvironment;

    /*******************************************************
     * @brief The type of the environment's volume.
     * Must be derived from @code INavigationVolume @endcode
     *
     *******************************************************/
    using VolumeType = typename Environment::VolumeType;

    /*******************************************************
     * @brief The Type of the environment's point unit.
     *
     *******************************************************/
    using Point = typename Environment::Point;

    /*************************************************************
     * @brief Directions are the same type as environment's points
     *
     *************************************************************/
    using DirectionVector = ::DirectionVector<typename Environment::Point::CoordinateType>;

    /*****************************************
     * @brief Position and direction aggregate
     *
     *****************************************/
    using Pose = ::Pose<typename Environment::Point::CoordinateType, typename Environment::Point::CoordinateType>;

public:
    /*************************************** Constructors ****************************************************
     * @brief Construct a new Body object
     * 
     * @param position Position of the body's origin point
     * @param orientation Orientation of the body's origin point
     * @param environment The reference to the environment in which the body is placed
     * @param bodyShape Holds the non-origin points of the body
     *
     *********************************************************************************************************/
    Body(const Point &position,
         const DirectionVector &orientation,
         const std::shared_ptr<Environment> &environment,
         std::vector<Point> &&bodyShape = {})

        : pose(position, orientation), shape(std::move(bodyShape)), parentBody(nullptr), environment(environment)
    {
        //Place body in environment on creation.
        occupyV(environment->getVolume(position));
    }

    virtual ~Body()
    {
        // Destroy child record in parent
        if (parentBody != nullptr)
            parentBody->detachBody(*this);
        // Destroy parent record in child
        for (auto &child : childBodies)
            child->parentBody = nullptr;
        childBodies.clear();
        // Leave Environment, children remain
        freeV();
    }

    //############################## IVolumeOccupant implementation ##########################################
    /*********************************************************************************************************
     * @brief Places the body in it's environment.
     * The body's origin point will be placed in the provided argument.
     * All associated child bodies will be placed as well.
     * 
     * @param originVolume Occupied volume of the body's origin point
     * @throws VolumeCollisionException thrown when any part of the body collides.
     *********************************************************************************************************/
    virtual void occupyV(VolumeType &originVolume) override
    {
        freeV();

        // Self origin - occupation
        originVolume.occupyV(this);
        containerVolume.push_back(&originVolume);

        // Self bodyparts -- occupation
        for (auto &bodypart : shape)
        {
            Point newPos = originVolume.getPosition().moved(DirectionVector(bodypart));
            environment->getVolume(newPos).occupyV(this);
            containerVolume.push_back(&environment->getVolume(newPos));
        }

        // All children - occupation
        for (auto &child : childBodies)
        {
            DirectionVector this2child = child->getPose().getPosition() - pose.getPosition();
            Point newChildPos = originVolume.getPosition().moved(this2child);
            child->occupyV(environment->getVolume(newChildPos));
        }
        pose.setPosition(originVolume.getPosition());
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

        containerVolume.clear();
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
        occupyV(environment->getVolume(pose.getPosition().moved(direction)));
    }

    /******************************************************************************
     * @brief Rotates the body's orientation
     * 
     * TODO: Rotate shaped bodies properly
     ******************************************************************************/
    void rotate(const DirectionVector &targetOrientation)
    {
        pose.setOrientation(targetOrientation);
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
        if (&body == this || body.getParent() != nullptr)
            return;

        childBodies.insert(&body);
        body.parentBody = this;
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
        body.parentBody = nullptr;
    }

    //################################# Getter #####################################
    /*******************************************************************************
     * @brief Returns the position and orientation of the body's origin point
     *******************************************************************************/
    const Pose &getPose() const { return pose; }
    Pose &getPose() { return pose; }

    /*******************************************************************************
     * @brief Returns the body's children.
     *******************************************************************************/
    const std::set<Body<Environment> *> &getChildren() const { return childBodies; }
    std::set<Body<Environment> *> &getChildren() { return childBodies; }

    /*******************************************************************************
     * @brief Returns the body's parent body if exist. Nullptr if no parent.
     *******************************************************************************/
    const Body<Environment> *getParent() const { return parentBody; }
    Body<Environment> *getParent() { return parentBody; }

    /*******************************************************************************
     * @brief Returns the Volumes that the body (except its children) occupies
     *******************************************************************************/
    const std::vector<VolumeType *> &getContainerVolume() const { return containerVolume; }

    /*******************************************************************************
     * @brief Returns the environment in which the body resides.
     *******************************************************************************/
    const Environment &getEnvironment() const { return *environment; }
    Environment &getEnvironment() { return *environment; }

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
    Body<Environment> *parentBody;

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

    // std::vector<VolumeType *> collidedBuffer; TODO IMPLEMENT

    /***********************
     * @brief Reference to the 3D environment in which the body resides.
     * Owner of parentVolume's elements.
     ***********************/
    std::shared_ptr<Environment> environment;
};
#endif /* BODY__H */
