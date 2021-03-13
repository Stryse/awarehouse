#ifndef INAVIGATIONAL_ENVIRONMENT__H
#define INAVIGATIONAL_ENVIRONMENT__H

// ############ Forward declarations ############## //
template <typename T>
class Point;
// ################################################

template <typename VolumeType>
class INavigationalEnvironment
{
public:
    using Point = Point<int>;

    /**
     * @brief Returns a unit of volume
     * @param point Point in space of the volume unit
     */
    virtual VolumeType &getVolume(const Point &point) = 0;

    /**
     * @overload VolumeType &getVolume(const Point &point) = 0;
     */
    virtual const VolumeType &getVolume(const Point &point) const = 0;

    /**
     * @brief Returns whether provided point is in the environment
     * @param point 3D Point in space
     */
    virtual bool isInBounds(const Point &point) const = 0;
};

#endif /* INAVIGATIONAL_ENVIRONMENT__H */