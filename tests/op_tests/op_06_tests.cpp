#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, LD_H_B_LoadsBIntoH)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x7A;
    cpu.reg(ProcessingUnit::Register::H) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_ld_h_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x7A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x7A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}

TEST_F(OpcodesCPUTest, LD_H_C_LoadsCIntoH)
{
    cpu.reg(ProcessingUnit::Register::C) = 0xA5;
    cpu.reg(ProcessingUnit::Register::H) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xB0;

    const int cycles = op_ld_h_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xA5);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xA5);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xB0);
}

TEST_F(OpcodesCPUTest, LD_H_D_LoadsDIntoH)
{
    cpu.reg(ProcessingUnit::Register::D) = 0x3C;
    cpu.reg(ProcessingUnit::Register::H) = 0x3C;
    cpu.reg(ProcessingUnit::Register::F) = 0x90;

    const int cycles = op_ld_h_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x3C);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x3C);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x90);
}

TEST_F(OpcodesCPUTest, LD_H_E_LoadsEIntoH)
{
    cpu.reg(ProcessingUnit::Register::E) = 0xC3;
    cpu.reg(ProcessingUnit::Register::H) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x70;

    const int cycles = op_ld_h_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xC3);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xC3);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x70);
}

TEST_F(OpcodesCPUTest, LD_H_H_KeepsHUnchanged)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x6E;
    cpu.reg(ProcessingUnit::Register::F) = 0xA0;

    const int cycles = op_ld_h_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x6E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xA0);
}

TEST_F(OpcodesCPUTest, LD_H_L_LoadsLIntoH)
{
    cpu.reg(ProcessingUnit::Register::L) = 0x55;
    cpu.reg(ProcessingUnit::Register::H) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xD0;

    const int cycles = op_ld_h_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x55);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x55);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xD0);
}

TEST_F(OpcodesCPUTest, LD_H_HL_LoadsMemoryIntoH)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x88;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    mmu.write(0xC088, 0x9B);

    const int cycles = op_ld_h_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x9B);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x88);
    EXPECT_EQ(cpu.get_hl(), 0x9B88);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}

TEST_F(OpcodesCPUTest, LD_H_A_LoadsAIntoH)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x4D;
    cpu.reg(ProcessingUnit::Register::H) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x50;

    const int cycles = op_ld_h_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x4D);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x4D);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x50);
}

TEST_F(OpcodesCPUTest, LD_L_B_LoadsBIntoL)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x21;
    cpu.reg(ProcessingUnit::Register::L) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xE0;

    const int cycles = op_ld_l_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x21);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x21);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xE0);
}

TEST_F(OpcodesCPUTest, LD_L_C_LoadsCIntoL)
{
    cpu.reg(ProcessingUnit::Register::C) = 0x9E;
    cpu.reg(ProcessingUnit::Register::L) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x40;

    const int cycles = op_ld_l_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x9E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x9E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x40);
}

TEST_F(OpcodesCPUTest, LD_L_D_LoadsDIntoL)
{
    cpu.reg(ProcessingUnit::Register::D) = 0x2A;
    cpu.reg(ProcessingUnit::Register::L) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xA0;

    const int cycles = op_ld_l_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x2A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x2A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xA0);
}

TEST_F(OpcodesCPUTest, LD_L_E_LoadsEIntoL)
{
    cpu.reg(ProcessingUnit::Register::E) = 0xD4;
    cpu.reg(ProcessingUnit::Register::L) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x90;

    const int cycles = op_ld_l_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0xD4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xD4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x90);
}

TEST_F(OpcodesCPUTest, LD_L_H_LoadsHIntoL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x61;
    cpu.reg(ProcessingUnit::Register::L) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x30;

    const int cycles = op_ld_l_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x61);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x61);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x30);
}

TEST_F(OpcodesCPUTest, LD_L_L_KeepsLUnchanged)
{
    cpu.reg(ProcessingUnit::Register::L) = 0x8C;
    cpu.reg(ProcessingUnit::Register::F) = 0xB0;

    const int cycles = op_ld_l_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x8C);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xB0);
}

TEST_F(OpcodesCPUTest, LD_L_HL_LoadsMemoryIntoL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x50;
    cpu.reg(ProcessingUnit::Register::F) = 0x70;
    mmu.write(0xC050, 0xDE);

    const int cycles = op_ld_l_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0xDE);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xC0);
    EXPECT_EQ(cpu.get_hl(), 0xC0DE);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x70);
}

TEST_F(OpcodesCPUTest, LD_L_A_LoadsAIntoL)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x77;
    cpu.reg(ProcessingUnit::Register::L) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_ld_l_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x77);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x77);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}
