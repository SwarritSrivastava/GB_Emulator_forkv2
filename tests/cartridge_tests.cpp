#include <gtest/gtest.h>
#include "cartridge.hpp"

TEST(CartridgeTest, LoadValidROM) {
    Cartridge cart;
    std::vector<u8> dummy_rom(0x8000, 0); // 32KB standard ROM allocation
    dummy_rom[0x147] = 0x00; // ROM ONLY type
    dummy_rom[0x148] = 0x00; // 32KB ROM bank mapping
    dummy_rom[0x149] = 0x00; // No RAM requested

    EXPECT_TRUE(cart.load_rom(dummy_rom));
    EXPECT_EQ(cart.get_mbc_type(), 0x00);
    EXPECT_EQ(cart.get_rom_bytes().size(), 0x8000);
}

TEST(CartridgeTest, PadsTinyROM) {
    Cartridge cart;
    std::vector<u8> tiny_rom(0x100, 0); // Critically undersized payload
    
    // The loader should gracefully zero-pad the memory to the hardware standard 32KB
    EXPECT_TRUE(cart.load_rom(tiny_rom));
    EXPECT_EQ(cart.get_rom_bytes().size(), 0x8000);
}

TEST(CartridgeTest, RomOnlyMemoryRead) {
    Cartridge cart;
    std::vector<u8> dummy_rom(0x8000, 0);
    dummy_rom[0x0150] = 0xAA;
    dummy_rom[0x4000] = 0xBB;
    
    cart.load_rom(dummy_rom);

    // Reading should safely retrieve mapped bytes with zero-overhead indexing
    EXPECT_EQ(cart.read(0x0150), 0xAA);
    EXPECT_EQ(cart.read(0x4000), 0xBB);
}
