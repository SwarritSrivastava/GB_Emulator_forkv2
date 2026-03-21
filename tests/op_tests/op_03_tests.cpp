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

TEST_F(OpcodesCPUTest, LD_SP_D16_LoadsImmediateIntoSP)
{
    std::vector<u8> rom(0x200);
    rom[0x100] = 0x34;
    rom[0x101] = 0x12;
    mmu.map_rom(rom);

    const int cycles = op_ld_sp_d16(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_sp(), 0x1234);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, LD_HLD_A_StoresAndDecrementsHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.reg(ProcessingUnit::Register::A) = 0x44;

    const int cycles = op_ld_hld_a(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(mmu.read(0xC000), 0x44);
    EXPECT_EQ(cpu.get_hl(), 0xBFFF);
}
