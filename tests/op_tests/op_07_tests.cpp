#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};
TEST_F(OpcodesCPUTest, LD_HL_B_WritesBToMemoryAtHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x23;
    cpu.reg(ProcessingUnit::Register::B) = 0xA7;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    mmu.write(0xC123, 0x00);

    const int cycles = op_ld_hl_b(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(mmu.read(0xC123), 0xA7);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xA7);
    EXPECT_EQ(cpu.get_hl(), 0xC123);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}

TEST_F(OpcodesCPUTest, LD_HL_C_WritesCToMemoryAtHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x24;
    cpu.reg(ProcessingUnit::Register::C) = 0x6D;
    cpu.reg(ProcessingUnit::Register::F) = 0xB0;
    mmu.write(0xC124, 0x00);

    const int cycles = op_ld_hl_c(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(mmu.read(0xC124), 0x6D);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x6D);
    EXPECT_EQ(cpu.get_hl(), 0xC124);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xB0);
}

TEST_F(OpcodesCPUTest, LD_HL_D_WritesDToMemoryAtHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x25;
    cpu.reg(ProcessingUnit::Register::D) = 0x3B;
    cpu.reg(ProcessingUnit::Register::F) = 0x90;
    mmu.write(0xC125, 0x00);

    const int cycles = op_ld_hl_d(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(mmu.read(0xC125), 0x3B);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x3B);
    EXPECT_EQ(cpu.get_hl(), 0xC125);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x90);
}

TEST_F(OpcodesCPUTest, LD_HL_E_WritesEToMemoryAtHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x26;
    cpu.reg(ProcessingUnit::Register::E) = 0xD2;
    cpu.reg(ProcessingUnit::Register::F) = 0x70;
    mmu.write(0xC126, 0x00);

    const int cycles = op_ld_hl_e(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(mmu.read(0xC126), 0xD2);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xD2);
    EXPECT_EQ(cpu.get_hl(), 0xC126);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x70);
}

TEST_F(OpcodesCPUTest, LD_HL_H_WritesHToMemoryAtHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x27;
    cpu.reg(ProcessingUnit::Register::F) = 0xA0;
    mmu.write(0xC127, 0x00);

    const int cycles = op_ld_hl_h(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(mmu.read(0xC127), 0xC1);
    EXPECT_EQ(cpu.get_hl(), 0xC127);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xA0);
}

TEST_F(OpcodesCPUTest, LD_HL_L_WritesLToMemoryAtHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x28;
    cpu.reg(ProcessingUnit::Register::F) = 0x50;
    mmu.write(0xC128, 0x00);

    const int cycles = op_ld_hl_l(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(mmu.read(0xC128), 0x28);
    EXPECT_EQ(cpu.get_hl(), 0xC128);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x50);
}

TEST_F(OpcodesCPUTest, HALT_SetsHaltState)
{
    cpu.setHalt(false);
    cpu.reg(ProcessingUnit::Register::F) = 0xE0;

    const int cycles = op_halt(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_TRUE(cpu.isHalt());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xE0);
}

TEST_F(OpcodesCPUTest, LD_HL_A_WritesAToMemoryAtHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x29;
    cpu.reg(ProcessingUnit::Register::A) = 0xF3;
    cpu.reg(ProcessingUnit::Register::F) = 0x30;
    mmu.write(0xC129, 0x00);

    const int cycles = op_ld_hl_a(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(mmu.read(0xC129), 0xF3);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xF3);
    EXPECT_EQ(cpu.get_hl(), 0xC129);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x30);
}

TEST_F(OpcodesCPUTest, LD_A_B_LoadsBIntoA)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x11;
    cpu.reg(ProcessingUnit::Register::B) = 0x4A;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    const int cycles = op_ld_a_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x4A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x4A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}

TEST_F(OpcodesCPUTest, LD_A_C_LoadsCIntoA)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x11;
    cpu.reg(ProcessingUnit::Register::C) = 0x82;
    cpu.reg(ProcessingUnit::Register::F) = 0xB0;

    const int cycles = op_ld_a_c_79(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x82);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x82);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xB0);
}

TEST_F(OpcodesCPUTest, LD_A_D_LoadsDIntoA)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x11;
    cpu.reg(ProcessingUnit::Register::D) = 0x2C;
    cpu.reg(ProcessingUnit::Register::F) = 0x90;

    const int cycles = op_ld_a_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x2C);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x2C);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x90);
}

TEST_F(OpcodesCPUTest, LD_A_E_LoadsEIntoA)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x11;
    cpu.reg(ProcessingUnit::Register::E) = 0xD8;
    cpu.reg(ProcessingUnit::Register::F) = 0x70;

    const int cycles = op_ld_a_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xD8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xD8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x70);
}

TEST_F(OpcodesCPUTest, LD_A_H_LoadsHIntoA)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x11;
    cpu.reg(ProcessingUnit::Register::H) = 0x6F;
    cpu.reg(ProcessingUnit::Register::F) = 0xA0;

    const int cycles = op_ld_a_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x6F);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x6F);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xA0);
}

TEST_F(OpcodesCPUTest, LD_A_L_LoadsLIntoA)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x11;
    cpu.reg(ProcessingUnit::Register::L) = 0x5E;
    cpu.reg(ProcessingUnit::Register::F) = 0x40;

    const int cycles = op_ld_a_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x5E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x5E);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x40);
}

TEST_F(OpcodesCPUTest, LD_A_HL_LoadsMemoryIntoA)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x2A;
    cpu.reg(ProcessingUnit::Register::A) = 0x11;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;
    mmu.write(0xC12A, 0xBE);

    const int cycles = op_ld_a_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xBE);
    EXPECT_EQ(cpu.get_hl(), 0xC12A);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0x10);
}

TEST_F(OpcodesCPUTest, LD_A_A_KeepsAUnchanged)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x93;
    cpu.reg(ProcessingUnit::Register::F) = 0xC0;

    const int cycles = op_ld_a_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x93);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xC0);
}
