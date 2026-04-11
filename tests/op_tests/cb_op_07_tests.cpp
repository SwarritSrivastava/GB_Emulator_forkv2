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

TEST_F(CB_OpcodesCPUTest, BIT_6_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xBF; // 1011 1111
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_6_b(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xBF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::B) = 0x40; // 0100 0000
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_6_b(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x40);
}

TEST_F(CB_OpcodesCPUTest, BIT_6_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xBF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_6_c(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xBF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::C) = 0x40;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_6_c(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x40);
}

TEST_F(CB_OpcodesCPUTest, BIT_6_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xBF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_6_d(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0xBF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::D) = 0x40;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_6_d(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x40);
}

TEST_F(CB_OpcodesCPUTest, BIT_6_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xBF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_6_e(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xBF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::E) = 0x40;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_6_e(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x40);
}

TEST_F(CB_OpcodesCPUTest, BIT_6_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xBF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_6_h(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xBF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::H) = 0x40;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_6_h(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x40);
}

TEST_F(CB_OpcodesCPUTest, BIT_6_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xBF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_6_l(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0xBF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::L) = 0x40;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_6_l(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x40);
}

TEST_F(CB_OpcodesCPUTest, BIT_6_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xBF);
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_6_hl(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(mmu.read(addr), 0xBF);
    EXPECT_EQ(cycles, 12);

    mmu.write(addr, 0x40);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_6_hl(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(mmu.read(addr), 0x40);
}

TEST_F(CB_OpcodesCPUTest, BIT_6_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xBF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_6_a(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xBF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::A) = 0x40;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_6_a(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x40);
}

TEST_F(CB_OpcodesCPUTest, BIT_7_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x7F; // 0111 1111
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_7_b(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x7F);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::B) = 0x80; // 1000 0000
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_7_b(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x80);
}

TEST_F(CB_OpcodesCPUTest, BIT_7_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x7F;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_7_c(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x7F);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::C) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_7_c(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x80);
}

TEST_F(CB_OpcodesCPUTest, BIT_7_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x7F;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_7_d(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x7F);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::D) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_7_d(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x80);
}

TEST_F(CB_OpcodesCPUTest, BIT_7_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x7F;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_7_e(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x7F);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::E) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_7_e(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x80);
}

TEST_F(CB_OpcodesCPUTest, BIT_7_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x7F;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_7_h(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x7F);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::H) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_7_h(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x80);
}

TEST_F(CB_OpcodesCPUTest, BIT_7_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x7F;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_7_l(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x7F);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::L) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_7_l(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x80);
}

TEST_F(CB_OpcodesCPUTest, BIT_7_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0x7F);
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_7_hl(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(mmu.read(addr), 0x7F);
    EXPECT_EQ(cycles, 12);

    mmu.write(addr, 0x80);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_7_hl(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(mmu.read(addr), 0x80);
}

TEST_F(CB_OpcodesCPUTest, BIT_7_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x7F;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_7_a(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x7F);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::A) = 0x80;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_7_a(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x80);
}
