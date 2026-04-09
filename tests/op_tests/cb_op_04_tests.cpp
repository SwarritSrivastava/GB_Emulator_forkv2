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

TEST_F(CB_OpcodesCPUTest, BIT_0_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xFE;
    int cycles = op_bit_0_b(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::B) = 0x01;
    op_bit_0_b(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_0_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xFE;
    int cycles = op_bit_0_c(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::C) = 0x01;
    op_bit_0_c(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_0_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xFE;
    int cycles = op_bit_0_d(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::D) = 0x01;
    op_bit_0_d(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_0_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xFE;
    int cycles = op_bit_0_e(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::E) = 0x01;
    op_bit_0_e(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_0_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xFE;
    int cycles = op_bit_0_h(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::H) = 0x01;
    op_bit_0_h(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_0_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xFE;
    int cycles = op_bit_0_l(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::L) = 0x01;
    op_bit_0_l(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_0_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xFE);
    int cycles = op_bit_0_hl(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 12);
    mmu.write(addr, 0x01);
    op_bit_0_hl(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_0_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xFE;
    int cycles = op_bit_0_a(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::A) = 0x01;
    op_bit_0_a(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_1_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xFD; // 1111 1101
    int cycles = op_bit_1_b(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::B) = 0x02; // 0000 0010
    op_bit_1_b(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_1_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xFD;
    int cycles = op_bit_1_c(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::C) = 0x02;
    op_bit_1_c(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_1_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xFD;
    int cycles = op_bit_1_d(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::D) = 0x02;
    op_bit_1_d(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_1_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xFD;
    int cycles = op_bit_1_e(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::E) = 0x02;
    op_bit_1_e(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_1_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xFD;
    int cycles = op_bit_1_h(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::H) = 0x02;
    op_bit_1_h(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_1_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xFD;
    int cycles = op_bit_1_l(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::L) = 0x02;
    op_bit_1_l(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_1_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xFD);
    int cycles = op_bit_1_hl(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 12);
    mmu.write(addr, 0x02);
    op_bit_1_hl(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, BIT_1_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xFD;
    int cycles = op_bit_1_a(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_EQ(cycles, 8);
    cpu.reg(ProcessingUnit::Register::A) = 0x02;
    op_bit_1_a(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
}
