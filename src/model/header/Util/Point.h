#ifndef POINT__H
#define POINT__H

#include "DirectionVector.h"
#include <vector>

/**
 * @brief 3 dimensional point in space
 * 
 * @tparam CoordT type of coordinates (must be numeric)
 */
template <typename CoordT = int>
class Point
{
    static_assert(std::is_arithmetic<CoordT>::value, "Type CoordT must be arithmetic value eg. int,float");
    using DirectionVector = DirectionVector<CoordT>;

private:
    std::vector<CoordT> coordinates;

public:
    Point(CoordT posX, CoordT posY, CoordT posZ = 0) : coordinates{posX, posY, posZ} {}
    explicit Point(const std::vector<CoordT> &coordinates) : coordinates(coordinates) {}
    Point(const Point &other) : coordinates(other.coordinates) {}

    //Getter
    CoordT getPosX() const { return coordinates[0]; }
    CoordT getPosY() const { return coordinates[1]; }
    CoordT getPosZ() const { return coordinates[2]; }

    //Setter
    void setPosX(CoordT posX) { coordinates[0] = posX; }
    void setPosY(CoordT posY) { coordinates[1] = posY; }
    void setPosZ(CoordT posZ) { coordinates[2] = posZ; }

    //Functionality
    void move(const DirectionVector &vector)
    {
        for (size_t i = 0; i < coordinates.size(); ++i)
            coordinates[i] += vector.getAsVector().at(i);
    }

    Point &operator=(const Point &other)
    {
        if (this != &other)
            this->coordinates = other.coordinates;

        return *this;
    }
};

#endif