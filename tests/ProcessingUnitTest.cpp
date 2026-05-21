#include <gtest/gtest.h>
#include "../include/ProcessingUnit.hpp"
#include "../include/mmu.hpp"

class CPUTest : public testing::Test {
protected:
    ProcessingUnit cpu; //Init cpu
    MMU mmu; //Init MMU
};

TEST_F(CPUTest, ResetValuesAreCorrect) {
    // Check 16-bit combined registers
    EXPECT_EQ(cpu.get_af(), 0x01B0);
    EXPECT_EQ(cpu.get_bc(), 0x0013);
    EXPECT_EQ(cpu.get_de(), 0x00D8);
    EXPECT_EQ(cpu.get_hl(), 0x014D);

    // Check individual flags (B0 = 1011 0000)
    EXPECT_TRUE(cpu.get_flag_z());   // Bit 7
    EXPECT_FALSE(cpu.get_flag_n());  // Bit 6
    EXPECT_TRUE(cpu.get_flag_h());   // Bit 5
    EXPECT_TRUE(cpu.get_flag_c());   // Bit 4
}

TEST_F(CPUTest, HaltStateInitial) {
    EXPECT_FALSE(cpu.isHalt());
}

TEST_F(CPUTest, ExecutesHALT)
{
    std::vector<u8> rom(0x8000, 0x00);
    rom[0x0100] = 0x76; // HALT

    mmu.map_rom(rom);

    const int cycles = cpu.step(mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_TRUE(cpu.isHalt());
}

TEST_F(CPUTest, PCIncrementsAfterStep)
{
    std::vector<u8> rom(0x8000, 0x00);
    rom[0x0100] = 0x00; // NOP

    mmu.map_rom(rom);

    cpu.step(mmu);

    EXPECT_EQ(cpu.get_pc(), 0x0101);
}

// TEST_F(CPUTest, HandlesUnknownOpcodeGracefully)
// {
//     // Invalid opcode
//     std::vector<u8> rom(0x8000, 0x00);
//     rom[0x0100] = 0xF0FF;
//     mmu.map_rom(rom);
//
//     const int cycles = cpu.step(mmu);
//
//     // Should not crash
//     EXPECT_EQ(cycles, 4);
//     EXPECT_FALSE(cpu.isHalt());
//     EXPECT_EQ(cpu.get_pc(), 0x0101); // check opcode consumed
// }

TEST_F(CPUTest, ExecutesMultipleNOPs)
{
    // Fill with NOPs
    const std::vector<u8> rom(0x8000, 0x00); // NOPs everywhere
    mmu.map_rom(rom);

    cpu.step(mmu);
    cpu.step(mmu);
    cpu.step(mmu);

    EXPECT_EQ(cpu.get_pc(), 0x0103);
    EXPECT_FALSE(cpu.isHalt());
}