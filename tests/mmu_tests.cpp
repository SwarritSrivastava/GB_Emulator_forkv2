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