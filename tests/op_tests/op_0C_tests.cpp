#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"
#include "../../include/opcode_table.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, CB_Prefix_ChecksForCorrectCBOpcodeCall) 
{
    const u8 test_opcode = 0x00;
    const u16 start_pc = 0x100;

    cpu.set_pc(start_pc);
    mmu.write(start_pc, test_opcode);

    auto original = cbInstructionTable[test_opcode];
    cbInstructionTable[test_opcode] = [](ProcessingUnit&, MMU&) -> int 
    {
        return 2;
    };

    int cycles = op_cb_prefix(cpu, mmu);

    EXPECT_EQ(cycles, 2);
    EXPECT_EQ(cpu.get_pc(), start_pc + 1);

    cbInstructionTable[test_opcode] = original;
}

TEST_F(OpcodesCPUTest, RET_NZ_DoesNotReturnWhenZeroFlagSet)
{
    cpu.set_pc(0x1234);
    cpu.set_sp(0xC100);
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    mmu.write(0xC100, 0x78);
    mmu.write(0xC101, 0x56);

    const int cycles = op_ret_nz(cpu, mmu);

    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.get_pc(), 0x1234);
    EXPECT_EQ(cpu.get_sp(), 0xC100);
}

TEST_F(OpcodesCPUTest, RET_NZ_ReturnsWhenZeroFlagClear)
{
    cpu.set_pc(0x1234);
    cpu.set_sp(0xC100);
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    mmu.write(0xC100, 0x78);
    mmu.write(0xC101, 0x56);

    const int cycles = op_ret_nz(cpu, mmu);

    EXPECT_EQ(cycles, 20);
    EXPECT_EQ(cpu.get_pc(), 0x5678);
    EXPECT_EQ(cpu.get_sp(), 0xC102);
}

TEST_F(OpcodesCPUTest, JP_NZ_JumpsWhenZeroFlagClear)
{
    std::vector<u8> rom(0x200, 0x00);
    rom[0x100] = 0x34;
    rom[0x101] = 0x12;
    ASSERT_TRUE(mmu.map_rom(rom));

    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_jp_nz(cpu, mmu);

    EXPECT_EQ(cycles, 16);
    EXPECT_EQ(cpu.get_pc(), 0x1234);
}

TEST_F(OpcodesCPUTest, JP_NZ_DoesNotJumpWhenZeroFlagSet)
{
    std::vector<u8> rom(0x200, 0x00);
    rom[0x100] = 0x34;
    rom[0x101] = 0x12;
    ASSERT_TRUE(mmu.map_rom(rom));

    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_jp_nz(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, JP_NZ_ThroughStep_UsesOpcodeTableAndJumps)
{
    std::vector<u8> rom(0x200, 0x00);
    rom[0x100] = 0xC2;
    rom[0x101] = 0x78;
    rom[0x102] = 0x56;
    ASSERT_TRUE(mmu.map_rom(rom));

    cpu.set_pc(0x100);
    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = cpu.step(mmu);

    EXPECT_EQ(cycles, 16);
    EXPECT_EQ(cpu.get_pc(), 0x5678);
}

TEST_F(OpcodesCPUTest, JP_Z_JumpsWhenZeroFlagSet)
{
    std::vector<u8> rom(0x200, 0x00);
    rom[0x100] = 0x34;
    rom[0x101] = 0x12;
    ASSERT_TRUE(mmu.map_rom(rom));

    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_jp_z(cpu, mmu);

    EXPECT_EQ(cycles, 16);
    EXPECT_EQ(cpu.get_pc(), 0x1234);
}

TEST_F(OpcodesCPUTest, JP_Z_DoesNotJumpWhenZeroFlagClear)
{
    std::vector<u8> rom(0x200, 0x00);
    rom[0x100] = 0x34;
    rom[0x101] = 0x12;
    ASSERT_TRUE(mmu.map_rom(rom));

    cpu.reg(ProcessingUnit::Register::F) = 0x00;

    const int cycles = op_jp_z(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_pc(), 0x102);
}

TEST_F(OpcodesCPUTest, POP_BC_LoadsLowByteIntoCAndHighByteIntoB)
{
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.set_sp(0xC100);

    mmu.write(0xC100, 0x34);
    mmu.write(0xC101, 0x12);

    const int cycles = op_pop_bc(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x12);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x34);
    EXPECT_EQ(cpu.get_bc(), 0x1234);
    EXPECT_EQ(cpu.get_sp(), 0xC102);
}

TEST_F(OpcodesCPUTest, POP_BC_DoesNotModifyFlags)
{
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    cpu.set_sp(0xC120);

    mmu.write(0xC120, 0x00);
    mmu.write(0xC121, 0x00);

    (void)op_pop_bc(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
}

TEST_F(OpcodesCPUTest, POP_BC_HandlesStackPointerWrapAround)
{
    cpu.set_sp(0xFFFF);
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.reg(ProcessingUnit::Register::C) = 0x00;

    mmu.write(0xFFFF, 0xCD);
    mmu.write(0x8000, 0xAB);
    mmu.map_rom({0xAB});

    const int cycles = op_pop_bc(cpu, mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xCD);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xAB);
    EXPECT_EQ(cpu.get_bc(), 0xABCD);
    EXPECT_EQ(cpu.get_sp(), 0x0001);
}

TEST_F(OpcodesCPUTest, POP_BC_ThroughStep_UsesOpcodeTableAndAdvancesPC)
{
    std::vector<u8> rom(0x102, 0x00);
    rom[0x100] = 0xC1;
    ASSERT_TRUE(mmu.map_rom(rom));

    cpu.set_pc(0x100);
    cpu.set_sp(0xC200);
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.reg(ProcessingUnit::Register::C) = 0x00;

    mmu.write(0xC200, 0xEF);
    mmu.write(0xC201, 0xBE);

    const int cycles = cpu.step(mmu);

    EXPECT_EQ(cycles, 12);
    EXPECT_EQ(cpu.get_pc(), 0x101);
    EXPECT_EQ(cpu.get_sp(), 0xC202);
    EXPECT_EQ(cpu.get_bc(), 0xBEEF);
}
TEST_F(OpcodesCPUTest, JP_Z_ThroughStep_UsesOpcodeTableAndJumps)
{
    std::vector<u8> rom(0x200, 0x00);
    rom[0x100] = 0xCA;
    rom[0x101] = 0x78;
    rom[0x102] = 0x56;
    ASSERT_TRUE(mmu.map_rom(rom));

    cpu.set_pc(0x100);
    cpu.reg(ProcessingUnit::Register::F) = 0x80;
    const int cycles = cpu.step(mmu);

    EXPECT_EQ(cycles, 16);
    EXPECT_EQ(cpu.get_pc(), 0x5678);
}
