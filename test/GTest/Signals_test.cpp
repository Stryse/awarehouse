#include "Battery.h"
#include "DeliveryRobot.h"
#include "MotorAction.h"
#include "Network.h"
#include "NetworkAdapter.h"
#include "ObservableEnvironment.h"
#include "PodDock.h"
#include "Tile.h"
#include "gtest/gtest.h"
#include <memory>
#include <queue>

class NotifyTest : public testing::Test
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
                    env->getBuffer()[env->getCoordAsIndex(x, y, z)] = std::make_shared<Tile>(Tile::Point{x, y, z});

        // Replace tile with PodDocks
        podDock = std::make_shared<PodDock<>>(PodDock<>::Point{1, 1, 0});
        env->getBuffer()[env->getCoordAsIndex(1, 1, 0)] = podDock;
        podDock->addAssociatedPod(env);

        // Init robot
        robot = std::make_unique<DeliveryRobot<>>(env, Point<>(1, 1, 0), DirectionVector<>::UP());
    }

    static std::shared_ptr<ObservableNavEnvironment<>> env;
    static std::shared_ptr<PodDock<>> podDock;
    static std::unique_ptr<DeliveryRobot<>> robot;

    using MotorAction = ::MotorAction<std::decay_t<decltype(*robot)>::Body, std::decay_t<decltype(*robot)>::Energy>;
    static void moveHelper(std::queue<MotorAction *> moveActionQueue);
};

std::shared_ptr<ObservableNavEnvironment<>> NotifyTest::env = nullptr;
std::unique_ptr<DeliveryRobot<>> NotifyTest::robot = nullptr;
std::shared_ptr<PodDock<>> NotifyTest::podDock = nullptr;

void NotifyTest::moveHelper(std::queue<NotifyTest::MotorAction *> moveActionQueue)
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

TEST_F(NotifyTest, BatterySignals)
{
    Battery<int> battery(100);

    int signalReceived = 0;
    battery.onChargeChanged.connect([&](const int &charge) { ++signalReceived; });

    // CHECK DEPLETION
    battery.deplete(1);
    EXPECT_EQ(signalReceived, 1);

    // CHECK CHARGE
    battery.charge(1);
    EXPECT_EQ(signalReceived, 2);
}

TEST_F(NotifyTest, AgentMovementNotify)
{
    int signalReceived = 0;

    // Connects
    robot->getMoveMechanism()->onBodyMoved.connect([&](const Body<ObservableNavEnvironment<>> &body) {
        ++signalReceived;
    });

    // Move Up
    NotifyTest::moveHelper(robot->getMoveMechanism()->move(DirectionVector<>::UP()));
    EXPECT_EQ(signalReceived, 1);

    // Move Down
    NotifyTest::moveHelper(robot->getMoveMechanism()->move(DirectionVector<>::DOWN()));
    EXPECT_EQ(signalReceived, 4);
}

TEST_F(NotifyTest, AgentPodPickup_PutDown)
{
    int signalReceived = 0;

    robot->getRackMotor().onPodPickedUp.connect([&](const Body<ObservableNavEnvironment<>> &body) {
        ++signalReceived;
    });

    robot->getRackMotor().onPodPutDown.connect([&](const Body<ObservableNavEnvironment<>> &body) {
        ++signalReceived;
    });

    // Pick up Pod
    (*robot->getRackMotor().pickUpPodAction())();
    EXPECT_EQ(signalReceived, 1);

    // Put down Pod
    (*robot->getRackMotor().putDownPodAction())();
    EXPECT_EQ(signalReceived, 2);
}

TEST_F(NotifyTest, AgentMovement_With_PodMovement)
{
    // Connect Pod Movement
    int podMoved = 0;
    podDock->getPodHolder().getChildPod()->onBodyMoved.connect([&](const Body<ObservableNavEnvironment<>> &body) {
        ++podMoved;
    });

    std::shared_ptr<Network> network = std::make_shared<Network>();

    // Connect Controller
    NetworkAdapter adapter;
    adapter.connectWithAddress(network, 0x1);

    // Connect Robot
    robot->getNetworkAdapter().connect(network);

    // Send AgentControlGranted message
    adapter.send(std::make_unique<AgentControlGrantedMessage>(adapter.getAddress()), 100);
    robot->tick(0);
    // Send Pickup Message
    adapter.send(std::make_unique<PickupPodMessage>(adapter.getAddress()), 100);

    // Robot Action perform (Pickup pod)
    robot->tick(0);

    // Movement With Pod
    adapter.send(std::make_unique<MoveAgentMessage>(DirectionVector<>::UP(), adapter.getAddress()), 100);
    robot->tick(1);
    EXPECT_EQ(podMoved, 1);

    // Move back with Pod
    adapter.send(std::make_unique<MoveAgentMessage>(DirectionVector<>::DOWN(), adapter.getAddress()), 100);
    robot->tick(2);
    EXPECT_EQ(podMoved, 2);
    robot->tick(3);
    EXPECT_EQ(podMoved, 3);
    robot->tick(4);
    EXPECT_EQ(podMoved, 4);

    // Put down pod
    adapter.send(std::make_unique<PutDownPodMessage>(adapter.getAddress()), 100);
    robot->tick(5);

    // Move up without Pod
    adapter.send(std::make_unique<MoveAgentMessage>(DirectionVector<>::UP(), adapter.getAddress()), 100);
    robot->tick(6);
    EXPECT_EQ(podMoved, 4);
    robot->tick(7);
    EXPECT_EQ(podMoved, 4);
    robot->tick(8);
    EXPECT_EQ(podMoved, 4);
}