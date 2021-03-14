#ifndef DIRECTIONVECTOR__H
#define DIRECTIONVECTOR__H

#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <vector>

/*********************************************
 * @brief Directions in 2 dimensional space.
 * ↑y →x
 * Can be used to construct DirectionVector
 *********************************************/
enum class Directions
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**********************************************
 * @brief 3 dimensional mathematical vector
 * 
 * @tparam CoordinateT type of vector coordinates,
 * must be numerical.
 **********************************************/
template <typename CoordT = int>
class DirectionVector
{
public:
    //////[ASSERTION] Template parameter CoordinateT must be numeric type
    static_assert(std::is_arithmetic<CoordT>::value, "Type CoordinateT must be arithmetic value eg. int,float");

    using CoordinateT = CoordT;

private:
    /********************
     * @brief Buffer that holds vector coordinates
     ********************/
    std::vector<CoordinateT> coordinates;

public:
    /*************************************************
     * @brief Creates a vector of provided coordinates
     * 
     * @param xAxis x coordinate
     * @param yAxis y coordinate (default 0)
     * @param zAxis z coordinate (default 0)
     *************************************************/
    explicit DirectionVector(CoordinateT xAxis, CoordinateT yAxis = 0, CoordinateT zAxis = 0)
        : coordinates{xAxis, yAxis, zAxis} {}

    /*************************************************
     * @brief Creates a vector of length 1 with the provided direction
     * 
     * @param direction Points toward this direction (enum)
     * @throws std::invalid_argument if a direction value is not handled
     *************************************************/
    explicit DirectionVector(const Directions &direction) : coordinates(3)
    {
        switch (direction)
        {
        case Directions::UP:
            coordinates = {0, 1, 0};
            break;

        case Directions::DOWN:
            coordinates = {0, -1, 0};
            break;

        case Directions::LEFT:
            coordinates = {-1, 0, 0};
            break;

        case Directions::RIGHT:
            coordinates = {1, 0, 0};
        default:
            throw std::invalid_argument("Unhandled value.");
        }
    }

    /***************************************************
     * @brief Dot product of two vectors
     * @return CoordinateT: scalar
     ***************************************************/
    static CoordinateT dot(const DirectionVector &v1, const DirectionVector &v2)
    {
        if (v1.coordinates.size() != v2.coordinates.size())
            return false;

        CoordinateT value = 0;
        for (size_t i = 0; i < v1.coordinates.size; ++i)
            value += v1.coordinates[i] * v2.coordinates[i];

        return value;
    }

    /********************** Operator ******************
     * @brief Equality on same size and same coordinates.
     **************************************************/
    bool operator==(const DirectionVector &other) const
    {
        if (coordinates.size() != other.coordinates.size())
            return false;

        for (size_t i = 0; i < coordinates.size(); ++i)
        {
            if (std::abs(coordinates[i] - other.coordinates[i]) > 1e-6f)
                return false;
        }

        return true;
    }

    bool operator!=(const DirectionVector &other) const
    {
        return !(*this == other);
    }

    //############################## Getter ####################################
    const CoordinateT &getX() const { return coordinates[0]; }
    const CoordinateT &getY() const { return coordinates[1]; }
    const CoordinateT &getZ() const { return coordinates[2]; }
    const std::vector<CoordinateT> &getAsVector() const { return coordinates; }
    std::vector<CoordinateT> &getAsVector() { return coordinates; }

    //############################### Setter ###################################
    void setX(const CoordinateT &xAxis) { coordinates[0] = xAxis; }
    void setY(const CoordinateT &yAxis) { coordinates[1] = yAxis; }
    void setZ(const CoordinateT &zAxis) { coordinates[2] = zAxis; }

    void setXY(CoordinateT xAxis, CoordinateT yAxis)
    {
        coordinates[0] = xAxis;
        coordinates[1] = yAxis;
    }
    void setXZ(CoordinateT xAxis, CoordinateT zAxis)
    {
        coordinates[0] = xAxis;
        coordinates[2] = zAxis;
    }
    void setYZ(CoordinateT yAxis, CoordinateT zAxis)
    {
        coordinates[1] = yAxis;
        coordinates[2] = zAxis;
    }
};

#endif /* DIRECTIONVECTOR__H */