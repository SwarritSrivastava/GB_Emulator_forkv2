#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, OR_B_BitwiseOrAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xF0;
    cpu.reg(ProcessingUnit::Register::B) = 0x0F;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_or_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFF);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, OR_C_SetsZeroWhenBothZero)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_or_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, OR_D_BitwiseOrAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xAA;
    cpu.reg(ProcessingUnit::Register::D) = 0x55;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_or_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFF);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, OR_E_BitwiseOrAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x10;
    cpu.reg(ProcessingUnit::Register::E) = 0x01;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_or_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x11);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, OR_H_BitwiseOrAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x03;
    cpu.reg(ProcessingUnit::Register::H) = 0x30;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_or_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x33);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, OR_L_BitwiseOrAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x44;
    cpu.reg(ProcessingUnit::Register::L) = 0x22;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_or_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x66);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, OR_HL_ReadsMemoryAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x0F;
    cpu.reg(ProcessingUnit::Register::H) = 0xC3;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    mmu.write(0xC300, 0xF0);

    const int cycles = op_or_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFF);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, OR_A_KeepsAAndClearsNHC)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x9C;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_or_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x9C);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, CP_B_CompareSetsFlagsWithoutChangingA)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x3C;
    cpu.reg(ProcessingUnit::Register::B) = 0x3C;
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_cp_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x3C);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, CP_C_HalfBorrowNoCarry)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x10;
    cpu.reg(ProcessingUnit::Register::C) = 0x01;
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_cp_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x10);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, CP_D_CarryWithoutHalfBorrow)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x20;
    cpu.reg(ProcessingUnit::Register::D) = 0x30;
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_cp_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x20);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, CP_E_HalfBorrowAndCarry)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.reg(ProcessingUnit::Register::E) = 0x01;
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_cp_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, CP_H_StandardCompare)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x9A;
    cpu.reg(ProcessingUnit::Register::H) = 0x20;
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_cp_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x9A);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, CP_L_StandardCompare)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x3A;
    cpu.reg(ProcessingUnit::Register::L) = 0x10;
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_cp_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x3A);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, CP_HL_ReadsMemoryAndSetsFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x40;
    cpu.reg(ProcessingUnit::Register::H) = 0xC4;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0x00;
    mmu.write(0xC400, 0x41);

    const int cycles = op_cp_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x40);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, CP_A_AlwaysZeroSet)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x77;
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_cp_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x77);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}
