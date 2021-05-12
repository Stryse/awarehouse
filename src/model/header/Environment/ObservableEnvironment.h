#ifndef OBSERVABLE_NAV_ENVIRONMENT__H
#define OBSERVABLE_NAV_ENVIRONMENT__H

#include "INavigationalEnvironment.h"
#include "Point.h"
#include <memory>
#include <vector>


// ####################### FORWARD DECLARATIONS ######################## //
class Tile;
// ##################################################################### //

/*******************************************************************
 * @brief A 3D navigateable and observable environment.
 * Volume units can be queried and occupied, also it can provide
 * different kinds of informations with different methods.
 *******************************************************************/
class ObservableNavEnvironment : public INavigationalEnvironment<Tile>
{
public:
    /************************************************************
     * @brief Type of Point in the environment.
     ************************************************************/
    using Point = typename INavigationalEnvironment<Tile>::Point;

private:
    std::vector<std::shared_ptr<Tile>> tileSpace;
    size_t xLength;
    size_t yLength;
    size_t zLength;

public:
    //################################## Constructors, destructors ############################################
    /**********************************************************************************************************
     * @brief Creates a observable,navigatable 3D environment of the sizes provided filled with default volumes
     * @param xLength X (Width)  -- Navplane
     * @param yLength Y (Height) -- Navplane
     * @param zLength Z (Depth)  -- Air -- Default is 1 meaning it's a plane
     **********************************************************************************************************/
    ObservableNavEnvironment(size_t xLength, size_t yLength, size_t zLength = 1);
    ObservableNavEnvironment(const ObservableNavEnvironment &other);
    virtual ~ObservableNavEnvironment();

    //################################ INavigationalEnvironment implementation ##################################
    /************************************************************************************************************
     * @brief Returns the volume unit at provided point
     ************************************************************************************************************/
    virtual const std::shared_ptr<Tile> &getVolume(const Point &point) const override;
    virtual std::shared_ptr<Tile> &getVolume(const Point &point) override;

    /************************************************************************************************************
     * @brief Returns whether point is inside the environment.
     ************************************************************************************************************/
    virtual bool isInBounds(const Point &point) const override;

    //############################################## Own Getter ####################################################

    const std::vector<std::shared_ptr<Tile>> &getBuffer() const;
    std::vector<std::shared_ptr<Tile>> &getBuffer();

    size_t getXLength() const;
    size_t getYLength() const;
    size_t getZLength() const;

    /***************************************************************
     * @brief Returns a 3D coordinate as a flattened 1D coordinate
     ***************************************************************/
    size_t getCoordAsIndex(size_t x, size_t y, size_t z) const;
    size_t getCoordAsIndex(const Point &point) const;
};

#endif /* OBSERVABLE_NAV_ENVIRONMENT__H */
