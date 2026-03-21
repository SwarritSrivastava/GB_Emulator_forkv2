#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, JR_NC_JumpsWhenCarryFlagIsClear)
{
    std::vector<u8> rom(0x200);
    rom[0x101] = 0x05;
    mmu.map_rom(rom);
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_jr_nc(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_pc(), 0x107);
}

TEST_F(OpcodesCPUTest, JR_NC_DoesNotJumpWhenCarryFlagIsSet)
{
    std::vector<u8> rom(0x200);
    rom[0x101] = 0x05;
    mmu.map_rom(rom);
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_jr_nc(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}
