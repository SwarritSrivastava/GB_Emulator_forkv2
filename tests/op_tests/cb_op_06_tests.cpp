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

TEST_F(CB_OpcodesCPUTest, BIT_4_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xEF; // 1110 1111
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_4_b(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xEF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::B) = 0x10; // 0001 0000
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_4_b(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x10);
}

TEST_F(CB_OpcodesCPUTest, BIT_4_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xEF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_4_c(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xEF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::C) = 0x10;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_4_c(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x10);
}

TEST_F(CB_OpcodesCPUTest, BIT_4_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xEF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_4_d(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0xEF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::D) = 0x10;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_4_d(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x10);
}

TEST_F(CB_OpcodesCPUTest, BIT_4_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xEF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_4_e(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xEF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::E) = 0x10;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_4_e(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x10);
}

TEST_F(CB_OpcodesCPUTest, BIT_4_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xEF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_4_h(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xEF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::H) = 0x10;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_4_h(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x10);
}

TEST_F(CB_OpcodesCPUTest, BIT_4_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xEF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_4_l(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0xEF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::L) = 0x10;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_4_l(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x10);
}

TEST_F(CB_OpcodesCPUTest, BIT_4_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xEF);
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_4_hl(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(mmu.read(addr), 0xEF);
    EXPECT_EQ(cycles, 12);

    mmu.write(addr, 0x10);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_4_hl(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(mmu.read(addr), 0x10);
}

TEST_F(CB_OpcodesCPUTest, BIT_4_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xEF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_4_a(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xEF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::A) = 0x10;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_4_a(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x10);
}

TEST_F(CB_OpcodesCPUTest, BIT_5_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xDF; // 1101 1111
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_5_b(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xDF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::B) = 0x20; // 0010 0000
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_5_b(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x20);
}

TEST_F(CB_OpcodesCPUTest, BIT_5_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xDF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_5_c(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xDF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::C) = 0x20;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_5_c(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x20);
}

TEST_F(CB_OpcodesCPUTest, BIT_5_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xDF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_5_d(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0xDF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::D) = 0x20;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_5_d(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x20);
}

TEST_F(CB_OpcodesCPUTest, BIT_5_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xDF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_5_e(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xDF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::E) = 0x20;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_5_e(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x20);
}

TEST_F(CB_OpcodesCPUTest, BIT_5_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xDF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_5_h(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xDF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::H) = 0x20;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_5_h(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x20);
}

TEST_F(CB_OpcodesCPUTest, BIT_5_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xDF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_5_l(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0xDF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::L) = 0x20;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_5_l(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x20);
}

TEST_F(CB_OpcodesCPUTest, BIT_5_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xDF);
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_5_hl(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(mmu.read(addr), 0xDF);
    EXPECT_EQ(cycles, 12);

    mmu.write(addr, 0x20);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_5_hl(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(mmu.read(addr), 0x20);
}

TEST_F(CB_OpcodesCPUTest, BIT_5_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xDF;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    int cycles = op_bit_5_a(cpu, mmu);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xDF);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::A) = 0x20;
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    op_bit_5_a(cpu, mmu);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x20);
}
