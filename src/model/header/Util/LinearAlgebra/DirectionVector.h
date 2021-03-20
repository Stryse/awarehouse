#ifndef DIRECTIONVECTOR__H
#define DIRECTIONVECTOR__H

#include <array>
#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <type_traits>

// ######################## FORWARD DECLARATION ########################
template <typename CoordT, std::size_t>
class Point;
// #####################################################################
/**********************************************
 * @brief 3 dimensional mathematical vector
 * 
 * @tparam CoordinateT type of vector coordinates,
 * must be numerical.
 * @tparam N dimension of the vector
 **********************************************/
template <typename CoordT = int, std::size_t N = 3>
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
    std::array<CoordinateT, N> coordinates;

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
     * @brief Factories of common vectors
     *************************************************/
    static DirectionVector<CoordinateT, 3> UP() { return DirectionVector{0, 1, 0}; }
    static DirectionVector<CoordinateT, 3> DOWN() { return DirectionVector{0, -1, 0}; }
    static DirectionVector<CoordinateT, 3> LEFT() { return DirectionVector{-1, 0, 0}; }
    static DirectionVector<CoordinateT, 3> RIGHT() { return DirectionVector{1, 0, 0}; }
    static DirectionVector<CoordinateT, 3> ABOVE() { return DirectionVector{0, 0, 1}; }
    static DirectionVector<CoordinateT, 3> BELOW() { return DirectionVector{0, 0, -1}; }
    /**************************************************
     * @brief Creates a vector with the provided buffer
     * DirectionVector takes hold of argument vector.
     **************************************************/
    explicit DirectionVector(std::array<CoordinateT, N> &&coordinateBuffer)
        : coordinates(std::move(coordinateBuffer))
    {
    }

    /**************************************************
     * @brief Creates a vector object out of a same
     * dimensional point.
     **************************************************/
    explicit DirectionVector(const Point<CoordinateT, N> &sourcePoint)
        : coordinates(sourcePoint.getCoordinates()) {}

    /***************************************************
     * @brief Dot product of two vectors
     * @return CoordinateT: scalar
     ***************************************************/
    static CoordinateT dot(const DirectionVector<CoordinateT, N> &v1,
                           const DirectionVector<CoordinateT, N> &v2)
    {
        CoordinateT value = 0;
        for (size_t i = 0; i < v1.coordinates.size(); ++i)
            value += v1.coordinates[i] * v2.coordinates[i];

        return value;
    }

    void mulWithScalar(CoordinateT scalar)
    {
        for (auto &c : coordinates)
            c *= scalar;
    }

    DirectionVector muledWithScalar(CoordinateT scalar)
    {
        std::array<CoordinateT, N> buffer{};
        for (size_t i = 0; i < N; ++i)
            buffer[i] = coordinates[i] * scalar;

        return DirectionVector{std::move(buffer)};
    }

    /********************** Operator ******************
     * @brief Equality on same size and same coordinates.
     **************************************************/
    friend inline bool operator==(const DirectionVector<CoordinateT, N> &lhs,
                                  const DirectionVector<CoordinateT, N> &rhs)
    {
        for (size_t i = 0; i < lhs.coordinates.size(); ++i)
        {
            if (std::abs(lhs.coordinates[i] - rhs.coordinates[i]) > 1e-6f)
                return false;
        }
        return true;
    }

    friend inline bool operator!=(const DirectionVector<CoordinateT, N> &lhs,
                                  const DirectionVector<CoordinateT, N> &rhs)
    {
        return !(lhs == rhs);
    }

    /***************************************************
     * @brief Component-wise comparison
     ***************************************************/
    friend inline bool operator<(const DirectionVector &lhs, const DirectionVector &rhs)
    {
        return lhs.getBuffer() < rhs.getBuffer();
    }

    //############################## Getter ####################################
    const CoordinateT &getX() const { return coordinates[0]; }
    const CoordinateT &getY() const { return coordinates[1]; }
    const CoordinateT &getZ() const { return coordinates[2]; }
    const std::array<CoordinateT, N> &getBuffer() const { return coordinates; }
    std::array<CoordinateT, N> &getBuffer() { return coordinates; }
    constexpr size_t getSize() const noexcept { return N; }
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