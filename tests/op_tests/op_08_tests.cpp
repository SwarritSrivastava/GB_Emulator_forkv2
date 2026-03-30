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
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_add_a_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    // sum = 0x8A + 0x23 = 0xAD
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xAD);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0); // Half carry: (0x8A + 0x23) = 0xAD → no carry from bit 11
    EXPECT_EQ(cpu.get_flag_c(), 0); // Carry: no overflow beyond 0xFFFF
}