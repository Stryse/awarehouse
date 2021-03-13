#ifndef MOCK_VOLUME_OCCUPANT__H
#define MOCK_VOLUME_OCCUPANT__H

#include "IVolumeOccupant.h"
#include "gmock/gmock.h"

template <typename VolumeType>
class MockVolumeOccupant : public IVolumeOccupant<VolumeType>
{
    MOCK_METHOD(void, occupyV, (VolumeType &), (override));
    MOCK_METHOD(void, freeV, (), (override));
};

#endif /* MOCK_VOLUME_OCCUPANT__H */