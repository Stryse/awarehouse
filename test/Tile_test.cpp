#include "Tile.h"
#include "mocks/MockVolumeOccupant.h"
#include "gtest/gtest.h"
#include <memory>

class TileTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        tile = Tile(Point<>(0, 1, 2));
        occupant = std::make_unique<MockVolumeOccupant<Tile>>();
        occupant_2 = std::make_unique<MockVolumeOccupant<Tile>>();
    }

    // Shared vars
    static std::unique_ptr<MockVolumeOccupant<Tile>> occupant;
    static std::unique_ptr<MockVolumeOccupant<Tile>> occupant_2;
    static Tile tile;
};

// #################################### SHARED VARIABLES ######################################################

std::unique_ptr<MockVolumeOccupant<Tile>> TileTest::occupant = nullptr;
std::unique_ptr<MockVolumeOccupant<Tile>> TileTest::occupant_2 = nullptr;
Tile TileTest::tile = Tile(Point<>(0, 0, 0));

// ####################################### TESTS ##############################################################

TEST_F(TileTest, Occupation_WasFree) // Térfoglaló térfoglalásának tesztelése üres térfogategység esetén
{
    EXPECT_TRUE(tile.isVolumeFree()) << "[ERROR] Volume was not free in the beginning" << std::endl;
    EXPECT_NO_THROW(tile.occupyV(occupant.get())) << "[ERROR] Occupant couldn't occupy empty tile" << std::endl;
    EXPECT_FALSE(tile.isVolumeFree()) << "[ERROR] Volume was free after occupation" << std::endl;
}

TEST_F(TileTest, Occupation_WasNotFree) // Térfoglaló térfoglalásának tesztelése NEM üres térfogategység esetén
{
    tile.occupyV(occupant.get());
    EXPECT_FALSE(tile.isVolumeFree()) << "[ERROR] Volume was free in the beginning" << std::endl;
    EXPECT_NO_THROW(tile.occupyV(occupant.get())) << "[ERROR] Wrong Self-collision exception" << std::endl;
    EXPECT_THROW(tile.occupyV(occupant_2.get()), Tile::VolumeCollisionException) << "[ERROR] No collision happened" << std::endl;
}

TEST_F(TileTest, Freeing_WasNotFree) // Térfoglaló térelhagyásának tesztelése NEM üres térfogategység esetén
{
    tile.occupyV(occupant.get());
    EXPECT_FALSE(tile.isVolumeFree()) << "[ERROR] Volume was free in the beginning" << std::endl;
    EXPECT_NO_THROW(tile.freeV()) << "[ERROR] Freeing unsuccessful" << std::endl;
    EXPECT_TRUE(tile.isVolumeFree()) << "[ERROR] Volume didn't become free after freeing" << std::endl;
}

TEST_F(TileTest, Freeing_WasFree) // Térfoglaló térelhagyásának tesztelése üres térfogategység esetén
{
    EXPECT_TRUE(tile.isVolumeFree()) << "[ERROR] Volume was not free in the beginning" << std::endl;
    EXPECT_NO_THROW(tile.freeV()) << "[ERROR] Freeing unsuccessful" << std::endl;
    EXPECT_TRUE(tile.isVolumeFree()) << "[ERROR] Volume didn't become free after freeing" << std::endl;
}

TEST_F(TileTest, GetOccupantsOnNotFree) // Térfoglaló lekérésének tesztelése NEM üres térfogategység esetén
{
    tile.occupyV(occupant.get());
    EXPECT_FALSE(tile.isVolumeFree()) << "[ERROR] Volume was free in the beginning" << std::endl;
    EXPECT_EQ(tile.getOccupant(), occupant.get());
}

TEST_F(TileTest, GetOccupantsOnFree) // Térfoglaló lekérésének tesztelése üres térfogategység esetén
{
    EXPECT_TRUE(tile.isVolumeFree()) << "[ERROR] Volume was not free in the beginning" << std::endl;
    EXPECT_EQ(tile.getOccupant(), nullptr);
}

TEST_F(TileTest, TilePositionEQ)
{
    bool equal = tile.getPosition() == Tile::Point(0, 1, 2);
    EXPECT_TRUE(equal);
}