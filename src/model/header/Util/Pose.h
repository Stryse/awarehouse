#ifndef POSE__H
#define POSE__H

#include "DirectionVector.h"
#include "Point.h"

template <typename PointCoordT = int, typename VectorCoordT = int>
class Pose
{
public:
    using Position = Point<PointCoordT>;
    using Orientation = DirectionVector<VectorCoordT>;

private:
    Position position;
    Orientation orientation;

public:
    //Constructors,Destructor
    Pose(const Position &position, const Orientation &orientation)
        : position(position), orientation(orientation) {}

    Pose(const Pose &other)
        : position(other.position), orientation(other.orientation) {}

    virtual ~Pose() = default;

    //Getter
    const Position &getPosition() const { return position; }
    const Orientation &getOrientation() const { return orientation; }
    Position &getPosition() { return position; }
    Orientation &getOrientation() { return orientation; }

    //Setter
    void setPosition(const Position &position) { this->position = position; }
    void setOrientation(const Orientation &orientation) { this->orientation = orientation; }
};

#endif /* POSE__H */