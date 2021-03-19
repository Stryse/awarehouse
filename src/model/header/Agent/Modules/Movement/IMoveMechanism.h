#ifndef I_MOVE_MECHANISM__H
#define I_MOVE_MECHANISM__H

#include "DirectionVector.h"
#include "LibConfig.h"
#include <set>
#include <vector>

// ###################### Forward Declarations ########################
template <typename TBody>
class AMotor;
template <typename TBody, typename TEnergy>
class MotorAction;
template <class TEnergy>
class IDepleting;
// ####################################################################

template <typename TBody, typename TEnergy = config::agent::DefaultEnergy>
class IMoveMechanism
{
public:
    // Body Related
    using Body = TBody;
    using DirectionVector = typename Body::DirectionVector;
    using AMotor = AMotor<Body>;

    //Energy related
    using Energy = TEnergy;
    using IDepleting = IDepleting<Energy>;
    using MotorAction = MotorAction<Body, Energy>;

public:
    explicit IMoveMechanism(std::vector<AMotor *> &&motors, IDepleting &resource)
        : motors(std::move(motors)), resource(resource)
    {
    }

public:
    virtual bool canMove(const DirectionVector &direction) const = 0;
    virtual MotorAction *move(const DirectionVector &direction) = 0;
    virtual Energy getEnergyCost(const DirectionVector &direction) const = 0;
    virtual int getTimeCost(const DirectionVector &direction) const = 0;

public:
    const std::vector<AMotor *> &getMotors() const { return motors; }
    const std::set<DirectionVector> &getMoveSet() const { return moveSet; }

protected:
    std::vector<AMotor *> motors;
    IDepleting &resource;
    std::set<DirectionVector> moveSet;
};

#endif /* I_MOVE_MECHANISM__H */