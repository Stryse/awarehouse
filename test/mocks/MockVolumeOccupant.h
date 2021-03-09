#ifndef MOCK_VOLUME_OCCUPANT__H
#define MOCK_VOLUME_OCCUPANT__H

#include "IVolumeOccupant.h"
#include "gmock/gmock.h"

class MockVolumeOccupant : public IVolumeOccupant
{
    MOCK_METHOD(void, occupyV, (const INavigationVolume<IVolumeOccupant> &), (const, override));
    MOCK_METHOD(void, freeV, (), (const, override));
    MOCK_METHOD(const std::vector<INavigationVolume<IVolumeOccupant> *> &, getOccupiedVolumes, (), (const, override));
};

#endif /* MOCK_VOLUME_OCCUPANT__H */