#include "Battery.h"
#include "DeliveryRobot.h"
#include "MotorAction.h"
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

    bool signalReceived = false;
    battery.onChargeChanged.connect([&](const int &charge) { signalReceived = true; });

    // CHECK DEPLETION
    battery.deplete(1);
    EXPECT_TRUE(signalReceived);

    // CHECK CHARGE
    signalReceived = false;
    battery.charge(1);
    EXPECT_TRUE(signalReceived);
}

TEST_F(NotifyTest, AgentMovementNotify)
{
    bool signalReceived = false;

    // Connects
    robot->getMoveMechanism()->onBodyMoved.connect([&](const Body<ObservableNavEnvironment<>> &body) {
        signalReceived = true;
    });

    // Move Up
    NotifyTest::moveHelper(robot->getMoveMechanism()->move(DirectionVector<>::UP()));
    EXPECT_TRUE(signalReceived);

    // Move Down
    signalReceived = false;
    NotifyTest::moveHelper(robot->getMoveMechanism()->move(DirectionVector<>::DOWN()));
    EXPECT_TRUE(signalReceived);
}

TEST_F(NotifyTest, AgentPodPickup_PutDown)
{
    bool signalReceived = false;

    robot->getRackMotor().onPodPickedUp.connect([&](const Body<ObservableNavEnvironment<>> &body) {
        signalReceived = true;
    });

    robot->getRackMotor().onPodPutDown.connect([&](const Body<ObservableNavEnvironment<>> &body) {
        signalReceived = true;
    });

    // Pick up Pod
    (*robot->getRackMotor().pickUpPodAction())();
    EXPECT_TRUE(signalReceived);

    // Put down Pod
    signalReceived = false;
    (*robot->getRackMotor().putDownPodAction())();
    EXPECT_TRUE(signalReceived);
}