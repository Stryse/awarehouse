#ifndef I_MOVE_MECHANISM__H
#define I_MOVE_MECHANISM__H

#include "LibConfig.h"

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

    virtual void move(Body &body, const DirectionVector &direction) = 0;
    virtual bool isValidMoveDirection(Body &body, const DirectionVector &direction) const = 0;
    virtual void rotate(Body &body, const DirectionVector &targetOrientation) = 0;
    virtual bool isValidRotation(Body &body, const DirectionVector &targetOrientation) const = 0;
    virtual Energy getMovementCost(Body &body, const DirectionVector &direction) const = 0;
};

#endif /* I_MOVE_MECHANISM__H */