#ifndef DIRECTIONVECTOR__H
#define DIRECTIONVECTOR__H

#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <vector>

/**
 * @brief Irányok 2 dimenzióban, jobbsodrású koordinátarendszerben értelmezzük.
 * 
 * ↑_y →_x
 */
enum class Directions
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * @brief A matematikai 3 dimenziós vektor típusa.
 * 
 * @tparam T Vektorkoordináták típusa.
 */
template <typename T = int>
class DirectionVector
{
    static_assert(std::is_arithmetic<T>::value, "Type T must be arithmetic value eg. int,float");

private:
    std::vector<T> coordinates;

public:
    /**
     * @brief Vektort hoz létre pontkoordináták alapján
     * 
     * @param xAxis x koordináta
     * @param yAxis y koordináta (alapértelmezetten 0)
     * @param zAxis z koordináta (alapértelmezetten 0)
     */
    DirectionVector(T xAxis, T yAxis = 0, T zAxis = 0) : coordinates{xAxis, yAxis, zAxis} {}

    /**
     * @brief Egyhosszú 2 dimenziós vektort hoz létre irány alapján
     * 
     * @param direction Irány felsorolási típus.
     * @throws std::invalid_argument ha le nem kezelt irány értékkel hívjuk.
     */
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

    /**
     * @brief Vektorok skaláris szorzata
     * 
     * @param v1 Első bemenő vektor
     * @param v2 Második bemenő vektor
     * @return T típusú skalár
     */
    static T dot(const DirectionVector &v1, const DirectionVector &v2)
    {
        if (v1.coordinates.size() != v2.coordinates.size())
            return false;

        T value = 0;
        for (size_t i = 0; i < v1.coordinates.size; ++i)
            value += v1.coordinates[i] * v2.coordinates[i];

        return value;
    }

    //Operators
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

    //Getter
    const T &getX() const { return coordinates[0]; }
    const T &getY() const { return coordinates[1]; }
    const T &getZ() const { return coordinates[2]; }
    const std::vector<T> &getAsVector() const { return coordinates; }
    std::vector<T> &getAsVector() { return coordinates; }

    //Setter
    void setX(const T &xAxis) { coordinates[0] = xAxis; }
    void setY(const T &yAxis) { coordinates[1] = yAxis; }
    void setZ(const T &zAxis) { coordinates[2] = zAxis; }
    void setXY(T xAxis, T yAxis)
    {
        coordinates[0] = xAxis;
        coordinates[1] = yAxis;
    }
    void setXZ(T xAxis, T zAxis)
    {
        coordinates[0] = xAxis;
        coordinates[2] = zAxis;
    }
    void setYZ(T yAxis, T zAxis)
    {
        coordinates[1] = yAxis;
        coordinates[2] = zAxis;
    }
};

#endif