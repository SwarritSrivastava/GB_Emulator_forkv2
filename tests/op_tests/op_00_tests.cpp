#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, NO_OPERATION)
{
    EXPECT_EQ(cpu.get_pc(), 0x0100);

    const int cycles = op_nop(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.get_bc(), 0x0013);
    EXPECT_EQ(cpu.get_de(), 0x00D8);
    EXPECT_EQ(cpu.get_hl(), 0x014D);
}

TEST_F(OpcodesCPUTest, LD_BC_D16_LoadsImmediateIntoBC)
{
    EXPECT_EQ(cpu.get_pc(), 0x0100);

    // Write immediate value 0x1234 (little endian)
    std::vector<u8> rom(0x200);
    rom[0x100] = 0x34;
    rom[0x101] = 0x12;
    mmu.map_rom(rom);

    const int cycles = op_ld_bc_d16(cpu, mmu);

    // PC should advance by 2
    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_bc(), 0x1234);
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::B)), 0x12);
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::C)), 0x34);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, LD_BC_A_StoresAIntoMemory)
{
    // Set bc = C000 (WRAM region so write is allowed)
    cpu.reg(ProcessingUnit::Register::B) = 0xC0;
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.reg(ProcessingUnit::Register::A) = 0x42; // Set a register

    const int cycles = op_ld_bc_a(cpu, mmu);
    EXPECT_EQ(cycles, 8); // Verify cycle count

    EXPECT_EQ(mmu.read(0xC000), 0x42);

    EXPECT_EQ(cpu.get_bc(), 0xC000); // Verify bc integrity
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x42); // Verify a integrity
}

TEST_F(OpcodesCPUTest, INC_BC_IncrementsRegisterPair)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x12;
    cpu.reg(ProcessingUnit::Register::C) = 0x34;

    const int cycles = op_inc_bc(cpu, mmu);

    EXPECT_EQ(cycles, 8);

    EXPECT_EQ(cpu.get_bc(), 0x1235); // BC should now be 0x1235
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x12);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x35);
}

TEST_F(OpcodesCPUTest, INC_B_IncrementsRegister)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x12;
    cpu.reg(ProcessingUnit::Register::C) = 0x34;

    const int cycles = op_inc_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);

    EXPECT_EQ(cpu.get_bc(), 0x1334); // BC should now be 0x1334
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x13);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x34);
}

TEST_F(OpcodesCPUTest, DEC_B_DecreasesRegister)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x12;
    cpu.reg(ProcessingUnit::Register::C) = 0x34;

    const int cycles = op_dec_b(cpu, mmu);

    EXPECT_EQ(cycles, 4);

    EXPECT_EQ(cpu.get_bc(), 0x1134); // BC should now be 0x1134
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x11);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x34);
}

TEST_F(OpcodesCPUTest, LD_B_D8_LoadsImmediateIntoB)
{
    const u16 pc = cpu.get_pc();
    EXPECT_EQ(pc, 0x0100);

    // Write immediate value 0x1234 (little endian)
    std::vector<u8> rom(0x200); // small fake ROM
    rom[0x100] = 0x00;
    rom[0x101] = 0x12;

    mmu.map_rom(rom);
    const int cycles = op_ld_b_d8(cpu, mmu);

    // Check returned cycle count
    EXPECT_EQ(cycles, 8);

    // Check B register value
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::B)), 0x12);

    // PC should advance by 2
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, RLCA_RotatesARegisterRight)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x79;

    const int cycles = op_rlca(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::A)), 0xF2);
    EXPECT_EQ(cpu.get_flag_c(), 0x00);
    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_pc(), 0x100);
}

TEST_F(OpcodesCPUTest, LD_A16_SP_StoresStackPointerInMemory)
{
    const u16 pc = cpu.get_pc();
    EXPECT_EQ(pc, 0x0100);

    std::vector<u8> rom(0x200);
    rom[0x100] = 0x00; // low byte
    rom[0x101] = 0xC1; // high byte
    mmu.map_rom(rom);

    const int cycles = op_ld_a16_sp(cpu, mmu);

    EXPECT_EQ(cycles, 20);
    EXPECT_EQ(mmu.read(0xC100), 0xFE); // SP low
    EXPECT_EQ(mmu.read(0xC101), 0xFF); // SP high

    EXPECT_EQ(cpu.get_sp(), 0xFFFE);

    EXPECT_EQ(cpu.get_pc(), 0x0102);
}

TEST_F(OpcodesCPUTest, ADD_HL_BC_AddsRegisterPair)
{
    const u16 pc = cpu.get_pc();
    EXPECT_EQ(pc, 0x0100);
    // BC = 0x1234
    cpu.reg(ProcessingUnit::Register::B) = 0x12;
    cpu.reg(ProcessingUnit::Register::C) = 0x34;
    // HL = 0x0101
    cpu.reg(ProcessingUnit::Register::H) = 0x01;
    cpu.reg(ProcessingUnit::Register::L) = 0x01;

    const int cycles = op_add_hl_bc(cpu, mmu);

    EXPECT_EQ(cycles, 8);

    EXPECT_EQ(cpu.get_bc(), 0x1234); // BC should remain same
    EXPECT_EQ(cpu.get_hl(), 0x1335);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x12);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x34);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x13);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x35);

    EXPECT_EQ(cpu.get_pc(), 0x0100);
}

TEST_F(OpcodesCPUTest, LD_A_BC_LoadsMemoryIntoA)
{
    cpu.reg(ProcessingUnit::Register::B) = 0xA0;
    cpu.reg(ProcessingUnit::Register::C) = 0x00;

    // Write value to memory
    mmu.write(0xA000, 0x77);

    const int cycles = op_ld_a_bc(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x77);

    EXPECT_EQ(cpu.get_bc(), 0xA000);
    EXPECT_EQ(cpu.get_pc(), 0x100);
}

TEST_F(OpcodesCPUTest, DEC_BC_DecreasesRegisterPair)
{
    // BC = 0x0000
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.reg(ProcessingUnit::Register::C) = 0x00;

    const int cycles = op_dec_bc(cpu, mmu);
    // BC = 0xFFFF
    EXPECT_EQ(cycles, 8);

    EXPECT_EQ(cpu.get_bc(), 0xFFFF); // BC should now be 0x1235
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xFF);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xFF);
}

TEST_F(OpcodesCPUTest, INC_C_IncrementsRegisterPair)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x12;
    cpu.reg(ProcessingUnit::Register::C) = 0x34;

    const int cycles = op_inc_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);

    EXPECT_EQ(cpu.get_bc(), 0x1235); // BC should now be 0x1235
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x12);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x35);
}

TEST_F(OpcodesCPUTest, DEC_C_DecreasesRegisterPair)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x12;
    cpu.reg(ProcessingUnit::Register::C) = 0x34;

    const int cycles = op_dec_c(cpu, mmu);

    EXPECT_EQ(cycles, 4);

    EXPECT_EQ(cpu.get_bc(), 0x1233); // BC should now be 0x1233
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x12);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x33);
}

TEST_F(OpcodesCPUTest, LD_C_D8_LoadsImmediateIntoC)
{
    EXPECT_EQ(cpu.get_pc(), 0x0100);

    std::vector<u8> rom(0x200);
    rom[0x100] = 0x77;
    mmu.map_rom(rom);

    const int cycles = op_ld_c_d8(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x77);
    EXPECT_EQ(cpu.get_pc(), 0x0101);
}

TEST_F(OpcodesCPUTest, RRCA_RotatesARegisterRight)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x79;
    const int cycles = op_rrca(cpu, mmu);

    EXPECT_EQ(cycles, 4);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xBC);

    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 1);

    EXPECT_EQ(cpu.get_pc(), 0x100);
}