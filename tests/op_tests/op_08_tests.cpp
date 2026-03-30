#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, ADD_A_B_AddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x8A;
    cpu.reg(ProcessingUnit::Register::B) = 0x23;
    cpu.reg(ProcessingUnit::Register::F) = 0x80; // initial flags shouldn't matter

    const int cycles = op_add_a_b(cpu, mmu);
    EXPECT_EQ(cycles, 4);

    // sum = 0x8A + 0x23 = 0xAD
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xAD);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x23); // unchanged
    EXPECT_EQ(cpu.get_flag_z(), 0); // result != 0
    EXPECT_EQ(cpu.get_flag_n(), 0); // always reset
    EXPECT_EQ(cpu.get_flag_h(), 0); // (0xA + 0x3 = 0xD) → no half carry
    EXPECT_EQ(cpu.get_flag_c(), 0); // no overflow beyond 0xFF
}

TEST_F(OpcodesCPUTest, ADD_A_C_AddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x8A;
    cpu.reg(ProcessingUnit::Register::C) = 0x23;
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_add_a_c(cpu, mmu);
    EXPECT_EQ(cycles, 4);

    // 0x8A + 0x23 = 0xAD
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xAD);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x23);

    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}