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

TEST_F(OpcodesCPUTest, CB_Prefix) 
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
