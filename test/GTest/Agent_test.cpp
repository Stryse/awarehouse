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
        env = std::make_shared<ObservableNavEnvironment>(X, Y, Z);
        for (int x = 0; x < X; ++x)
            for (int y = 0; y < Y; ++y)
                for (int z = 0; z < Z; ++z)
                    env->getBuffer()[env->getCoordAsIndex(x, y, z)] = std::make_shared<Tile>(Tile::Point{x, y, z});

        // Replace tile with PodDocks
        podDock = std::make_shared<PodDock>(PodDock::Point{1, 1, 0});
        env->getBuffer()[env->getCoordAsIndex(1, 1, 0)] = podDock;
        podDock->addAssociatedPod(env);

        // Init robot
        robot = std::make_unique<DeliveryRobot>(env, Point<>(1, 1, 0), DirectionVector<>::UP());
    }

    static std::shared_ptr<ObservableNavEnvironment> env;
    static std::shared_ptr<PodDock> podDock;

    static std::unique_ptr<DeliveryRobot> robot;

    static void moveHelper(std::queue<MotorAction *> moveActionQueue);
};

// #################################### SHARED VARIABLES ######################################################
std::shared_ptr<ObservableNavEnvironment> AgentTest::env = nullptr;
std::unique_ptr<DeliveryRobot> AgentTest::robot = nullptr;
std::shared_ptr<PodDock> AgentTest::podDock = nullptr;

/************************************************************************
 * @brief Perform movement without agent MCU solely with MoveMechanism
 ************************************************************************/
void AgentTest::moveHelper(std::queue<MotorAction *> moveActionQueue)
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

    // Send AgentControlGranted message
    adapter.send(std::make_unique<AgentControlGrantedMessage>(adapter.getAddress()), 100);
    robot->tick(0);
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

    // Send AgentControlGranted message
    adapter.send(std::make_unique<AgentControlGrantedMessage>(adapter.getAddress()), 100);
    // Send Pickup Message
    adapter.send(std::make_unique<PickupPodMessage>(adapter.getAddress()), 100);

    // Robot Action perform (Pickup pod)
    EXPECT_EQ(robot->getPodHolder().getChildPod(), nullptr);
    EXPECT_NE(podDock->getPodHolder().getChildPod(), nullptr);
    EXPECT_EQ(robot->getBody()->getChildren().size(), 0);
    EXPECT_EQ(podDock->getPodHolder().getChildPod()->getBody()->getParent(), nullptr);
    robot->tick(0);
    EXPECT_NE(robot->getPodHolder().getChildPod(), nullptr);
    EXPECT_EQ(podDock->getPodHolder().getChildPod(), nullptr);
    EXPECT_EQ(robot->getBody()->getChildren().size(), 1);
    EXPECT_NE(robot->getPodHolder().getChildPod()->getBody()->getParent(), nullptr);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 99);

    // Movement With Pod
    adapter.send(std::make_unique<MoveAgentMessage>(DirectionVector<>::UP(), adapter.getAddress()), 100);
    bool checkPos = robot->getBody()->getPose().getPosition() == Point<>(1, 1, 0);
    bool checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::UP();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    robot->tick(1);

    // Check Agent moved and Pod moved with agent
    checkPos = robot->getBody()->getPose().getPosition() == Point<>(1, 2, 0);
    checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::UP();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 98);
    checkPos = robot->getPodHolder().getChildPod()->getBody()->getPose().getPosition() == Point<>(1, 2, 1);
    EXPECT_TRUE(checkPos);

    // Move back with Pod
    adapter.send(std::make_unique<MoveAgentMessage>(DirectionVector<>::DOWN(), adapter.getAddress()), 100);
    robot->tick(2);
    robot->tick(3);
    robot->tick(4);
    checkPos = robot->getBody()->getPose().getPosition() == Point<>(1, 1, 0);
    checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::DOWN();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 95);
    checkPos = robot->getPodHolder().getChildPod()->getBody()->getPose().getPosition() == Point<>(1, 1, 1);
    EXPECT_TRUE(checkPos);

    // Put down pod
    adapter.send(std::make_unique<PutDownPodMessage>(adapter.getAddress()), 100);
    EXPECT_NE(robot->getPodHolder().getChildPod(), nullptr);
    EXPECT_EQ(podDock->getPodHolder().getChildPod(), nullptr);
    EXPECT_EQ(robot->getBody()->getChildren().size(), 1);
    EXPECT_NE(robot->getPodHolder().getChildPod()->getBody()->getParent(), nullptr);
    robot->tick(5);
    EXPECT_EQ(robot->getPodHolder().getChildPod(), nullptr);
    EXPECT_NE(podDock->getPodHolder().getChildPod(), nullptr);
    EXPECT_EQ(podDock->getPodHolder().getChildPod()->getBody()->getParent(), nullptr);
    EXPECT_EQ(robot->getBody()->getChildren().size(), 0);
    EXPECT_EQ(robot->getEnergySource()->getCharge(), 94);

    // Move up again
    adapter.send(std::make_unique<MoveAgentMessage>(DirectionVector<>::UP(), adapter.getAddress()), 100);
    robot->tick(6);
    robot->tick(7);
    robot->tick(8);
    checkPos = robot->getBody()->getPose().getPosition() == Point<>(1, 2, 0);
    checkOr = robot->getBody()->getPose().getOrientation() == DirectionVector<>::UP();
    EXPECT_TRUE(checkPos);
    EXPECT_TRUE(checkOr);
    checkPos = podDock->getPodHolder().getChildPod()->getBody()->getPose().getPosition() == Point<>(1, 1, 1);
    EXPECT_TRUE(checkPos);
}