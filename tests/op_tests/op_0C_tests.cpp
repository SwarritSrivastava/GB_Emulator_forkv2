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
