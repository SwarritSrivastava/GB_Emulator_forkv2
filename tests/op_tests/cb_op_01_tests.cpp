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

TEST_F(CB_OpcodesCPUTest, RLB_RotatesBRegisterLeft) {
    cpu.reg(ProcessingUnit::Register::B) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c0 = op_rl_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B) , 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_EQ(c0, 8);

    cpu.reg(ProcessingUnit::Register::B) = 0x01;
    cpu.setFlag(ProcessingUnit::Flag::C , true);

    op_rl_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x03);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
}