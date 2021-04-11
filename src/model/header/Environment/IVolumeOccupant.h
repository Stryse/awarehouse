#ifndef IVOLUMEOCCUPANT__H
#define IVOLUMEOCCUPANT__H

#include "LibConfig.h"
#include <memory>
#include <vector>

// ######################### FORWARD DECLARATIONS ############################
template <typename T>
class INavigationVolume;
// ###########################################################################

/*****************************************************************************
 * @brief Interface of an object that can occupy volume(s) that
 * is derived from type provided as Template parameter (TVolumeType)
 * 
 * @tparam TVolumeType Type of volume the occupant can occupy.
 *****************************************************************************/
template <typename TVolumeType = config::navigation::DefaultVolumeType>
class IVolumeOccupant
{
public:
    /***************************************************************************
     * @brief Type of volume the occupant can occupy
     ***************************************************************************/
    using VolumeType = TVolumeType;

public:
    /***************************************************************************
     * @brief The Volume Occupant must leave the volume
     * on destruction ( call FreeV() )
     ***************************************************************************/
    virtual ~IVolumeOccupant() = default;

    /***************************************************************************
     * @brief The occupant occupies the volume(s) according to its size and shape
     * and registers it as one of its parent volumes.
     * 
     * @param originVolume Volume instance that will be associated with
     * the occupant's origin point.
     ***************************************************************************/
    virtual void occupyV(const std::shared_ptr<VolumeType> &originVolume) = 0;

    /***************************************************************************
     * @brief The occupant leaves its associated parent volumes.
     ***************************************************************************/
    virtual void freeV() = 0;
};

#endif /* IVOLUMEOCCUPANT__H */