#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, LD_D_B_LoadsBIntoD)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x7A;
    cpu.reg(ProcessingUnit::Register::D) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_ld_d_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x7A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x7A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}

TEST_F(OpcodesCPUTest, LD_D_C_LoadsCIntoD)
{
    cpu.reg(ProcessingUnit::Register::C) = 0xA5;
    cpu.reg(ProcessingUnit::Register::D) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xB0;

    const int cycles = op_ld_d_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0xA5);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xA5);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xB0);
}

TEST_F(OpcodesCPUTest, LD_D_D_KeepsDUnchanged)
{
    cpu.reg(ProcessingUnit::Register::D) = 0x3C;
    cpu.reg(ProcessingUnit::Register::F) = 0x90;

    const int cycles = op_ld_d_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x3C);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x90);
}

TEST_F(OpcodesCPUTest, LD_D_E_LoadsEIntoD)
{
    cpu.reg(ProcessingUnit::Register::E) = 0xC3;
    cpu.reg(ProcessingUnit::Register::D) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x70;

    const int cycles = op_ld_d_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0xC3);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xC3);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x70);
}

TEST_F(OpcodesCPUTest, LD_D_H_LoadsHIntoD)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x6E;
    cpu.reg(ProcessingUnit::Register::D) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xA0;

    const int cycles = op_ld_d_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x6E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x6E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xA0);
}

TEST_F(OpcodesCPUTest, LD_D_L_LoadsLIntoD)
{
    cpu.reg(ProcessingUnit::Register::L) = 0x55;
    cpu.reg(ProcessingUnit::Register::D) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xD0;

    const int cycles = op_ld_d_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x55);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x55);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xD0);
}

TEST_F(OpcodesCPUTest, LD_D_HL_LoadsMemoryIntoD)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x88;
    cpu.reg(ProcessingUnit::Register::D) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    mmu.write(0xC088, 0x9B);

    const int cycles = op_ld_d_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x9B);
    EXPECT_EQ(cpu.get_hl(), 0xC088);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}

TEST_F(OpcodesCPUTest, LD_D_A_LoadsAIntoD)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x4D;
    cpu.reg(ProcessingUnit::Register::D) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x50;

    const int cycles = op_ld_d_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x4D);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x4D);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x50);
}

TEST_F(OpcodesCPUTest, LD_E_B_LoadsBIntoE)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x21;
    cpu.reg(ProcessingUnit::Register::E) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xE0;

    const int cycles = op_ld_e_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x21);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x21);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xE0);
}

TEST_F(OpcodesCPUTest, LD_E_C_LoadsCIntoE)
{
    cpu.reg(ProcessingUnit::Register::C) = 0x9E;
    cpu.reg(ProcessingUnit::Register::E) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x40;

    const int cycles = op_ld_e_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x9E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x9E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x40);
}

TEST_F(OpcodesCPUTest, LD_E_D_LoadsDIntoE)
{
    cpu.reg(ProcessingUnit::Register::D) = 0x2A;
    cpu.reg(ProcessingUnit::Register::E) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xA0;

    const int cycles = op_ld_e_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x2A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x2A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xA0);
}

TEST_F(OpcodesCPUTest, LD_E_E_KeepsEUnchanged)
{
    cpu.reg(ProcessingUnit::Register::E) = 0xD4;
    cpu.reg(ProcessingUnit::Register::F) = 0x90;

    const int cycles = op_ld_e_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xD4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x90);
}

TEST_F(OpcodesCPUTest, LD_E_H_LoadsHIntoE)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x61;
    cpu.reg(ProcessingUnit::Register::E) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x30;

    const int cycles = op_ld_e_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x61);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x61);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x30);
}

TEST_F(OpcodesCPUTest, LD_E_L_LoadsLIntoE)
{
    cpu.reg(ProcessingUnit::Register::L) = 0x8C;
    cpu.reg(ProcessingUnit::Register::E) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xB0;

    const int cycles = op_ld_e_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x8C);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x8C);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xB0);
}

TEST_F(OpcodesCPUTest, LD_E_HL_LoadsMemoryIntoE)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x50;
    cpu.reg(ProcessingUnit::Register::E) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x70;
    mmu.write(0xC050, 0xDE);

    const int cycles = op_ld_e_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xDE);
    EXPECT_EQ(cpu.get_hl(), 0xC050);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x70);
}

TEST_F(OpcodesCPUTest, LD_E_A_LoadsAIntoE)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x77;
    cpu.reg(ProcessingUnit::Register::E) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_ld_e_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x77);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x77);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}
