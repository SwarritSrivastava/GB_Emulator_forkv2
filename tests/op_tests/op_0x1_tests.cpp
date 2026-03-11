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