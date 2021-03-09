#ifndef INAVIGATIONVOLUME__H
#define INAVIGATIONVOLUME__H

#include <stdexcept>

namespace inav_util
{
    /**
 * @brief Térfogatütközés kivétele, kiváltódik ha egy térfogategységbe esik 2
 * különböző térfoglaló.
 * 
 * @tparam Occupant A térfoglaló típusa
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
 *  @brief Egy térfogategység funkcióit előíró interfész.
 *
 *  A térfogategységnek tudnia kell fogadni térfoglalókat és számontartani ki
 * foglalja el őket. Ha két térfoglaló foglalja el egyszerre kivételben jeleznie
 * kell az ütközést.
 *
 *  @tparam Occupant A térfoglaló típusa.
 *  @throws VolumeCollisionException térfogat ütközéskor.
 */
template <typename Occupant>
class INavigationVolume
{
public:
    using VolumeCollisionException = inav_util::VolumeCollisionException<Occupant>;

public:
    virtual ~INavigationVolume() {}

    /**
   * @brief A térfoglaló megkísérli elfoglalni a térfogategységet.
   * @throws VollumeCollisionException kiváltódik, ha egyszerre legalább 2
   * térfoglaló próbálja birtokolni.
   */
    virtual void occupyV(const Occupant &) = 0;

    /**
   * @brief Az Occupant térfoglaló elhagyja a térfogategységet, szabaddá
   * teszi.
   */
    virtual void freeV() = 0;

    /**
   * @brief Visszaadja, hogy szabad-e a térfogategység.
   */
    virtual bool isVolumeFree() const = 0;

    /**
   * @brief A térfogategységet jelenleg elfoglaló térfoglalót adja vissza
   * 
   * @return Occupant Térfoglaló, amennyiben létezik, különben pedig implementációfüggő (exception vagy nullptr)
   */
    virtual const Occupant &getOccupant() const = 0;
};
#endif /* INAVIGATIONVOLUME__H */