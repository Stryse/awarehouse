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
        env = std::make_unique<ObservableNavEnvironment<Tile>>(X, Y, Z);
        for (int x = 0; x < X; ++x)
            for (int y = 0; y < Y; ++y)
                for (int z = 0; z < Z; ++z)
                    env->getBuffer()[env->getCoordAsIndex(x, y, z)] = std::make_unique<Tile>(Tile::Point{x, y, z});

        // ####################### Setup Large Body ##########################
        largeBody = std::make_unique<Body<ObservableNavEnvironment<Tile>>>(
            Point<int>(1, 1, 0),
            DirectionVector<int>(Directions::UP),
            *env,
            BodyShapeFactory::createShape(BodyShapeFactory::BodyShape::TWO_BLOCKS_HEIGH));

        // ####################### Setup Small Body ##########################
        tinyBody = std::make_unique<Body<ObservableNavEnvironment<Tile>>>(
            Point<int>(1, 1, 2),
            DirectionVector<int>(Directions::UP),
            *env,
            BodyShapeFactory::createShape(BodyShapeFactory::BodyShape::ONLY_ORIGIN));
    }

    static std::unique_ptr<ObservableNavEnvironment<Tile>> env;
    static std::unique_ptr<Body<ObservableNavEnvironment<Tile>>> largeBody;
    static std::unique_ptr<Body<ObservableNavEnvironment<Tile>>> tinyBody;
};

// #################################### SHARED VARIABLES ######################################################
std::unique_ptr<ObservableNavEnvironment<Tile>> BodyTest::env = nullptr;
std::unique_ptr<Body<ObservableNavEnvironment<Tile>>> BodyTest::largeBody = nullptr;
std::unique_ptr<Body<ObservableNavEnvironment<Tile>>> BodyTest::tinyBody = nullptr;
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
    //Detach
    largeBody->detachBody(*tinyBody);
    EXPECT_EQ(largeBody->getChildren().size(), 0) << "[ERROR] Body didn't register children after detachment" << std::endl;
    EXPECT_TRUE(largeBody->getChildren().find(&(*tinyBody)) == largeBody->getChildren().end()) << "[ERROR] child body is still registered in parent" << std::endl;
    EXPECT_EQ(tinyBody->getParent(), nullptr) << "[ERROR] Body still had parent after detachment" << std::endl;
}

TEST_F(BodyTest, SelfMovement)
{
    int testPhase = 0;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<int>(1, 1, 0)) << "[ERROR] Body's initial position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<int>(1, 1, 0)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
    //MoveUp
    largeBody->moveBody(DirectionVector<int>(Directions::UP));
    ++testPhase;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<int>(1, 2, 0)) << "[ERROR] Body's moved (UP) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<int>(1, 2, 0)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
    //MoveRight
    largeBody->moveBody(DirectionVector<int>(Directions::RIGHT));
    ++testPhase;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<int>(2, 2, 0)) << "[ERROR] Body's moved (RIGHT) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<int>(2, 2, 0)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
    //MoveLeft
    largeBody->moveBody(DirectionVector<int>(Directions::LEFT));
    ++testPhase;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<int>(1, 2, 0)) << "[ERROR] Body's moved (LEFT) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<int>(1, 2, 0)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
    //MoveDown
    largeBody->moveBody(DirectionVector<int>(Directions::DOWN));
    ++testPhase;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<int>(1, 1, 0)) << "[ERROR] Body's moved (DOWN) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<int>(1, 1, 0)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
    //Move X_BY_1 && Z_UP_BY_2
    largeBody->moveBody(DirectionVector<int>(1, 0, 2));
    ++testPhase;
    EXPECT_EQ(largeBody->getPose().getPosition(), Point<int>(2, 1, 2)) << "[ERROR] Body's moved (X_BY_1 && Z_UP_BY_2) position was wrong |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(Point<int>(2, 1, 2)).getOccupant() == &(*largeBody)) << "[ERROR] Body not found in the environment at it's location |" << testPhase << std::endl;
    EXPECT_TRUE(env->getVolume(largeBody->getPose().getPosition()).getOccupant() == &(*largeBody)) << "[ERROR] Position doesn't match actual location |" << testPhase << std::endl;
}
