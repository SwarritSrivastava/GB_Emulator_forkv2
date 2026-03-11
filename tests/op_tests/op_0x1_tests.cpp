#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, STOP_SetsStopState)
{
    EXPECT_EQ(cpu.get_pc(), 0x0100);
    EXPECT_TRUE(!cpu.isStop());
    std::vector<u8> rom(0x200);
    rom[0x100] = 0x00;
    mmu.map_rom(rom);

    const int cycles = op_stop(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_TRUE(cpu.isStop());
    EXPECT_EQ(cpu.get_pc(), 0x0100);
}

TEST_F(OpcodesCPUTest, LD_DE_D16_LoadsImmediateIntoBC)
{
    EXPECT_EQ(cpu.get_pc(), 0x0100);

    // Write immediate value 0x1234 (little endian)
    std::vector<u8> rom(0x200);
    rom[0x100] = 0x34;
    rom[0x101] = 0x12;
    mmu.map_rom(rom);

    const int cycles = op_ld_de_d16(cpu, mmu);

    // PC should advance by 2
    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_de(), 0x1234);
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::D)), 0x12);
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::E)), 0x34);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}