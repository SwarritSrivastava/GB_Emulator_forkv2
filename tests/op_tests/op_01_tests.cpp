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
