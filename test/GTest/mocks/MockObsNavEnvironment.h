#ifndef MOCK_OBSERVABLE_NAVIGATIONAL_ENVIRONMENT
#define MOCK_OBSERVABLE_NAVIGATIONAL_ENVIRONMENT

#include "INavigationalEnvironment.h"
#include "IObservableEnvironment.h"
#include "gmock/gmock.h"

template <typename TileType>
class MockObsNavEnvironment : public IObservableEnvironment, public INavigationalEnvironment<TileType>
{
    MOCK_METHOD(Info *, rayCast, (const Point &, const DirectionVector &, int), (const, override));
    MOCK_METHOD(Info *, scan, (const Point &, int), (const, override));
    MOCK_METHOD(Info *, getInfoAtPoint, (const Point &), (const, override));

    MOCK_METHOD(const TileType &, getVolume, (const Point &), (const, override));
    MOCK_METHOD(TileType &, getVolume, (const Point &), (override));
    MOCK_METHOD(bool, isInBounds, (const Point &), (const, override));
};

#endif /* MOCK_OBSERVABLE_NAVIGATIONAL_ENVIRONMENT */