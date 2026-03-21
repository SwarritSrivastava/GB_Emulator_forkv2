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
