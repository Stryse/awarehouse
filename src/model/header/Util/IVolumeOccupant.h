#ifndef IVOLUMEOCCUPANT__H
#define IVOLUMEOCCUPANT__H

#include <memory>
#include <vector>

// ########## FORWARD DECLARATIONS ############ //
template <typename T>
class INavigationVolume;
// ############################################ //

/**
 * @brief Interface of an object that can occupy volume(s) that
 * is derived from type: "INavigationVolume<std::shared_ptr<IVolumeOccupant>>"
 */
template <typename VolumeType>
class IVolumeOccupant
{
public:
    /**
     * @brief The Volume Occupant must leave the volume
     * on destruction
     */
    virtual ~IVolumeOccupant() = default;

    /**
     * @brief The occupant occupies the volume(s) according to its size and shape
     * and registers it as one of its parent volumes.
     * 
     * @param originVolume Volume instance that will be associated with
     * the occupant's origin point.
     */
    virtual void occupyV(VolumeType &originVolume) = 0;

    /**
     * @brief The occupant leaves its associated parent volumes
     */
    virtual void freeV() = 0;
};

#endif /* IVOLUMEOCCUPANT__H */