#include <gtest/gtest.h>
#include "../include/mmu.hpp"

TEST(MMUTest, InitializationZerosMemory) {
    MMU mmu;
    EXPECT_EQ(mmu.read(0x0150), 0x00); 
    EXPECT_EQ(mmu.read(0x8000), 0x00);
}

TEST(MMUTest, CanWriteToValidMemory) {
    MMU mmu;
    mmu.write(0x8000, 0xAB);
    EXPECT_EQ(mmu.read(0x8000), 0xAB);
}

TEST(MMUTest, CannotWriteToROM) {
    MMU mmu;
    mmu.write(0x0100, 0xFF);
    EXPECT_EQ(mmu.read(0x0100), 0x00); 
}

TEST(MMUTest, MapRomAndSimulateFetch) {
    MMU mmu;
    
    std::vector<u8> dummy_rom(0x150, 0x00);
    dummy_rom[0x0100] = 0xC3; 

    EXPECT_TRUE(mmu.map_rom(dummy_rom));
    EXPECT_EQ(mmu.read(0x0100), 0xC3);
}

TEST(MMUTest, MapRomFailsOnEmptyRom) {
    MMU mmu;
    std::vector<u8> empty_rom;
    EXPECT_FALSE(mmu.map_rom(empty_rom)); 
}

TEST(MMUTest, MapRomTruncatesOversizedRom) {
    MMU mmu;
    std::vector<u8> oversized_rom(0x9000, 0xAA);
    
    EXPECT_TRUE(mmu.map_rom(oversized_rom));

    EXPECT_EQ(mmu.read(0x7FFF), 0xAA);
    EXPECT_EQ(mmu.read(0x8000), 0x00); 
}

TEST(MMUTest, WramValuesPersist) {
    MMU mmu;
    mmu.write(0xC000, 0xAA); // first byte of wram
    mmu.write(0xC500, 0x42); // middle of wram
    mmu.write(0xCAAA, 0xFF); // random spot
    mmu.write(0xDFFF, 0xBB); // last byte of wram

    EXPECT_EQ(mmu.read(0xC000), 0xAA);
    EXPECT_EQ(mmu.read(0xC500), 0x42);
    EXPECT_EQ(mmu.read(0xCAAA), 0xFF);
    EXPECT_EQ(mmu.read(0xDFFF), 0xBB);
}