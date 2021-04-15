#include "ObservableEnvironment.h"
#include "Tile.h"

ObservableNavEnvironment::ObservableNavEnvironment(size_t xLength, size_t yLength, size_t zLength)
    : xLength(xLength), yLength(yLength), zLength(zLength)
{
    tileSpace.reserve(xLength * yLength * zLength);

    // Fill with defaults
    for (size_t x = 0; x < xLength; ++x)
        for (size_t y = 0; y < yLength; ++y)
            for (size_t z = 0; z < zLength; ++z)
                tileSpace[getCoordAsIndex(x, y, z)] = std::make_shared<Tile>(Point(x, y, z));
}

ObservableNavEnvironment::ObservableNavEnvironment(const ObservableNavEnvironment &other)
    : tileSpace(other.tileSpace), xLength(other.xLength), yLength(other.yLength), zLength(other.zLength)
{
}

ObservableNavEnvironment::~ObservableNavEnvironment() {}

const std::shared_ptr<Tile> &ObservableNavEnvironment::getVolume(const ObservableNavEnvironment::Point &point) const
{
    return tileSpace[point.getPosX() + (xLength * point.getPosY()) + (xLength * yLength * point.getPosZ())];
}

std::shared_ptr<Tile> &ObservableNavEnvironment::getVolume(const ObservableNavEnvironment::Point &point)
{
    return const_cast<std::shared_ptr<Tile> &>(static_cast<const ObservableNavEnvironment &>(*this).getVolume(point));
}

bool ObservableNavEnvironment::isInBounds(const Point &point) const
{
    return point.getPosX() >= 0 && point.getPosX() < xLength &&
           point.getPosY() >= 0 && point.getPosY() < yLength &&
           point.getPosZ() >= 0 && point.getPosZ() < zLength;
}

const std::vector<std::shared_ptr<Tile>> &ObservableNavEnvironment::getBuffer() const { return tileSpace; }
std::vector<std::shared_ptr<Tile>> &ObservableNavEnvironment::getBuffer() { return tileSpace; }

size_t ObservableNavEnvironment::getXLength() const { return xLength; }
size_t ObservableNavEnvironment::getYLength() const { return yLength; }
size_t ObservableNavEnvironment::getZLength() const { return zLength; }

size_t ObservableNavEnvironment::getCoordAsIndex(size_t x, size_t y, size_t z) const { return x + (xLength * y) + (xLength * yLength * z); }
size_t ObservableNavEnvironment::getCoordAsIndex(const Point &point) const { return getCoordAsIndex(point.getPosX(), point.getPosY(), point.getPosZ()); }