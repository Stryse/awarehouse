#ifndef I_MOVE_MECHANISM__H
#define I_MOVE_MECHANISM__H

#include "LibConfig.h"
#include <functional>
#include <vector>

// ###################### Forward Declarations ########################
template <typename Environment>
class Body;
template <typename CoordT>
class DirectionVector;
// ####################################################################

template <typename TEnvironment = config::navigation::DefaultEnvironment,
          typename TEnergy = config::agent::DefaultEnergy>

class IMoveMechanism
{

public:
    using Environment = TEnvironment;
    using Energy = TEnergy;
    using Body = Body<Environment>;
    using DirectionVector = typename Body::DirectionVector;

public:
    explicit IMoveMechanism(Body &body)
        : body(body) {}

public:
    virtual void move(const DirectionVector &direction) = 0;
    virtual bool isValidMoveDirection(const DirectionVector &direction) const = 0;
    virtual void rotate(const DirectionVector &targetOrientation) = 0;
    virtual bool isValidRotation(const DirectionVector &targetOrientation) const = 0;
    virtual Energy getMovementCost(const DirectionVector &direction) const = 0;
    virtual std::vector<std::function<void(const DirectionVector &)>> getMoveActionSeq(const DirectionVector &direction) const = 0;

protected:
    Body &body;
};

#endif /* I_MOVE_MECHANISM__H */