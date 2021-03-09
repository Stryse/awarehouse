#ifndef TILE__H
#define TILE__H

#include "INavigationVolume.h"
#include "Point.h"
#include <memory>

// ############### FORWARD DECLARATIONS #################
class IVolumeOccupant;
// ######################################################

/**
 * @brief 
 * A raktártér egy csempéjét leíró osztály.
 */
class Tile : public INavigationVolume<std::shared_ptr<IVolumeOccupant>>
{
public:
    using Point = Point<int>;
    using OccupantType = std::shared_ptr<IVolumeOccupant>;

private:
    Point position;
    OccupantType occupant;

public:
    //Constructors,destructor
    explicit Tile(const Point &pos);
    virtual ~Tile();

    //INavigationVolumeInterface implementation
    virtual void occupyV(const OccupantType &occupant) override;
    virtual void freeV() override;
    virtual bool isVolumeFree() const override;
    virtual const OccupantType &getOccupant() const override;

    //Getter
    const Point &getPosition() const;
};

#endif