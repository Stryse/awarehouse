#ifndef IVOLUMEOCCUPANT__H
#define IVOLUMEOCCUPANT__H

#include <vector>

// ########## FORWARD DECLARATIONS ############ //
template<typename T> class INavigationVolume;
// ############################################ //

/**
 * @brief Térfoglaló entitás interfésze.
 */
class IVolumeOccupant
{
public:
    virtual ~IVolumeOccupant() {}

    /**
     * @brief Elfoglalja a megfelelő térfogategysége(ke)t alakjához mérten a kapott középponttal.
     * 
     * @param originVolume Az entitás középpontjához rendelt térfogategység.
     */
    virtual void occupyV(const INavigationVolume<IVolumeOccupant> &originVolume) const = 0;

    /**
     * @brief Elhagyja az elfoglalt térfogategysége(ke)t az entitás.
     */
    virtual void freeV() const = 0;

    /**
     * @brief Visszaadja az entitás által elfoglalt térfogategysége(ke)t.
     */
    virtual const std::vector<INavigationVolume<IVolumeOccupant> *> &getOccupiedVolumes() const = 0;
};

#endif /* IVOLUMEOCCUPANT__H */