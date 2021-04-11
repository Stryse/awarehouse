#include "Battery.h"
#include "ChargingStation.h"
#include "DeliveryRobot.h"
#include "ObservableEnvironment.h"
#include "OrderModel.h"
#include "Pod.h"
#include "PodDock.h"
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
    // Setup Environment
    ObservableNavEnvironment<> env{3, 3, 3};
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 3; ++z)
            {
                env.getBuffer()[env.getCoordAsIndex(x, y, z)] =
                    std::make_shared<decltype(env)::VolumeType>(decltype(env)::VolumeType::Point{x, y, z});

                // Check Point Sanity
                EXPECT_EQ(env.getVolume(decltype(env)::Point(x, y, z))->getPosition().getPosX(), x) << "[ERROR] Wrong coordinate X";
                EXPECT_EQ(env.getVolume(decltype(env)::Point(x, y, z))->getPosition().getPosY(), y) << "[ERROR] Wrong coordinate Y";
                EXPECT_EQ(env.getVolume(decltype(env)::Point(x, y, z))->getPosition().getPosZ(), z) << "[ERROR] Wrong coordinate Z";
            }
}

TEST_F(ObservableNavEnvironmentTest, chargingStationChargeTest)
{
    // SETUP Battery and check Sanity
    Battery<int> battery(100);
    EXPECT_EQ(battery.getCharge(), 100);
    battery.deplete(100);
    EXPECT_EQ(battery.getCharge(), 0);

    // Setup signal
    ChargeSignal chargeSignal;
    int counter = 0;

    // Construct environment
    ObservableNavEnvironment<> env{3, 3, 3};
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 3; ++z)
            {
                env.getBuffer()[env.getCoordAsIndex(x, y, z)] =
                    std::make_shared<ChargingStation<int>>(decltype(env)::VolumeType::Point{x, y, z}, 3);

                //Send Charge signal
                env.getVolume(Point<>(x, y, z))->receive(battery, chargeSignal);

                //Should be charged
                EXPECT_EQ(battery.getCharge(), ++counter);
            }
    // Check PostCondition
    EXPECT_EQ(battery.getCharge(), 27);
}

TEST_F(ObservableNavEnvironmentTest, PodTransfer)
{
    std::vector<std::shared_ptr<PodDock<>>> docks;
    std::shared_ptr<ObservableNavEnvironment<>> env = std::make_shared<ObservableNavEnvironment<>>(3, 3, 3);
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 3; ++z)
            {
                docks.push_back(std::make_shared<PodDock<>>(Point<>(x, y, z)));
                env->getBuffer()[env->getCoordAsIndex(x, y, z)] = docks[docks.size() - 1];
            }
    docks[1]->addAssociatedPod(env);
    docks[1]->getPodHolder().getChildPod()->push(std::make_unique<OrderModel>(2));
    docks[1]->getPodHolder().getChildPod()->push(std::make_unique<OrderModel>(3));
    docks[1]->getPodHolder().getChildPod()->push(std::make_unique<OrderModel>(5));
    docks[1]->getPodHolder().getChildPod()->push(std::make_unique<OrderModel>(2));

    EXPECT_NE(docks[1]->getPodHolder().getChildPod(), nullptr);
    EXPECT_EQ(docks[1]->getPodHolder().getChildPod()->getInventory().size(), 3);

    DeliveryRobot<> robot(env, Point<>(0, 0, 0), DirectionVector<>::UP());
    PickupPodSignal pps;
    docks[1]->receive(robot.getPodHolder(), pps);

    EXPECT_EQ(docks[1]->getPodHolder().getChildPod(), nullptr);
    EXPECT_NE(robot.getPodHolder().getChildPod(), nullptr);
    EXPECT_EQ(robot.getPodHolder().getChildPod()->getInventory().size(), 3);
}