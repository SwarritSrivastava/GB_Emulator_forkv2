#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, LD_B_B_KeepsBUnchanged)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x7A;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_ld_b_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x7A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}

TEST_F(OpcodesCPUTest, LD_B_C_LoadsCIntoB)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x11;
    cpu.reg(ProcessingUnit::Register::C) = 0xA5;
    cpu.reg(ProcessingUnit::Register::F) = 0xB0;

    const int cycles = op_ld_b_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xA5);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xA5);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xB0);
}

TEST_F(OpcodesCPUTest, LD_B_D_LoadsDIntoB)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x11;
    cpu.reg(ProcessingUnit::Register::D) = 0x3C;
    cpu.reg(ProcessingUnit::Register::F) = 0x90;

    const int cycles = op_ld_b_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x3C);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x3C);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x90);
}

TEST_F(OpcodesCPUTest, LD_B_E_LoadsEIntoB)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x11;
    cpu.reg(ProcessingUnit::Register::E) = 0xC3;
    cpu.reg(ProcessingUnit::Register::F) = 0x70;

    const int cycles = op_ld_b_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xC3);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xC3);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x70);
}

TEST_F(OpcodesCPUTest, LD_B_H_LoadsHIntoB)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x11;
    cpu.reg(ProcessingUnit::Register::H) = 0x6E;
    cpu.reg(ProcessingUnit::Register::F) = 0xA0;

    const int cycles = op_ld_b_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x6E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x6E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xA0);
}

TEST_F(OpcodesCPUTest, LD_B_L_LoadsLIntoB)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x11;
    cpu.reg(ProcessingUnit::Register::L) = 0x55;
    cpu.reg(ProcessingUnit::Register::F) = 0xD0;

    const int cycles = op_ld_b_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x55);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x55);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xD0);
}

TEST_F(OpcodesCPUTest, LD_B_HL_LoadsMemoryIntoB)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x88;
    cpu.reg(ProcessingUnit::Register::B) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    mmu.write(0xC088, 0x9B);

    const int cycles = op_ld_b_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x9B);
    EXPECT_EQ(cpu.get_hl(), 0xC088);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}

TEST_F(OpcodesCPUTest, LD_B_A_LoadsAIntoB)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x4D;
    cpu.reg(ProcessingUnit::Register::B) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x50;

    const int cycles = op_ld_b_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x4D);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x4D);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x50);
}
