#include <gtest/gtest.h>
#include "../include/mmu.hpp"
#include "../include/ProcessingUnit.hpp"
#include "../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, LD_BC_D16_LoadsImmediateIntoBC)
{
    const u16 pc = cpu.get_pc();
    EXPECT_EQ(pc, 0x0100);

    // Write immediate value 0x1234 (little endian)
    std::vector<u8> rom(0x200); // small fake ROM
    rom[0x100] = 0x34;
    rom[0x101] = 0x12;

    mmu.map_rom(rom);
    const int cycles = op_ld_bc_d16(cpu, mmu);

    // Check returned cycle count
    EXPECT_EQ(cycles, 12);

    // Check BC register value
    EXPECT_EQ(cpu.get_bc(), 0x1234);

    // Check individual registers
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::B)), 0x12);
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::C)), 0x34);

    // PC should advance by 2
    EXPECT_EQ(cpu.get_pc(), 0x102);
}