#ifndef I_MOVE_MECHANISM__H
#define I_MOVE_MECHANISM__H

#include "DirectionVector.h"
#include "LibConfig.h"
#include <memory>
#include <queue>
#include <set>
#include <vector>

// ########################## Forward Declarations ########################
template <typename TBody>
class AMotor;
template <typename TBody, typename TEnergy>
class MotorAction;
template <class TEnergy>
class IDepleting;
// ########################################################################

template <typename TBody, typename TEnergy = config::agent::DefaultEnergy>
class IMoveMechanism
{
public:
    // ######################### Body Related #############################
    using Body = TBody;
    using DirectionVector = typename Body::DirectionVector;
    using AMotor = AMotor<Body>;

    //Energy related
    using Energy = TEnergy;
    using IDepleting = IDepleting<Energy>;
    using MotorAction = MotorAction<Body, Energy>;
    // ####################################################################

protected:
    explicit IMoveMechanism(std::vector<std::unique_ptr<AMotor>> &&motors,
                            std::set<DirectionVector> &&moveSet,
                            IDepleting &resource)

        : motors(std::move(motors)), moveSet(std::move(moveSet)), resource(resource)
    {
    }

public:
    virtual ~IMoveMechanism() = default;

public:
    /************************************************************************
     * @brief Returns whether the entity can move to the
     * provided direction.
     ************************************************************************/
    virtual bool canMove(const DirectionVector &direction) const = 0;

    /*************************************************************************
     * @brief Returns a sequence of MotorActions that will lead
     * to the provided direction .
     * (Must be reachable by the entity (canMove() ==> true)).
     *************************************************************************/
    virtual std::queue<MotorAction *> move(const DirectionVector &direction) = 0;

    /**************************************************************************
     * @brief Returns the amount of energy needed to reach the
     * provided direction.
     **************************************************************************/
    virtual Energy getEnergyCost(const DirectionVector &direction) const = 0;

    /**************************************************************************
     * @brief Returns the amount of time needed to reach the
     * provided direction.
     **************************************************************************/
    virtual int getTimeCost(const DirectionVector &direction) const = 0;

public:
    /***************************************************************************
     * @brief Returns all the motors that the entity has.
     ***************************************************************************/
    const std::vector<std::unique_ptr<AMotor>> &getMotors() const { return motors; }

    /***************************************************************************
     * @brief Returns all the directions where the entity can move.
     ***************************************************************************/
    const std::set<DirectionVector> &getMoveSet() const { return moveSet; }

protected:
    std::vector<std::unique_ptr<AMotor>> motors;
    std::set<DirectionVector> moveSet;
    IDepleting &resource;
};

#endif /* I_MOVE_MECHANISM__H */