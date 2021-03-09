#include "Tile.h"
#include "IVolumeOccupant.h"

Tile::Tile(const Point &pos) : INavigationVolume<Tile::OccupantType>(), position(pos), occupant(nullptr)
{
}

Tile::~Tile()
{
}

void Tile::occupyV(const Tile::OccupantType &occupant)
{
    if (this->occupant != nullptr && this->occupant != occupant)
        throw Tile::VolumeCollisionException("Volume collision happened", this->occupant, occupant);

    this->occupant = occupant;
}

void Tile::freeV()
{
    occupant.reset();
}

bool Tile::isVolumeFree() const
{
    return occupant == nullptr;
}

const Tile::OccupantType &Tile::getOccupant() const
{
    return occupant;
}

const Tile::Point &Tile::getPosition() const
{
    return position;
}