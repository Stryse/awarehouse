#ifndef POINT__H
#define POINT__H

#include "DirectionVector.h"
#include <array>
#include <cstddef>

/*********************************************************
 * @brief 3 dimensional point in space.
 * 
 * @tparam CoordT type of coordinates (must be numeric).
 *********************************************************/
template <typename CoordT = int, std::size_t N = 3>
class Point
{
public:
    //[ASSERTION] Template parameter type must be numeric type.
    static_assert(std::is_arithmetic<CoordT>::value, "Type CoordT must be arithmetic value eg. int,float");

    /********************* TYPES ***************
     * @brief Type of the coordinates of the point (numeric).
     *********************/
    using CoordinateType = CoordT;

    /*********************
     * @brief Type of the vector that the point can be moved with.
     *********************/
    using DirectionVector = ::DirectionVector<CoordT, N>;

private:
    std::array<CoordT, N> coordinates;

public:
    /**************************************************************
     * @brief Constructs a 3D point with the given coordinates.
     **************************************************************/
    Point(CoordT posX, CoordT posY, CoordT posZ = 0) : coordinates{posX, posY, posZ} {}

    /**************************************************************
     * @brief Constructs a N dimensional point with the coordinates 
     * of the provided vector
     **************************************************************/
    explicit Point(const std::array<CoordT, N> &coordinates) : coordinates(coordinates) {}
    explicit Point(const std::array<CoordT, N> &&coordinates) : coordinates(std::move(coordinates)) {}

    Point(const Point &other) : coordinates(other.coordinates) {}

    //################## Getter ########################
    constexpr size_t getSize() const noexcept { return N; }
    std::array<CoordT, N> getCoordinates() { return coordinates; }
    const std::array<CoordT, N> getCoordinates() const { return coordinates; }
    CoordT getPosX() const { return coordinates[0]; }
    CoordT getPosY() const { return coordinates[1]; }
    CoordT getPosZ() const { return coordinates[2]; }

    //################## Setter ########################
    void setPosX(CoordT posX) { coordinates[0] = posX; }
    void setPosY(CoordT posY) { coordinates[1] = posY; }
    void setPosZ(CoordT posZ) { coordinates[2] = posZ; }

    //################ Functionality ###################

    /***************************************************
     * @brief Moves a point with a direction vector.
     ***************************************************/
    void move(const DirectionVector &vector)
    {
        for (size_t i = 0; i < coordinates.size(); ++i)
            coordinates[i] += vector.getBuffer().at(i);
    }

    /****************************************************
     * @brief Returns a new point moved in a direction.
     ****************************************************/
    Point moved(const DirectionVector &vector) const
    {
        std::array<CoordT, N> buffer{};
        for (size_t i = 0; i < coordinates.size(); ++i)
            buffer[i] = coordinates[i] + vector.getBuffer()[i];

        return Point(std::move(buffer));
    }

    //################## Operators #####################
    Point &operator=(const Point &other)
    {
        if (this != &other)
            this->coordinates = other.coordinates;

        return *this;
    }

    /***************************************************************
     * @brief Two points are equal if they are the same dimensions
     * and all the coordinates are equal
     ***************************************************************/
    friend inline bool operator==(const Point<CoordinateType, N> &lhs, const Point<CoordinateType, N> &rhs)
    {
        for (size_t i = 0; i < lhs.coordinates.size(); ++i)
        {
            if ((std::abs(lhs.coordinates[i] - rhs.coordinates[i]) > 1e-6f))
                return false;
        }

        return true;
    }

    /*******************************************************************
     * @brief A subtraction of two points that result in a DirectionVector.
     * 
     * @param lhs Point that the result vector is pointing to
     * @param rhs Point that the result vector is pointing from
     * @return DirectionVector vector pointing from rhs to lhs
     *******************************************************************/
    friend inline DirectionVector operator-(const Point<CoordinateType, N> &lhs, const Point<CoordinateType, N> &rhs)
    {
        std::array<CoordinateType, N> buffer{};
        for (size_t i = 0; i < lhs.coordinates.size(); ++i)
            buffer[i] = lhs.coordinates[i] - rhs.coordinates[i];

        return DirectionVector(std::move(buffer));
    }

    static int manhattanNorm(const Point<CoordinateType, N> &first, const Point<CoordinateType, N> &second)
    {
        int norm = 0;
        for (int i = 0; i < first.getSize(); ++i)
            norm += std::abs(first.coordinates[i] - second.coordinates[i]);

        return norm;
    }
};

#endif /* POINT__H */
