#ifndef BODY__H
#define BODY__H

#include "INavigationalEnvironment.h"
#include "IVolumeOccupant.h"
#include "ObservableEnvironment.h"
#include "Pose.h"
#include "Tile.h"
#include <memory>
#include <set>
#include <vector>

// ##################################### FORWARD DECLARATIONS ################################################
// ###########################################################################################################

/*************************************************************************************************************
 * @brief A connected set of Volume Occupant placed in an Environment which must be derived from
 * INavigationalEnvironment. Body's can have child bodies which behave together with the parent body.
 * If a body occupies or leaves an environment all child bodies do the same.
 * However if a parent body is deleted child bodies detach automatically.
 *************************************************************************************************************/
class Body : public IVolumeOccupant<Tile>
{
public:
    /*******************************************************
     * @brief The Type of the environment's point unit.
     *
     *******************************************************/
    using Point = typename ObservableNavEnvironment::Point;

    /*************************************************************
     * @brief Directions are the same type as environment's points
     *
     *************************************************************/
    using DirectionVector = ::DirectionVector<ObservableNavEnvironment::Point::CoordinateType>;

    /*****************************************
     * @brief Position and direction aggregate
     *
     *****************************************/
    using Pose = ::Pose<ObservableNavEnvironment::Point::CoordinateType, ObservableNavEnvironment::Point::CoordinateType>;

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
         const std::shared_ptr<ObservableNavEnvironment> &environment,
         std::vector<Point> &&bodyShape = {});

    Body(const Body &other) = delete;
    Body(Body &&other) = delete;
    Body &operator=(const Body &other) = delete;
    virtual ~Body();

    //############################## IVolumeOccupant implementation ##########################################
    /*********************************************************************************************************
     * @brief Places the body in it's environment.
     * The body's origin point will be placed in the provided argument.
     * All associated child bodies will be placed as well.
     * 
     * @param originVolume Occupied volume of the body's origin point
     * @throws VolumeCollisionException thrown when any part of the body collides.
     *********************************************************************************************************/
    virtual void occupyV(const std::shared_ptr<Tile> &originVolume) override;

    /*******************************************************************
     * @brief The body and all of its child bodies leave the environment
     * making it possible for other entities to occupy that space.
     *******************************************************************/
    virtual void freeV() override;

    //##################################### Public own Methods #################################################
    /******************************************************************************
     * @brief Moves the body in the environment in the provided direction.
     * All child bodies will be moved as well.
     * 
     * @param direction Direction of the movement
     * @throws BodyCollisionException thrown when any part of the body collides.
     ******************************************************************************/
    void moveBody(const DirectionVector &direction);

    /*****************************************************************************
     * @brief Moves the body outside the environment without occupying volumes
     *
     * @param direction Direction of the movement
     * ***************************************************************************/
    void moveBodyOutsideEnvironment(const DirectionVector &direction);

    /******************************************************************************
     * @brief Rotates the body's orientation
     * 
     ******************************************************************************/
    void rotate(const DirectionVector &targetOrientation);

    /******************************************************************************
     * @brief Attaches another body as child body.
     * A body cannot be self attached.
     * 
     * Child bodies behave accordingly to the parent body but can behave
     * independently if operation is called on child body reference directly.
     * @param body Child body to attach.
     ******************************************************************************/
    void attachBody(Body &body);

    /******************************************************************************
     * @brief Detaches the provided body 
     * if it's attached to this as child.
     * 
     * @param body The child body to detach.
     ******************************************************************************/
    void detachBody(Body &body);

    //################################# Getter #####################################
    /*******************************************************************************
     * @brief Returns the position and orientation of the body's origin point
     *******************************************************************************/
    const Pose &getPose() const;
    Pose &getPose();

    /*******************************************************************************
     * @brief Returns the body's children.
     *******************************************************************************/
    const std::set<Body *> &getChildren() const;
    std::set<Body *> &getChildren();

    /*******************************************************************************
     * @brief Returns the body's parent body if exist. Nullptr if no parent.
     *******************************************************************************/
    const Body *getParent() const;
    Body *getParent();

    /*******************************************************************************
     * @brief Returns the Volumes that the body (except its children) occupies
     *******************************************************************************/
    const std::vector<std::shared_ptr<Tile>> &getContainerVolume() const;

    /*******************************************************************************
     * @brief Returns the environment in which the body resides.
     *******************************************************************************/
    const ObservableNavEnvironment &getEnvironment() const;
    ObservableNavEnvironment &getEnvironment();

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
    Body *parentBody;

    /**********************
     * @brief Associated other body instances that can behave (eg. move) with this body.
     **********************/
    std::set<Body *> childBodies;

    /***********************
     * @brief Set of volumes that the body occupies in the environment.
     * Elements point to "environment" class member.
     ***********************/
    std::vector<std::shared_ptr<Tile>> containerVolume;

    /***********************
     * @brief Parts of the body and it's children
     * which are collided with the last operation.
     ***********************/

    /***********************
     * @brief Reference to the 3D environment in which the body resides.
     * Owner of parentVolume's elements.
     ***********************/
    std::shared_ptr<ObservableNavEnvironment> environment;
};
#endif /* BODY__H */
