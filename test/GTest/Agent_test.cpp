#include "DeliveryRobot.h"
#include "MotorAction.h"
#include "Network.h"
#include "ObservableEnvironment.h"
#include "PodDock.h"
#include "Tile.h"
#include "gtest/gtest.h"
#include <memory>
#include <queue>
#include <stdexcept>

class AgentTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        // ####################### Setup Environment ########################
        const size_t X = 6;
        const size_t Y = 6;
        const size_t Z = 6;
        env = std::make_shared<ObservableNavEnvironment<>>(X, Y, Z);
        for (int x = 0; x < X; ++x)
            for (int y = 0; y < Y; ++y)
                for (int z = 0; z < Z; ++z)
                {
                    // Robot's tile will be podDock for additional testing
                    if (x == 1 && y == 1 && z == 0)
                    {
                        std::shared_ptr<PodDock<>> podDock = std::make_shared<PodDock<>>(PodDock<>::Point{x, y, z});
                        env->getBuffer()[env->getCoordAsIndex(x, y, z)] = podDock;
                        podDock->addAssociatedPod(env);
                    }
                    else
                        env->getBuffer()[env->getCoordAsIndex(x, y, z)] = std::make_shared<Tile>(Tile::Point{x, y, z});
                }

        robot = std::make_unique<DeliveryRobot<>>(env, Point<>(1, 1, 0), DirectionVector<>::UP());
    }

    static std::shared_ptr<ObservableNavEnvironment<>> env;
    static std::unique_ptr<DeliveryRobot<>> robot;

    using MotorAction = ::MotorAction<std::decay_t<decltype(*robot)>::Body, std::decay_t<decltype(*robot)>::Energy>;
    static void moveHelper(std::queue<MotorAction *> moveActionQueue);
};

// #################################### SHARED VARIABLES ######################################################
std::shared_ptr<ObservableNavEnvironment<>> AgentTest::env = nullptr;
std::unique_ptr<DeliveryRobot<>> AgentTest::robot = nullptr;

/************************************************************************
 * @brief Perform movement without agent MCU solely with MoveMechanism
 ************************************************************************/
void AgentTest::moveHelper(std::queue<AgentTest::MotorAction *> moveActionQueue)
{
    if (moveActionQueue.empty())
        return;

    while (!moveActionQueue.empty())
    {
        MotorAction *action = moveActionQueue.front();
        (*action)();

        moveActionQueue.pop();
    }
}
// ####################################### TESTS ##############################################################

TEST_F(AgentTest, Movement)
{
    bool checkPos = robot->getBody()->getPose().getPosition() == Point<>(1, 1, 0);
    bool checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::UP();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 100);

    // Up Move
    AgentTest::moveHelper(robot->getMoveMechanism()->move(DirectionVector<>::UP()));
    checkPos = robot->getBody()->getPose().getPosition() == Point<>(1, 2, 0);
    checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::UP();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 99);

    // Left Move
    AgentTest::moveHelper(robot->getMoveMechanism()->move(DirectionVector<>::LEFT()));
    checkPos = robot->getBody()->getPose().getPosition() == Point<>(0, 2, 0);
    checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::LEFT();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 97);

    // Right Move
    AgentTest::moveHelper(robot->getMoveMechanism()->move(DirectionVector<>::RIGHT()));
    checkPos = robot->getBody()->getPose().getPosition() == Point<>(1, 2, 0);
    checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::RIGHT();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 94);

    // Down Move
    AgentTest::moveHelper(robot->getMoveMechanism()->move(DirectionVector<>::DOWN()));
    checkPos = robot->getBody()->getPose().getPosition() == Point<>(1, 1, 0);
    checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::DOWN();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 92);
}

TEST_F(AgentTest, Network_Connection)
{
    std::shared_ptr<Network> network = std::make_shared<Network>();
    robot->getNetworkAdapter().connect(network);
    EXPECT_EQ(robot->getNetworkAdapter().getAddress(), 100);
    EXPECT_TRUE(robot->getNetworkAdapter().isConnected());

    robot->getNetworkAdapter().disconnect();
    EXPECT_EQ(robot->getNetworkAdapter().getAddress(), 0);
    EXPECT_FALSE(robot->getNetworkAdapter().isConnected());
}

TEST_F(AgentTest, Networked_Movement)
{
    std::shared_ptr<Network> network = std::make_shared<Network>();

    // Connect Controller
    NetworkAdapter adapter;
    adapter.connectWithAddress(network, 0x1);
    EXPECT_EQ(adapter.getAddress(), 0x1);
    EXPECT_TRUE(adapter.isConnected());

    // Connect Robot
    robot->getNetworkAdapter().connect(network);
    EXPECT_EQ(robot->getNetworkAdapter().getAddress(), 100);
    EXPECT_TRUE(robot->getNetworkAdapter().isConnected());

    // Send Movement message
    adapter.send(std::make_unique<MoveAgentMessage>(DirectionVector<>::LEFT(), adapter.getAddress()), 100);

    // Turn tick
    robot->tick(0);
    bool checkPos = robot->getBody()->getPose().getPosition() == Point<>(1, 1, 0);
    bool checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::LEFT();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 99);

    // Move tick
    robot->tick(1);
    checkPos = robot->getBody()->getPose().getPosition() == Point<>(0, 1, 0);
    checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::LEFT();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 98);
}

TEST_F(AgentTest, Networked_PodManagement)
{
    std::shared_ptr<Network> network = std::make_shared<Network>();

    // Connect Controller
    NetworkAdapter adapter;
    adapter.connectWithAddress(network, 0x1);
    EXPECT_EQ(adapter.getAddress(), 0x1);
    EXPECT_TRUE(adapter.isConnected());

    // Connect Robot
    robot->getNetworkAdapter().connect(network);
    EXPECT_EQ(robot->getNetworkAdapter().getAddress(), 100);
    EXPECT_TRUE(robot->getNetworkAdapter().isConnected());

    // Send Pickup Message
    adapter.send(std::make_unique<PickupPodMessage>(adapter.getAddress()), 100);

    // Robot Action perform
    EXPECT_EQ(robot->getPodHolder().getChildPod(), nullptr);
    robot->tick(0);
    EXPECT_NE(robot->getPodHolder().getChildPod(), nullptr);
}