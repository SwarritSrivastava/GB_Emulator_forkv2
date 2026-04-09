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

TEST_F(CB_OpcodesCPUTest, BIT_2_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xFB; // 1111 1011
    int cycles = op_bit_2_b(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::B) = 0x04; // 0000 0100
    op_bit_2_b(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_2_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xFB;
    int cycles = op_bit_2_c(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::C) = 0x04;
    op_bit_2_c(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_2_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xFB;
    int cycles = op_bit_2_d(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::D) = 0x04;
    op_bit_2_d(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_2_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xFB;
    int cycles = op_bit_2_e(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::E) = 0x04;
    op_bit_2_e(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_2_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xFB;
    int cycles = op_bit_2_h(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::H) = 0x04;
    op_bit_2_h(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_2_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xFB;
    int cycles = op_bit_2_l(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::L) = 0x04;
    op_bit_2_l(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_2_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xFB);
    int cycles = op_bit_2_hl(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 12);
    mmu.write(addr, 0x04);
    op_bit_2_hl(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_2_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xFB;
    int cycles = op_bit_2_a(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::A) = 0x04;
    op_bit_2_a(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_3_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xF7; // 1111 0111
    int cycles = op_bit_3_b(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::B) = 0x08; // 0000 1000
    op_bit_3_b(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_3_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xF7;
    int cycles = op_bit_3_c(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::C) = 0x08;
    op_bit_3_c(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_3_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xF7;
    int cycles = op_bit_3_d(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::D) = 0x08;
    op_bit_3_d(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_3_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xF7;
    int cycles = op_bit_3_e(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::E) = 0x08;
    op_bit_3_e(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_3_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xF7;
    int cycles = op_bit_3_h(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::H) = 0x08;
    op_bit_3_h(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_3_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xF7;
    int cycles = op_bit_3_l(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::L) = 0x08;
    op_bit_3_l(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_3_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xF7);
    int cycles = op_bit_3_hl(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 12);
    mmu.write(addr, 0x08);
    op_bit_3_hl(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_3_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xF7;
    int cycles = op_bit_3_a(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::A) = 0x08;
    op_bit_3_a(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}
