#include "Body.h"
#include "ObservableEnvironment.h"
#include "Tile.h"
#include "mocks/MockObsNavEnvironment.h"
#include "gtest/gtest.h"

class BodyTest : public testing::Test
{
protected:
};

// #################################### SHARED VARIABLES ######################################################

// ####################################### TESTS ##############################################################

TEST_F(BodyTest, getVolumeAtPoint_x_y_z)
{
    ObservableNavEnvironment<Tile> env{3, 3, 3};
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 3; ++z)
                env.getBuffer()[env.getCoordAsIndex(x, y, z)] = std::make_unique<Tile>(Tile::Point{x, y, z});

    Body<ObservableNavEnvironment<Tile>> body(Point<int>{0, 0, 0},
                                              DirectionVector<int>{Directions::UP},
                                              env);

    body.occupyV(env.getVolume(Point<int>{1, 1, 0}));
    EXPECT_TRUE(true);
}