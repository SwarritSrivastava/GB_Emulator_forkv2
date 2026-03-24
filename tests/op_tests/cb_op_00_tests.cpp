#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/cb_opcodes.hpp"
#include "../../include/opcodes.hpp"

class CB_OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(CB_OpcodesCPUTest, RLCB_RotatesBRegisterLeft) 
{
    cpu.reg(ProcessingUnit::Register::B) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c0 = op_rlc_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B) , 0x01);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());

    //Test for Z flag

    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c1 = op_rlc_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x00);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}