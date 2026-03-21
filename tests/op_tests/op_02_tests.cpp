#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, JR_NZ_JumpsWhenZeroFlagIsClear)
{
    std::vector<u8> rom(0x200);
    rom[0x101] = 0x05;
    mmu.map_rom(rom);
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_jr_nz(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_pc(), 0x107);
}

TEST_F(OpcodesCPUTest, JR_NZ_DoesNotJumpWhenZeroFlagIsSet)
{
    std::vector<u8> rom(0x200);
    rom[0x101] = 0x05;
    mmu.map_rom(rom);
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_jr_nz(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, LD_HL_D16_LoadsImmediate)
{
    std::vector<u8> rom(0x200);
    rom[0x100] = 0x34;
    rom[0x101] = 0x12;
    mmu.map_rom(rom);

    const int cycles = op_ld_hl_d16(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_hl(), 0x1234);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, LD_HLI_A_StoresAndIncrementsHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.reg(ProcessingUnit::Register::A) = 0x77;

    const int cycles = op_ld_hli_a(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(mmu.read(0xC000), 0x77);
    EXPECT_EQ(cpu.get_hl(), 0xC001);
}

TEST_F(OpcodesCPUTest, INC_HL_IncrementsRegisterPair)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x12;
    cpu.reg(ProcessingUnit::Register::L) = 0xFF;

    const int cycles = op_inc_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_hl(), 0x1300);
}

TEST_F(OpcodesCPUTest, INC_H_UpdatesFlagsAndPreservesCarry)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x0F;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_inc_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x10);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, DEC_H_UpdatesFlagsAndPreservesCarry)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x10;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_dec_h(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x0F);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 1);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, LD_H_D8_LoadsImmediateIntoH)
{
    std::vector<u8> rom(0x200);
    rom[0x100] = 0x00;
    rom[0x101] = 0x56;
    mmu.map_rom(rom);

    const int cycles = op_ld_h_d8(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x56);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, DAA_AdjustsAfterAddition)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x3C;
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_daa(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x42);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}

TEST_F(OpcodesCPUTest, DAA_AdjustsAfterSubtraction)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x0F;
    cpu.reg(ProcessingUnit::Register::F) = 0x60;

    const int cycles = op_daa(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x09);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 1);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}

TEST_F(OpcodesCPUTest, JR_Z_JumpsWhenZeroFlagIsSet)
{
    std::vector<u8> rom(0x200);
    rom[0x101] = 0xFB;
    mmu.map_rom(rom);
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_jr_z(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_pc(), 0x0FD);
}

TEST_F(OpcodesCPUTest, JR_Z_DoesNotJumpWhenZeroFlagIsClear)
{
    std::vector<u8> rom(0x200);
    rom[0x101] = 0xFB;
    mmu.map_rom(rom);
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_jr_z(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, ADD_HL_HL_AddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x8A;
    cpu.reg(ProcessingUnit::Register::L) = 0x23;
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_add_hl_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_hl(), 0x1446);
    EXPECT_EQ(cpu.get_flag_z(), 1);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, LD_A_HLI_LoadsAndIncrementsHL)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x10;
    mmu.write(0xC010, 0xAB);

    const int cycles = op_ld_a_hli(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xAB);
    EXPECT_EQ(cpu.get_hl(), 0xC011);
}

TEST_F(OpcodesCPUTest, DEC_HL_DecrementsRegisterPair)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x34;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;

    const int cycles = op_dec_hl(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_hl(), 0x33FF);
}

TEST_F(OpcodesCPUTest, INC_L_UpdatesFlagsAndPreservesCarry)
{
    cpu.reg(ProcessingUnit::Register::L) = 0xFF;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_inc_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x00);
    EXPECT_EQ(cpu.get_flag_z(), 1);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, DEC_L_UpdatesFlagsAndPreservesCarry)
{
    cpu.reg(ProcessingUnit::Register::L) = 0x10;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_dec_l(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x0F);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 1);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, LD_L_D8_LoadsImmediateIntoL)
{
    std::vector<u8> rom(0x200);
    rom[0x100] = 0x00;
    rom[0x101] = 0x9A;
    mmu.map_rom(rom);

    const int cycles = op_ld_l_d8(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x9A);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, CPL_ComplementsAAndSetsNH)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x55;
    cpu.reg(ProcessingUnit::Register::F) = 0x90;

    const int cycles = op_cpl(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xAA);
    EXPECT_EQ(cpu.get_flag_z(), 1);
    EXPECT_EQ(cpu.get_flag_n(), 1);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}
