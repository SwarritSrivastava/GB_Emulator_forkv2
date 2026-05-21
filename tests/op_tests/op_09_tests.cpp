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
    cpu.reg(ProcessingUnit::Register::C) = 0x01;

    const int cycles = op_sub_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x0F);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SUB_D_Borrow)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x22;
    cpu.reg(ProcessingUnit::Register::D) = 0x30;

    const int cycles = op_sub_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xF2);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SUB_E_ZeroResult)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x40;
    cpu.reg(ProcessingUnit::Register::E) = 0x40;

    const int cycles = op_sub_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SUB_H_HalfBorrowNoCarry)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x10;
    cpu.reg(ProcessingUnit::Register::H) = 0x01;

    const int cycles = op_sub_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x0F);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SUB_L_Standard)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x9A;
    cpu.reg(ProcessingUnit::Register::L) = 0x22;

    const int cycles = op_sub_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x78);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SUB_HL_ReadsMemoryAndBorrows)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(0xC100, 0x01);

    const int cycles = op_sub_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFF);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SUB_A_AlwaysZero)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x77;

    const int cycles = op_sub_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SBC_A_B_UsesCarryIn)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x3E;
    cpu.reg(ProcessingUnit::Register::B) = 0x1D;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_sbc_a_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x20);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SBC_A_C_CarryCreatesBorrow)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_sbc_a_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFF);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SBC_A_D_CanReachZero)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x10;
    cpu.reg(ProcessingUnit::Register::D) = 0x0F;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_sbc_a_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SBC_A_E_NoCarryInBehavesLikeSUB)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x22;
    cpu.reg(ProcessingUnit::Register::E) = 0x11;
    cpu.clearFlags();

    const int cycles = op_sbc_a_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x11);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SBC_A_H_ZeroWithoutBorrow)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x05;
    cpu.reg(ProcessingUnit::Register::H) = 0x04;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_sbc_a_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SBC_A_L_BorrowWithoutHalfBorrow)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x30;
    cpu.reg(ProcessingUnit::Register::L) = 0x40;
    cpu.clearFlags();

    const int cycles = op_sbc_a_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xF0);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SBC_A_HL_ReadsMemory)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x10;
    cpu.reg(ProcessingUnit::Register::H) = 0xC2;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;
    mmu.write(0xC200, 0x00);

    const int cycles = op_sbc_a_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x0F);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, SBC_A_A_WithCarryLeavesFF)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x8B;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_sbc_a_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFF);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
}
