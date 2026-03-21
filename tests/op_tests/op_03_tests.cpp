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

TEST_F(OpcodesCPUTest, INC_SP_IncrementsStackPointer)
{
    cpu.set_sp(0x1234);

    const int cycles = op_inc_sp(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_sp(), 0x1235);
}

TEST_F(OpcodesCPUTest, INC_HL_PTR_IncrementsMemoryAndFlags)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x10;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;
    mmu.write(0xC010, 0x0F);

    const int cycles = op_inc_hl_ptr(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(mmu.read(0xC010), 0x10);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, DEC_HL_PTR_DecrementsMemoryAndFlags)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x20;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;
    mmu.write(0xC020, 0x10);

    const int cycles = op_dec_hl_ptr(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(mmu.read(0xC020), 0x0F);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 1);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, LD_HL_D8_StoresImmediateToMemory)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x30;
    std::vector<u8> rom(0x200);
    rom[0x100] = 0x00;
    rom[0x101] = 0xAB;
    mmu.map_rom(rom);

    const int cycles = op_ld_hl_d8(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(mmu.read(0xC030), 0xAB);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, SCF_SetsCarryAndClearsNH)
{
    cpu.reg(ProcessingUnit::Register::F) = 0xE0;

    const int cycles = op_scf(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.get_flag_z(), 1);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, JR_C_JumpsWhenCarryFlagIsSet)
{
    std::vector<u8> rom(0x200);
    rom[0x101] = 0xFB;
    mmu.map_rom(rom);
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_jr_c(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_pc(), 0x0FD);
}

TEST_F(OpcodesCPUTest, JR_C_DoesNotJumpWhenCarryFlagIsClear)
{
    std::vector<u8> rom(0x200);
    rom[0x101] = 0x05;
    mmu.map_rom(rom);
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_jr_c(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, ADD_HL_SP_AddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x8A;
    cpu.reg(ProcessingUnit::Register::L) = 0x23;
    cpu.set_sp(0x8A23);
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_add_hl_sp(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_hl(), 0x1446);
    EXPECT_EQ(cpu.get_flag_z(), 1);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, LD_A_HLD_LoadsAndDecrementsHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x40;
    mmu.write(0xC040, 0xBE);

    const int cycles = op_ld_a_hld(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xBE);
    EXPECT_EQ(cpu.get_hl(), 0xC03F);
}

TEST_F(OpcodesCPUTest, DEC_SP_DecrementsStackPointer)
{
    cpu.set_sp(0x1234);

    const int cycles = op_dec_sp(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_sp(), 0x1233);
}

TEST_F(OpcodesCPUTest, INC_A_UpdatesFlagsAndPreservesCarry)
{
    cpu.reg(ProcessingUnit::Register::A) = 0xFF;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_inc_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_EQ(cpu.get_flag_z(), 1);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, DEC_A_UpdatesFlagsAndPreservesCarry)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x10;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_dec_a(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x0F);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 1);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, LD_A_D8_LoadsImmediateIntoA)
{
    std::vector<u8> rom(0x200);
    rom[0x100] = 0x00;
    rom[0x101] = 0x7E;
    mmu.map_rom(rom);

    const int cycles = op_ld_a_d8(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x7E);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}
