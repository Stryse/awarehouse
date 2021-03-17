#include "ObservableEnvironment.h"
#include "Tile.h"
#include "gtest/gtest.h"
#include <memory>

class ObservableNavEnvironmentTest : public testing::Test
{
protected:
};

// #################################### SHARED VARIABLES ######################################################

// ####################################### TESTS ##############################################################

TEST_F(ObservableNavEnvironmentTest, getVolumeAtPoint_x_y_z)
{
    ObservableNavEnvironment<> env{3, 3, 3};
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 3; ++z)
            {
                env.getBuffer()[env.getCoordAsIndex(x, y, z)] =
                    std::make_unique<decltype(env)::VolumeType>(Tile::Point{x, y, z});

                EXPECT_EQ(env.getVolume(decltype(env)::Point(x, y, z)).getPosition().getPosX(), x) << "[ERROR] Wrong coordinate X";
                EXPECT_EQ(env.getVolume(decltype(env)::Point(x, y, z)).getPosition().getPosY(), y) << "[ERROR] Wrong coordinate Y";
                EXPECT_EQ(env.getVolume(decltype(env)::Point(x, y, z)).getPosition().getPosZ(), z) << "[ERROR] Wrong coordinate Z";
            }
}