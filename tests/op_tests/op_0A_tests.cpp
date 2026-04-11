#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, AND_B_BitwiseAndAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xF0;
    cpu.reg(ProcessingUnit::Register::B) = 0x3C;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_and_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x30);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, AND_C_SetsZeroWhenResultZero)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x0F;
    cpu.reg(ProcessingUnit::Register::C) = 0xF0;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_and_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, AND_D_BitwiseAndAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xAA;
    cpu.reg(ProcessingUnit::Register::D) = 0xCC;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_and_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x88);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, AND_E_BitwiseAndAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x5A;
    cpu.reg(ProcessingUnit::Register::E) = 0x0F;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_and_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x0A);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, AND_H_BitwiseAndAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x3F;
    cpu.reg(ProcessingUnit::Register::H) = 0x33;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_and_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x33);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, AND_L_BitwiseAndAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xF5;
    cpu.reg(ProcessingUnit::Register::L) = 0x5F;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_and_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x55);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, AND_HL_ReadsMemoryAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xF0;
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    mmu.write(0xC100, 0x0F);

    const int cycles = op_and_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, AND_A_KeepsAAndSetsH)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x9C;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_and_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x9C);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, XOR_B_BitwiseXorAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xF0;
    cpu.reg(ProcessingUnit::Register::B) = 0x0F;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_xor_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFF);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, XOR_C_SetsZeroWhenSame)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x5A;
    cpu.reg(ProcessingUnit::Register::C) = 0x5A;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_xor_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, XOR_D_BitwiseXorAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xAA;
    cpu.reg(ProcessingUnit::Register::D) = 0xCC;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_xor_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x66);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, XOR_E_BitwiseXorAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x3C;
    cpu.reg(ProcessingUnit::Register::E) = 0x0F;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_xor_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x33);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, XOR_H_BitwiseXorAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x81;
    cpu.reg(ProcessingUnit::Register::H) = 0x01;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_xor_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x80);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, XOR_L_BitwiseXorAndFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x55;
    cpu.reg(ProcessingUnit::Register::L) = 0xAA;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_xor_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFF);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, XOR_HL_ReadsMemoryAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xF0;
    cpu.reg(ProcessingUnit::Register::H) = 0xC2;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    mmu.write(0xC200, 0xF0);

    const int cycles = op_xor_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(OpcodesCPUTest, XOR_A_AlwaysClearsA)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x9C;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_xor_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
}
