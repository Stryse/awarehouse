#include "DeliveryRobot.h"
#include "ObservableEnvironment.h"
#include "Tile.h"
#include "gtest/gtest.h"

class AgentTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        // ####################### Setup Environment ########################
        const size_t X = 6;
        const size_t Y = 6;
        const size_t Z = 6;
        env = std::make_unique<ObservableNavEnvironment<>>(X, Y, Z);
        for (int x = 0; x < X; ++x)
            for (int y = 0; y < Y; ++y)
                for (int z = 0; z < Z; ++z)
                    env->getBuffer()[env->getCoordAsIndex(x, y, z)] = std::make_unique<Tile>(Tile::Point{x, y, z});

        robot = std::make_unique<DeliveryRobot<>>(*env, Point<>(1, 1, 0), DirectionVector<>(Directions::UP));
    }

    static std::unique_ptr<ObservableNavEnvironment<>> env;
    static std::unique_ptr<DeliveryRobot<>> robot;
};

// #################################### SHARED VARIABLES ######################################################
std::unique_ptr<ObservableNavEnvironment<>> AgentTest::env = nullptr;
std::unique_ptr<DeliveryRobot<>> AgentTest::robot = nullptr;
// ####################################### TESTS ##############################################################

TEST_F(AgentTest, fasz)
{
    robot->move(DirectionVector<>(Directions::UP));
    EXPECT_TRUE(true);
}