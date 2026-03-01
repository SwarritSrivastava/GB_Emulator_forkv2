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