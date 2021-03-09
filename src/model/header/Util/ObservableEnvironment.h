#ifndef OBSERVABLE_ENVIRONMENT__H
#define OVSERVABLE_ENVIRONMENT__H

#include "IObservableEnvironment.h"
#include "Point.h"
#include <memory>
#include <vector>

template <typename TileType>
class ObservableEnvironment : public IObservableEnvironment
{
public:
    using TileSpace = std::vector<std::shared_ptr<TileType>>;

private:
    TileSpace tileSpace;
    size_t xLength;
    size_t yLength;
    size_t zLength;

public:
    //Constructors
    /**
     * @brief Létrehoz egy megfigyelhető környezet adatszerkezetet 
     * a paraméterként kapott méretekkel
     * 
     * @param xLength X tengely mérete (szélesség)
     * @param yLength Y tengely mérete (magasság)
     * @param zLength Z tengely mérete (mélység)
     */
    ObservableEnvironment(size_t xLength, size_t yLength, size_t zLength)
        : tileSpace(xLength * yLength * zLength), xLength(xLength), yLength(yLength), zLength(zLength)
    {
    }

    //IObservableEnvironment implementation
    virtual Info *rayCast(const DirectionVector &direction, int depth) const override
    {
        //TODO
        return nullptr;
    }

    virtual Info *scan(const Point &center, int range) const override
    {
        //TODO
        return nullptr;
    }

    virtual Info *getInfoAtPoint(const Point &targetPoint) const override
    {
        //TODO
        return getTile(targetPoint)->getInfo();
    }

    //Getter
    /**
     * @brief Egy térfogatrészt visszaad a csempetérből
     * 
     * @param x X koordináta
     * @param y Y koordináta
     * @param z Z koordináta
     * @return Csempe típus
     */
    const std::shared_ptr<const TileType> &getTile(int x, int y = 0, int z = 0) const
    {
        return tileSpace[x + yLength * (y + zLength * z)];
    }

    /**
     * @brief Egy térfogatrészt visszaad a csempetérből
     * 
     * @param point Térpont
     * @return Csempe típus
     */
    const std::shared_ptr<const TileType> &getTile(const Point &point) const
    {
        return getTile(point.getPosX(), point.getPosY(), point.getPosZ());
    }

    /**
     * @overload const std::shared_ptr<const TileType> &getTile(int x, int y = 0, int z = 0) const
     */
    const std::shared_ptr<TileType> &getTile(int x, int y, int z)
    {
        return tileSpace[x + yLength * (y + zLength * z)];
    }

    /**
     * @overload const std::shared_ptr<const TileType> &getTile(const Point &point) const
     */
    const std::shared_ptr<TileType> &getTile(const Point &point)
    {
        return getTile(point.getPosX(), point.getPosY(), point.getPosZ());
    }
};

#endif