#ifndef POINT__H
#define POINT__H

#include "directionvector.h"
#include <vector>

/**
 * @brief A geometriai 3 dimenziós pont.
 * 
 * @tparam T koordináták típusa.
 */
template <typename T = int>
class Point
{
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic value eg. int,float");
    using DirectionVector = DirectionVector<T>;

private:
    std::vector<int> coordinates;

public:
    Point(int posX, int posY, int posZ = 0) : coordinates{posX, posY, posZ} {}
    Point(const std::vector<int> &coordinates) : coordinates(coordinates) {}
    Point(const Point &other) : coordinates(other.coordinates) {}

    /* Limit point space if we want */
    // static bool isInPointDomain(const Point &point);
    // static bool isInPointDomain(int posX, int posY, int posZ = 0);

    //Getter
    int getPosX() const { return coordinates[0]; }
    int getPosY() const { return coordinates[1]; }
    int getPosZ() const { return coordinates[2]; }

    //Setter
    void setPosX(int posX) { coordinates[0] = posX; }
    void setPosY(int posY) { coordinates[1] = posY; }
    void setPosZ(int posZ) { coordinates[2] = posZ; }

    //Functionality
    void move(const DirectionVector &vector)
    {
        for (size_t i = 0; i < coordinates.size(); ++i)
            coordinates[i] += vector.getAsVector().at(i);
    }

    Point &operator=(const Point &other)
    {
        this->coordinates = other.coordinates;
        return *this;
    }
};

#endif