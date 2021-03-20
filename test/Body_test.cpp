#include "Body.h"
#include "BodyShapeFactory.h"
#include "ObservableEnvironment.h"
#include "Tile.h"
#include "gtest/gtest.h"
#include <memory>

class BodyTest : public testing::Test
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
                    env->getBuffer()[env->getCoordAsIndex(x, y, z)] = std::make_unique<Tile>(Tile::Point{x, y, z});

        // ####################### Setup Large Body ##########################
        largeBody = std::make_unique<Body<>>(
            Point<>(1, 1, 0),
            DirectionVector<>::UP(),
            env,
            BodyShapeFactory<>::twoBlockHeigh());

        // ####################### Setup Small Body ##########################
        tinyBody = std::make_unique<Body<>>(
            Point<>(1, 1, 2),
            DirectionVector<>::UP(),
            env,
            BodyShapeFactory<>::onlyOrigin());
    }

    static std::shared_ptr<ObservableNavEnvironment<>> env;
    static std::unique_ptr<Body<>> largeBody;
    static std::unique_ptr<Body<>> tinyBody;
};

// #################################### SHARED VARIABLES ######################################################
std::shared_ptr<ObservableNavEnvironment<>> BodyTest::env = nullptr;
std::unique_ptr<Body<>> BodyTest::largeBody = nullptr;
std::unique_ptr<Body<>> BodyTest::tinyBody = nullptr;
// ####################################### TESTS ##############################################################

TEST_F(BodyTest, AttachChildDetachChild)
{
    EXPECT_EQ(largeBody->getChildren().size(), 0) << "[ERROR] Body had children after initialization" << std::endl;
    EXPECT_EQ(largeBody->getParent(), nullptr) << "[ERROR] Body had parent after initialization" << std::endl;
    //Attach
    largeBody->attachBody(*tinyBody);
    EXPECT_EQ(largeBody->getChildren().size(), 1) << "[ERROR] Body didn't register children after attachment" << std::endl;
    EXPECT_FALSE(largeBody->getChildren().find(&(*tinyBody)) == largeBody->getChildren().end()) << "[ERROR] child body is not registered in parent" << std::endl;
    EXPECT_EQ(tinyBody->getParent(), &(*largeBody)) << "[ERROR] Wrong parent" << std::endl;

    std::unique_ptr<Body<>> fakeParent = std::make_unique<Body<>>(Point<>(2, 2, 2), DirectionVector<>::UP(), env, BodyShapeFactory<>::onlyOrigin());
    fakeParent->attachBody(*tinyBody);
    //Check no attachment on fake parent
    EXPECT_EQ(tinyBody->getParent(), &(*largeBody));
    EXPECT_EQ(fakeParent->getChildren().size(), 0);

    //Detach
    largeBody->detachBody(*tinyBody);
    EXPECT_EQ(largeBody->getChildren().size(), 0) << "[ERROR] Body didn't register children after detachment" << std::endl;
    EXPECT_TRUE(largeBody->getChildren().find(&(*tinyBody)) == largeBody->getChildren().end()) << "[ERROR] child body is still registered in parent" << std::endl;
    EXPECT_EQ(tinyBody->getParent(), nullptr) << "[ERROR] Body still had parent after detachment" << std::endl;
}

TEST_F(BodyTest, SelfMovement)
{
    int testPhase = 0;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<>(1, 1, 0)) << "[ERROR] Body's initial position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(1, 1, 0)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
    //MoveUp
    largeBody->moveBody(DirectionVector<>::UP());
    ++testPhase;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<>(1, 2, 0)) << "[ERROR] Body's moved (UP) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(1, 2, 0)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
    //MoveRight
    largeBody->moveBody(DirectionVector<>::RIGHT());
    ++testPhase;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<>(2, 2, 0)) << "[ERROR] Body's moved (RIGHT) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(2, 2, 0)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
    //MoveLeft
    largeBody->moveBody(DirectionVector<>::LEFT());
    ++testPhase;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<>(1, 2, 0)) << "[ERROR] Body's moved (LEFT) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(1, 2, 0)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
    //MoveDown
    largeBody->moveBody(DirectionVector<>::DOWN());
    ++testPhase;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<>(1, 1, 0)) << "[ERROR] Body's moved (DOWN) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(1, 1, 0)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
    //Move X_BY_1 && Z_UP_BY_2
    largeBody->moveBody(DirectionVector<>(1, 0, 2));
    ++testPhase;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<>(2, 1, 2)) << "[ERROR] Body's moved (X_BY_1 && Z_UP_BY_2) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(2, 1, 2)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
}

TEST_F(BodyTest, ChildCarrying)
{
    largeBody->attachBody(*tinyBody);
    // Parent move up
    int testPhase = 0;
    largeBody->moveBody(DirectionVector<>::UP());
    EXPECT_EQ(tinyBody->getPose().getPosition(), Point<>(1, 2, 2)) << "[ERROR] Child moved (UP) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(1, 2, 2)).getOccupant() == &(*tinyBody)) << "[ERROR] Child not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(tinyBody->getPose().getPosition()).getOccupant() == &(*tinyBody)) << "[ERROR] Child Position doesn't match actual location |" << testPhase << std::endl;
    // Parent move right
    ++testPhase;
    largeBody->moveBody(DirectionVector<>::RIGHT());
    EXPECT_EQ(tinyBody->getPose().getPosition(), Point<>(2, 2, 2)) << "[ERROR] Child moved (RIGHT) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(2, 2, 2)).getOccupant() == &(*tinyBody)) << "[ERROR] Child not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(tinyBody->getPose().getPosition()).getOccupant() == &(*tinyBody)) << "[ERROR] Child Position doesn't match actual location |" << testPhase << std::endl;
    // Parent move left
    ++testPhase;
    largeBody->moveBody(DirectionVector<>::LEFT());
    EXPECT_EQ(tinyBody->getPose().getPosition(), Point<>(1, 2, 2)) << "[ERROR] Child moved (LEFT) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(1, 2, 2)).getOccupant() == &(*tinyBody)) << "[ERROR] Child not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(tinyBody->getPose().getPosition()).getOccupant() == &(*tinyBody)) << "[ERROR] Child Position doesn't match actual location |" << testPhase << std::endl;
    // Parent move down
    ++testPhase;
    largeBody->moveBody(DirectionVector<>::DOWN());
    EXPECT_EQ(tinyBody->getPose().getPosition(), Point<>(1, 1, 2)) << "[ERROR] Child moved (DOWN) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(1, 1, 2)).getOccupant() == &(*tinyBody)) << "[ERROR] Child not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(tinyBody->getPose().getPosition()).getOccupant() == &(*tinyBody)) << "[ERROR] Child Position doesn't match actual location |" << testPhase << std::endl;
    // Parent Move X_BY_1 && Z_UP_BY_2
    ++testPhase;
    largeBody->moveBody(DirectionVector<>(1, 0, 2));
    EXPECT_EQ(tinyBody->getPose().getPosition(), Point<>(2, 1, 4)) << "[ERROR] Child moved (X_BY_1 && Z_UP_BY_2) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<>(2, 1, 4)).getOccupant() == &(*tinyBody)) << "[ERROR] Child not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(tinyBody->getPose().getPosition()).getOccupant() == &(*tinyBody)) << "[ERROR] Child Position doesn't match actual location |" << testPhase << std::endl;
}

TEST_F(BodyTest, ProperDestruction_References)
{
    //Attach Child
    largeBody->attachBody(*tinyBody);
    largeBody = std::make_unique<Body<>>(
        Point<>(4, 4, 4),
        DirectionVector<>::LEFT(),
        env,
        BodyShapeFactory<>::onlyOrigin());
    // Original parent destroyed
    EXPECT_EQ(tinyBody->getParent(), nullptr) << "[ERROR] Child still has record of destroyed parent" << std::endl;
    // Destroy again
    largeBody = nullptr;
    EXPECT_EQ(tinyBody->getParent(), nullptr) << "[ERROR] Child still has record of destroyed parent" << std::endl;
    // Renew parent
    largeBody = std::make_unique<Body<>>(
        Point<>(4, 4, 4),
        DirectionVector<>::LEFT(),
        env,
        BodyShapeFactory<>::onlyOrigin());
    // Attach
    largeBody->attachBody(*tinyBody);
    // Destroy child
    tinyBody = std::make_unique<Body<>>(
        Point<>(4, 4, 5),
        DirectionVector<>::LEFT(),
        env,
        BodyShapeFactory<>::onlyOrigin());
    EXPECT_EQ(largeBody->getChildren().size(), 0) << "[ERROR] Parent still has record of destroyed child" << std::endl;
    EXPECT_TRUE(largeBody->getChildren().find(&(*tinyBody)) == largeBody->getChildren().end()) << "[ERROR] Parent still has record of destroyed child" << std::endl;
    // Destroy child again
    tinyBody = nullptr;
    EXPECT_EQ(largeBody->getChildren().size(), 0) << "[ERROR] Parent still has record of destroyed child" << std::endl;
    EXPECT_TRUE(largeBody->getChildren().find(&(*tinyBody)) == largeBody->getChildren().end()) << "[ERROR] Parent still has record of destroyed child" << std::endl;
}

TEST_F(BodyTest, ProperDestruction_Environment_Leaving)
{
    largeBody->attachBody(*tinyBody);
    EXPECT_EQ(env->getVolume(largeBody->getPose().getPosition()).getOccupant(), &(*largeBody)) << "[ERROR] Body is not in it's place in environment" << std::endl;
    EXPECT_FALSE(env->getVolume(largeBody->getPose().getPosition()).isVolumeFree()) << "[Error] Occupied volume is marked free" << std::endl;

    EXPECT_EQ(env->getVolume(tinyBody->getPose().getPosition()).getOccupant(), &(*tinyBody)) << "[ERROR] Body is not in it's place in environment" << std::endl;
    EXPECT_FALSE(env->getVolume(tinyBody->getPose().getPosition()).isVolumeFree()) << "[Error] Occupied volume is marked free" << std::endl;

    // Destroy parent
    const Point<> lastPos = largeBody->getPose().getPosition();
    void *lastAddress = &(*largeBody);
    largeBody = nullptr;

    // Parent leaves
    EXPECT_NE(env->getVolume(lastPos).getOccupant(), lastAddress) << "[ERROR] Body remained in the environment after destruction" << std::endl;
    EXPECT_EQ(env->getVolume(lastPos).getOccupant(), nullptr) << "[ERROR] Body remained in the environment after destruction" << std::endl;
    EXPECT_TRUE(env->getVolume(lastPos).isVolumeFree()) << "[Error] Occupied volume is marked occupied after leaving" << std::endl;

    // Child remains
    EXPECT_EQ(env->getVolume(tinyBody->getPose().getPosition()).getOccupant(), &(*tinyBody)) << "[ERROR] Child left with parent" << std::endl;
    EXPECT_FALSE(env->getVolume(tinyBody->getPose().getPosition()).isVolumeFree()) << "[Error] Occupied volume is marked free" << std::endl;
}