#include "Tile.h"
#include "mocks/MockVolumeOccupant.h"
#include "gtest/gtest.h"
#include <memory>

class TileTest : public testing::Test
{
protected:
    virtual void SetUp()
    {
        occupant = std::make_shared<MockVolumeOccupant>();
        occupant_2 = std::make_shared<MockVolumeOccupant>();
        tile = Tile(Point<>(0, 0, 0));
    }

    // Shared vars
    static Tile::OccupantType occupant;
    static Tile::OccupantType occupant_2;
    static Tile tile;
};

// #################################### SHARED VARIABLES ######################################################

Tile::OccupantType TileTest::occupant = nullptr;
Tile::OccupantType TileTest::occupant_2 = nullptr;
Tile TileTest::tile = Tile(Point<>(0, 0, 0));

// ####################################### TESTS ##############################################################

TEST_F(TileTest, Occupation_WasFree) // Térfoglaló térfoglalásának tesztelése üres térfogategység esetén
{
    EXPECT_TRUE(tile.isVolumeFree()) << "[ERROR] Volume was not free in the beginning" << std::endl;
    EXPECT_NO_THROW(tile.occupyV(occupant)) << "[ERROR] Occupant couldn't occupy empty tile" << std::endl;
    EXPECT_FALSE(tile.isVolumeFree()) << "[ERROR] Volume was free after occupation" << std::endl;
}

TEST_F(TileTest, Occupation_WasNotFree) // Térfoglaló térfoglalásának tesztelése NEM üres térfogategység esetén
{
    tile.occupyV(occupant);
    EXPECT_FALSE(tile.isVolumeFree()) << "[ERROR] Volume was free in the beginning" << std::endl;
    EXPECT_NO_THROW(tile.occupyV(occupant)) << "[ERROR] Wrong Self-collision exception" << std::endl;
    EXPECT_THROW(tile.occupyV(occupant_2), Tile::VolumeCollisionException) << "[ERROR] No collision happened" << std::endl;
}

TEST_F(TileTest, Freeing_WasNotFree) // Térfoglaló térelhagyásának tesztelése NEM üres térfogategység esetén
{
    tile.occupyV(occupant);
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
    tile.occupyV(occupant);
    EXPECT_FALSE(tile.isVolumeFree()) << "[ERROR] Volume was free in the beginning" << std::endl;
    EXPECT_EQ(tile.getOccupant(), occupant);
}

TEST_F(TileTest, GetOccupantsOnFree) // Térfoglaló lekérésének tesztelése üres térfogategység esetén
{
    EXPECT_TRUE(tile.isVolumeFree()) << "[ERROR] Volume was not free in the beginning" << std::endl;
    EXPECT_EQ(tile.getOccupant(), nullptr);
}