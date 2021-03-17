#ifndef OBSERVABLE_NAV_ENVIRONMENT__H
#define OVSERVABLE_NAV_ENVIRONMENT__H

#include "IInfoProvider.h"
#include "INavigationalEnvironment.h"
#include "IObservableEnvironment.h"
#include "LibConfig.h"
#include "Point.h"
#include <memory>
#include <vector>

template <typename TVolumeType = config::navigation::DefaultVolumeType>
class ObservableNavEnvironment : public INavigationalEnvironment<TVolumeType>
{
public:
    /****************
     * @brief Type of Volume that can be queried in the environment.
     ****************/
    using VolumeType = TVolumeType;

    /****************
     * @brief Type of Point in the environment.
     ****************/
    using Point = typename INavigationalEnvironment<VolumeType>::Point;

private:
    std::vector<std::unique_ptr<TVolumeType>> tileSpace;
    size_t xLength;
    size_t yLength;
    size_t zLength;

public:
    //################################## Constructors, destructors ############################################
    /**
     * @brief Creates a observable,navigatable 3D environment of the sizes provided
     * @param xLength X (width)  -- Navplane
     * @param yLength Y (Height) -- Navplane
     * @param zLength Z (Depth)  -- Air -- Default is 1 meaning it's a plane
     */
    ObservableNavEnvironment(size_t xLength, size_t yLength, size_t zLength = 1)
        : tileSpace(xLength * yLength * zLength), xLength(xLength), yLength(yLength), zLength(zLength)
    {
    }

    ObservableNavEnvironment(const ObservableNavEnvironment &other)
        : tileSpace(other.tileSpace), xLength(other.xLength), yLength(other.yLength), zLength(other.zLength)
    {
    }

    virtual ~ObservableNavEnvironment() = default;

    //#############################  IObservableEnvironment implementation ######################################

    //################################ INavigationalEnvironment implementation ##################################

    virtual const VolumeType &getVolume(const Point &point) const override
    {
        return *tileSpace[point.getPosX() + yLength * (point.getPosY() + zLength * point.getPosZ())];
    }

    virtual VolumeType &getVolume(const Point &point) override
    {
        return const_cast<VolumeType &>(static_cast<const ObservableNavEnvironment &>(*this).getVolume(point));
    }

    virtual bool isInBounds(const Point &point) const override
    {
        return point.getPosX() >= 0 && point.getPosX() < xLength &&
               point.getPosY() >= 0 && point.getPosY() < yLength &&
               point.getPosZ() >= 0 && point.getPosZ() < zLength;
    }

    //############################################## Own Getter ####################################################

    const std::vector<std::unique_ptr<VolumeType>> &getBuffer() const { return tileSpace; }

    std::vector<std::unique_ptr<VolumeType>> &getBuffer() { return tileSpace; }

    size_t getXLength() const { return xLength; }

    size_t getYLength() const { return yLength; }

    size_t getZLength() const { return zLength; }

    size_t getCoordAsIndex(size_t x, size_t y, size_t z) const { return x + yLength * (y + zLength * z); }
};

#endif /* OBSERVABLE_NAV_ENVIRONMENT__H */