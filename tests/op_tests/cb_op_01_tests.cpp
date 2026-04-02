#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/cb_opcodes.hpp"
#include "../../include/opcodes.hpp"

class CB_Op01_Test : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(CB_Op01_Test, RL_B_RotatesThroughCarry) {
    cpu.reg(ProcessingUnit::Register::B) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    int cycles = op_rl_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::B) = 0x01;
    cpu.setFlag(ProcessingUnit::Flag::C, true);

    op_rl_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x03);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RL_C_RotatesThroughCarry) {
    cpu.reg(ProcessingUnit::Register::C) = 0x85;
    cpu.setFlag(ProcessingUnit::Flag::C, true);

    op_rl_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x0B);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RL_D_RotatesThroughCarry) {
    cpu.reg(ProcessingUnit::Register::D) = 0x11;
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    op_rl_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x22);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RL_E_RotatesThroughCarry) {
    cpu.reg(ProcessingUnit::Register::E) = 0xFF;
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    op_rl_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xFE);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RL_H_RotatesThroughCarry) {
    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.setFlag(ProcessingUnit::Flag::C, true);

    op_rl_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x01);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RL_L_RotatesThroughCarry) {
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    op_rl_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x00);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RL_HL_RotatesThroughCarry) {
    u16 hl = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(hl, 0x80);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    int cycles = op_rl_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(hl), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_EQ(cycles, 16);

    mmu.write(hl, 0x01);
    cpu.setFlag(ProcessingUnit::Flag::C, true);

    op_rl_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(hl), 0x03);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RL_A_RotatesThroughCarry) {
    cpu.reg(ProcessingUnit::Register::A) = 0x95;
    cpu.setFlag(ProcessingUnit::Flag::C, true);

    op_rl_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x2B);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RR_B_RotatesRightThroughCarry) {
    cpu.reg(ProcessingUnit::Register::B) = 0x01;
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    int cycles = op_rr_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::B) = 0x8A;
    cpu.setFlag(ProcessingUnit::Flag::C, true);

    op_rr_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xC5);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RR_C_RotatesRightThroughCarry) {
    cpu.reg(ProcessingUnit::Register::C) = 0x8A;
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    op_rr_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x45);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RR_D_RotatesRightThroughCarry) {
    cpu.reg(ProcessingUnit::Register::D) = 0x01;
    cpu.setFlag(ProcessingUnit::Flag::C, true);

    op_rr_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x80);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RR_E_RotatesRightThroughCarry) {
    cpu.reg(ProcessingUnit::Register::E) = 0xFF;
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    op_rr_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x7F);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RR_H_RotatesRightThroughCarry) {
    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.setFlag(ProcessingUnit::Flag::C, true);

    op_rr_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x80);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RR_L_RotatesRightThroughCarry) {
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    op_rr_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x00);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RR_HL_RotatesRightThroughCarry) {
    u16 hl = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(hl, 0x01);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    int cycles = op_rr_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(hl), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_EQ(cycles, 16);

    mmu.write(hl, 0x8A);
    cpu.setFlag(ProcessingUnit::Flag::C, true);

    op_rr_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(hl), 0xC5);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_Op01_Test, RR_A_RotatesRightThroughCarry) {
    cpu.reg(ProcessingUnit::Register::A) = 0x3B;
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    op_rr_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x1D);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}
