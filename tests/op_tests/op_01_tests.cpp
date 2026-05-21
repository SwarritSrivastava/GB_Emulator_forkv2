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

TEST_F(OpcodesCPUTest, LD_DE_A_StoresAIntoMemory)
{
    // Set de = C000 (WRAM region so write is allowed)
    cpu.reg(ProcessingUnit::Register::D) = 0xC0;
    cpu.reg(ProcessingUnit::Register::E) = 0x00;
    cpu.reg(ProcessingUnit::Register::A) = 0x42; // Set a register

    const int cycles = op_ld_de_a(cpu, mmu);
    EXPECT_EQ(cycles, 8); // Verify cycle count

    EXPECT_EQ(mmu.read(0xC000), 0x42);

    EXPECT_EQ(cpu.get_de(), 0xC000); // Verify de integrity
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x42); // Verify a integrity
}

TEST_F(OpcodesCPUTest, INC_DE_IncrementsRegisterPair)
{
    cpu.reg(ProcessingUnit::Register::D) = 0x12;
    cpu.reg(ProcessingUnit::Register::E) = 0x34;

    const int cycles = op_inc_de(cpu, mmu);

    EXPECT_EQ(cycles, 8);

    EXPECT_EQ(cpu.get_de(), 0x1235); // DE should now be 0x1235
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x12);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x35);
}

TEST_F(OpcodesCPUTest, INC_D_IncrementsRegister)
{
    cpu.reg(ProcessingUnit::Register::D) = 0x12;
    cpu.reg(ProcessingUnit::Register::E) = 0x34;

    const int cycles = op_inc_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);

    EXPECT_EQ(cpu.get_de(), 0x1334); // DE should now be 0x1334
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x13);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x34);
}

TEST_F(OpcodesCPUTest, INC_D_UpdatesFlagsAndPreservesCarry)
{
    cpu.reg(ProcessingUnit::Register::D) = 0xFF;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_inc_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x00);
    EXPECT_EQ(cpu.get_flag_z(), 1);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, DEC_D_DecreasesRegisterPair)
{
    cpu.reg(ProcessingUnit::Register::D) = 0x12;
    cpu.reg(ProcessingUnit::Register::E) = 0x34;

    const int cycles = op_dec_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);

    EXPECT_EQ(cpu.get_de(), 0x1134); // DE should now be 0x1235
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x11);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x34);
}

TEST_F(OpcodesCPUTest, DEC_D_UpdatesFlagsAndPreservesCarry)
{
    cpu.reg(ProcessingUnit::Register::D) = 0x01;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_dec_d(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x00);
    EXPECT_EQ(cpu.get_flag_z(), 1);
    EXPECT_EQ(cpu.get_flag_n(), 1);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, INC_DEC_D_ConsecutiveOpcodesUpdateFlags)
{
    cpu.reg(ProcessingUnit::Register::D) = 0x0F;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int incCycles = op_inc_d(cpu, mmu);
    EXPECT_EQ(incCycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x10);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);

    const int decCycles = op_dec_d(cpu, mmu);
    EXPECT_EQ(decCycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x0F);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 1);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, LD_D_D8_LoadsImmediateIntoD)
{
    const u16 pc = cpu.get_pc();
    EXPECT_EQ(pc, 0x0100);

    // Write immediate value 0x1234 (little endian)
    std::vector<u8> rom(0x200); // small fake ROM
    rom[0x100] = 0x00;
    rom[0x101] = 0x12;

    mmu.map_rom(rom);
    const int cycles = op_ld_d_d8(cpu, mmu);

    // Check returned cycle count
    EXPECT_EQ(cycles, 8);

    // Check B register value
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::D)), 0x12);

    // PC should advance by 2
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, RLA_RotatesAThroughCarry)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x85;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_rla(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x0B);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 1);
    EXPECT_EQ(cpu.get_pc(), 0x100);
}

TEST_F(OpcodesCPUTest, RLA_UsesCarryIn)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x42;
    cpu.clearFlags();

    const int cycles = op_rla(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x84);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}

TEST_F(OpcodesCPUTest, JR_R8_JumpsForwardFromNextInstruction)
{
    std::vector<u8> rom(0x200);
    rom[0x101] = 0x05;
    mmu.map_rom(rom);

    cpu.normalizeFlags();

    const int cycles = op_jr_r8(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_pc(), 0x107);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}

TEST_F(OpcodesCPUTest, JR_R8_JumpsBackwardFromNextInstruction)
{
    std::vector<u8> rom(0x200);
    rom[0x101] = 0xFB;
    mmu.map_rom(rom);

    const int cycles = op_jr_r8(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_pc(), 0x0FD);
}

TEST_F(OpcodesCPUTest, ADD_HL_DE_AddsRegisterPairAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::H) = 0x12;
    cpu.reg(ProcessingUnit::Register::L) = 0x34;
    cpu.reg(ProcessingUnit::Register::D) = 0x11;
    cpu.reg(ProcessingUnit::Register::E) = 0x11;
    cpu.normalizeFlags();

    const int cycles = op_add_hl_de(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_hl(), 0x2345);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
    EXPECT_EQ(cpu.get_flag_z(), 1);
}

TEST_F(OpcodesCPUTest, ADD_HL_DE_SetsHalfCarryAndCarry)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xFF;
    cpu.reg(ProcessingUnit::Register::L) = 0xFF;
    cpu.reg(ProcessingUnit::Register::D) = 0x00;
    cpu.reg(ProcessingUnit::Register::E) = 0x01;
    cpu.clearFlags();

    const int cycles = op_add_hl_de(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_hl(), 0x0000);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, LD_A_DE_LoadsMemoryIntoA)
{
    cpu.reg(ProcessingUnit::Register::D) = 0xA0;
    cpu.reg(ProcessingUnit::Register::E) = 0x02;
    mmu.write(0xA002, 0x66);

    const int cycles = op_ld_a_de(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x66);
    EXPECT_EQ(cpu.get_de(), 0xA002);
}

TEST_F(OpcodesCPUTest, DEC_DE_DecrementsRegisterPair)
{
    cpu.reg(ProcessingUnit::Register::D) = 0x12;
    cpu.reg(ProcessingUnit::Register::E) = 0x00;

    const int cycles = op_dec_de(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_de(), 0x11FF);
}

TEST_F(OpcodesCPUTest, INC_E_UpdatesFlagsAndPreservesCarry)
{
    cpu.reg(ProcessingUnit::Register::E) = 0xFF;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_inc_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x00);
    EXPECT_EQ(cpu.get_flag_z(), 1);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, DEC_E_UpdatesFlagsAndPreservesCarry)
{
    cpu.reg(ProcessingUnit::Register::E) = 0x10;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_dec_e(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x0F);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 1);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, LD_E_D8_LoadsImmediateIntoE)
{
    std::vector<u8> rom(0x200);
    rom[0x100] = 0x00;
    rom[0x101] = 0x34;
    mmu.map_rom(rom);

    const int cycles = op_ld_e_d8(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x34);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, RRA_RotatesAThroughCarry)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x01;
    cpu.reg(ProcessingUnit::Register::F) = 0x10;

    const int cycles = op_rra(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x80);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}
