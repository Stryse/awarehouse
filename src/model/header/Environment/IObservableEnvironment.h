#ifndef I_OBSERVABLE_ENVIRONMENT__H
#define I_OBSERVABLE_ENVIRONMENT__H

#include <cstddef>

//Forward Declarations
class Info;
template <typename T, std::size_t N>
class DirectionVector;
template <typename T, std::size_t N>
class Point;
//

/**
 * @brief Interface of a data structure that can be observed
 * and can retreive info of its elements
 */
class IObservableEnvironment
{
public:
    using DirectionVector = DirectionVector<int, 3>;
    using Point = Point<int, 3>;

    virtual ~IObservableEnvironment() = default;

    /**
     * @brief Retreives info of it's elements starting from a point
     * to a provided direction in a provided depth
     * @param direction Direction of the scanray
     * @param depth The amount of units the ray travels
     * @return Info* Observed information
     */
    virtual Info *rayCast(const Point &startPoint, const DirectionVector &direction, int depth) const = 0;

    /**
     * @brief Retreives info of it's elements from a center point
     * to a provided radius
     * 
     * @param center Center of scanning
     * @param radius Maximum length of scan circle
     * @return Info* Observed information
     */
    virtual Info *scan(const Point &center, int radius) const = 0;

    /**
     * @brief Retreives info from a provided point
     * 
     * @param targetPoint Point from which we want to get info
     * @return Info* Observed information
     */
    virtual Info *getInfoAtPoint(const Point &targetPoint) const = 0;
};

#endif /* I_OBSERVABLE_ENVIRONMENT__H */