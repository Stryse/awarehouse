#ifndef INAVIGATIONAL_ENVIRONMENT__H
#define INAVIGATIONAL_ENVIRONMENT__H

// ############################## Forward declarations ####################################
template <typename T>
class Point;
// ########################################################################################

/******************************************************************************************
 * @brief An Interface of a 3D environment.
 * Implementors of this interface must provide a way to access each unit of volume
 * based on a provided 3D point, and must tell if a point is in or out of bounds of the
 * environment.
 * @tparam TVolumeType Type of the 3D volume.
 * Should derive from INavigationVolume interface.
 ******************************************************************************************/
template <typename TVolumeType>
class INavigationalEnvironment
{
public:
    using Point = Point<int>;
    using VolumeType = TVolumeType;

public:
    /*****************************************************************
     * @brief Returns a unit of volume based on a provided 3D point.
     * @param point Point in space of the volume unit.
     *****************************************************************/
    virtual VolumeType &getVolume(const Point &point) = 0;

    /*****************************************************************
     * @brief Returns a unit of volume based on a provided 3D point.
     * @param point Point in space of the volume unit.
     *****************************************************************/
    virtual const VolumeType &getVolume(const Point &point) const = 0;

    /*****************************************************************
     * @brief Returns whether provided point is in the environment.
     * @param point 3D Point in space.
     *****************************************************************/
    virtual bool isInBounds(const Point &point) const = 0;
};

#endif /* INAVIGATIONAL_ENVIRONMENT__H */