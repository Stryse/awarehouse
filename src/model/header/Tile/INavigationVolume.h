#ifndef INAVIGATIONVOLUME__H
#define INAVIGATIONVOLUME__H

#include <stdexcept>

namespace inav_util
{
    /**
 * @brief Thrown when 2 or more occupants occupy one unit of volume (collision)
 * @tparam Occupant Type of volume occupant
 */
    template <typename Occupant>
    class VolumeCollisionException : std::runtime_error
    {
    public:
        const Occupant &occupant;
        const Occupant &collider;

        explicit VolumeCollisionException(const char *message, const Occupant &occupant, const Occupant &collider)
            : std::runtime_error(message), occupant(occupant), collider(collider) {}
    };
} // namespace inav_util

/**
 *  @brief Interface of one unit of volume.
 *
 *  Implementors of this interface must accept Occupants and record which
 *  are occupying them. If two of occupants try to occupy them simultaniously
 *  it must throw an exception indicating collision.

 *  @tparam Occupant Type of the occupant that this class accepts.
 * Must call freeV() in destructor.
 * Must be pointer type.
 *  @throws VolumeCollisionException thrown when collision happens.
 */
template <typename Occupant>
class INavigationVolume
{
    //Asserting pointer template parameter type
    static_assert(std::is_pointer<Occupant>::value,
                  "Template parameter \"Occupant\" of type \"INavigationVolume\" must be of pointer type");

public:
    using VolumeCollisionException = inav_util::VolumeCollisionException<Occupant>;

public:
    virtual ~INavigationVolume() = default;

    /**
   * @brief The occupant tries to enter (occupy) the volume. Volume registers occupant as holder.
   * @throws VolumeCollisionException thrown when collision happens.
   */
    virtual void occupyV(const Occupant &) = 0;

    /**
   * @brief The occupant tries to leave (free) the volume. Volume registers occupant as null.
   */
    virtual void freeV() = 0;

    /**
   * @brief Returns whether the volume is free (safe to occupy)
   */
    virtual bool isVolumeFree() const = 0;

    /**
   * @brief Returns the occupant that currently occupies the if it exists.
   * 
   * @return Current occupant. Nullptr if no occupant.
   */
    virtual const Occupant &getOccupant() const = 0;
};
#endif /* INAVIGATIONVOLUME__H */