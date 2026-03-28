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

TEST_F(CB_OpcodesCPUTest, RLCC_RotatesCRegisterLeft) 
{
    cpu.reg(ProcessingUnit::Register::C) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c0 = op_rlc_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C) , 0x01);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());

    //Test for Z flag

    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c1 = op_rlc_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x00);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, RLCD_RotatesDRegisterLeft) 
{
    cpu.reg(ProcessingUnit::Register::D) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c0 = op_rlc_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D) , 0x01);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());

    //Test for Z flag

    cpu.reg(ProcessingUnit::Register::D) = 0x00;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c1 = op_rlc_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x00);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, RLCE_RotatesERegisterLeft) 
{
    cpu.reg(ProcessingUnit::Register::E) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c0 = op_rlc_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E) , 0x01);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());

    //Test for Z flag

    cpu.reg(ProcessingUnit::Register::E) = 0x00;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c1 = op_rlc_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x00);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, RLCH_RotatesHRegisterLeft) 
{
    cpu.reg(ProcessingUnit::Register::H) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c0 = op_rlc_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H) , 0x01);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());

    //Test for Z flag

    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c1 = op_rlc_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x00);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, RLCL_RotatesLRegisterLeft) 
{
    cpu.reg(ProcessingUnit::Register::L) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c0 = op_rlc_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L) , 0x01);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());

    //Test for Z flag

    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int c1 = op_rlc_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x00);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, RLCHL_RotatesHLMemoryLeft) 
{
    u16 hl = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(hl, 0x80);
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int cycles = op_rlc_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(hl), 0x01);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 16);

    mmu.write(hl, 0x00);
    op_rlc_hl(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, RLCA_RotatesARegisterLeft) 
{
    cpu.reg(ProcessingUnit::Register::A) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    op_rlc_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x01);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, RRCB_RotatesBRegisterRight) 
{
    cpu.reg(ProcessingUnit::Register::B) = 0x01;
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    op_rrc_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x80);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());

    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    op_rrc_b(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, RRCC_RotatesCRegisterRight) 
{
    cpu.reg(ProcessingUnit::Register::C) = 0x01;
    op_rrc_c(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x80);
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, RRCD_RotatesDRegisterRight) 
{
    cpu.reg(ProcessingUnit::Register::D) = 0x01;
    op_rrc_d(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x80);
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, RRCE_RotatesERegisterRight) 
{
    cpu.reg(ProcessingUnit::Register::E) = 0x01;
    op_rrc_e(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x80);
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, RRCH_RotatesHRegisterRight) 
{
    cpu.reg(ProcessingUnit::Register::H) = 0x01;
    op_rrc_h(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x80);
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, RRCL_RotatesLRegisterRight) 
{
    cpu.reg(ProcessingUnit::Register::L) = 0x01;
    op_rrc_l(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x80);
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, RRCHL_RotatesHLMemoryRight) 
{
    u16 hl = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(hl, 0x01);
    cpu.setFlag(ProcessingUnit::Flag::C , false);

    const int cycles = op_rrc_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(hl), 0x80);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, RRCA_RotatesARegisterRight) 
{
    cpu.reg(ProcessingUnit::Register::A) = 0x01;
    op_rrc_a(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x80);
    EXPECT_TRUE(cpu.get_flag_c());
}
