#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, SUB_B_StandardNoBorrow)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x15;
    cpu.reg(ProcessingUnit::Register::B) = 0x05;

    const int cycles = op_sub_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x10);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SUB_C_HalfCarry)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x10;
    cpu.reg(ProcessingUnit::Register::C) = 0x01; // 0x10 - 0x01 will borrow from bit 4

    const int cycles = op_sub_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x0F);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h()); // Half carry should be set
    EXPECT_FALSE(cpu.get_flag_c());
}
