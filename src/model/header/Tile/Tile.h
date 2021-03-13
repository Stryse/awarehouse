#ifndef TILE__H
#define TILE__H

#include "INavigationVolume.h"
#include "Point.h"

// ############### FORWARD DECLARATIONS #################
template <typename VolumeType>
class IVolumeOccupant;
// ######################################################

/**
 * @brief
 * Element of volume in the Warehouse space.
 */
class Tile : public INavigationVolume<IVolumeOccupant<Tile> *>
{
public:
    using Point = Point<int>;
    using OccupantType = IVolumeOccupant<Tile> *;

private:
    Point position;
    OccupantType occupant;

public:
    // Constructors,destructor
    explicit Tile(const Point &pos);
    virtual ~Tile();

    // INavigationVolumeInterface implementation
    virtual void occupyV(const OccupantType &occupant) override;
    virtual void freeV() override;
    virtual bool isVolumeFree() const override;
    virtual const OccupantType &getOccupant() const override;

    // Getter
    const Point &getPosition() const;
};

#endif