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

TEST_F(CB_OpcodesCPUTest, SWAP_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x12;
    int cycles = op_swap_b(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x21);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());

    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    op_swap_b(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SWAP_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xA5;
    int cycles = op_swap_c(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x5A);
}

TEST_F(CB_OpcodesCPUTest, SWAP_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xF0;
    int cycles = op_swap_d(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x0F);
}

TEST_F(CB_OpcodesCPUTest, SWAP_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x01;
    int cycles = op_swap_e(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x10);
}

TEST_F(CB_OpcodesCPUTest, SWAP_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xCC;
    int cycles = op_swap_h(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xCC);
}

TEST_F(CB_OpcodesCPUTest, SWAP_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x10;
    int cycles = op_swap_l(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x01);
}

TEST_F(CB_OpcodesCPUTest, SWAP_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = (addr >> 8) & 0xFF;
    cpu.reg(ProcessingUnit::Register::L) = addr & 0xFF;
    mmu.write(addr, 0x12);
    int cycles = op_swap_hl(cpu, mmu);
    EXPECT_EQ(cycles, 16);
    EXPECT_EQ(mmu.read(addr), 0x21);
}

TEST_F(CB_OpcodesCPUTest, SWAP_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x12;
    int cycles = op_swap_a(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x21);
}

TEST_F(CB_OpcodesCPUTest, SRL_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x81;
    int cycles = op_srl_b(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x40);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());

    cpu.reg(ProcessingUnit::Register::B) = 0x01;
    op_srl_b(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SRL_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x02;
    int cycles = op_srl_c(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x01);
    EXPECT_FALSE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, SRL_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x02;
    int cycles = op_srl_d(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x01);
}

TEST_F(CB_OpcodesCPUTest, SRL_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x02;
    int cycles = op_srl_e(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x01);
}

TEST_F(CB_OpcodesCPUTest, SRL_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x02;
    int cycles = op_srl_h(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x01);
}

TEST_F(CB_OpcodesCPUTest, SRL_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x02;
    int cycles = op_srl_l(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x01);
}

TEST_F(CB_OpcodesCPUTest, SRL_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = (addr >> 8) & 0xFF;
    cpu.reg(ProcessingUnit::Register::L) = addr & 0xFF;
    mmu.write(addr, 0x03);
    int cycles = op_srl_hl(cpu, mmu);
    EXPECT_EQ(cycles, 16);
    EXPECT_EQ(mmu.read(addr), 0x01);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SRL_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x02;
    int cycles = op_srl_a(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x01);
}