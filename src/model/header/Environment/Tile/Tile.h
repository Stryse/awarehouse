#ifndef TILE__H
#define TILE__H

#include "IAgentSignalHandler.h"
#include "INavigationVolume.h"
#include "LibConfig.h"
#include "Point.h"

// ######################### FORWARD DECLARATIONS ##############################
template <typename VolumeType>
class IVolumeOccupant;
// #############################################################################

/*******************************************************************************
 * @brief
 * Element of volume in the Warehouse space.
 * Can accept occupants of that derive from IVolumeOccupant<Tile>
 *******************************************************************************/
class Tile : public INavigationVolume<IVolumeOccupant<Tile> *>,
             public IAgentSignalHandler<config::navigation::DefaultEnvironment>
{
public:
    /*****************************************************
     * @brief Type of 3D point in which tile can be placed
     *****************************************************/
    using Point = Point<int, 3>;

    /*****************************************************
     * @brief Type of occupant that the tile can accept.
     *****************************************************/
    using OccupantType = IVolumeOccupant<Tile> *;

private:
    /************
     * @brief Position in 3D space
     ************/
    Point position;

    /************
     * @brief The occupant currently occupying the tile.
     * Nullptr if there is no occupant.
     ************/
    OccupantType occupant;

public:
    // Constructors,destructor

    /*************
     * @brief Construct a new Tile object
     * @param pos Point in 3D space, should be unique per tile
     *************/
    explicit Tile(const Point &pos);
    virtual ~Tile();

    // INavigationVolumeInterface implementation

    virtual void occupyV(const OccupantType &occupant) override;
    virtual void freeV() override;
    virtual bool isVolumeFree() const override;
    virtual const OccupantType &getOccupant() const override;

    // Getter

    virtual const Point &getPosition() const;
};

#endif /* TILE__H */