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

// --- SET 6 ---
TEST_F(CB_OpcodesCPUTest, SET_6_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_6_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x40);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_6_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_6_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x40);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_6_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_6_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x40);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_6_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_6_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x40);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_6_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_6_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x40);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_6_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_6_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x40);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_6_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0x00);
    cpu.normalizeFlags();

    int cycles = op_set_6_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(addr), 0x40);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, SET_6_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_6_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x40);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

// --- SET 7 ---
TEST_F(CB_OpcodesCPUTest, SET_7_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_7_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x80);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_7_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_7_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x80);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_7_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_7_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x80);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_7_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_7_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x80);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_7_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_7_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x80);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_7_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_7_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x80);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_7_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0x00);
    cpu.normalizeFlags();

    int cycles = op_set_7_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(addr), 0x80);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, SET_7_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_7_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x80);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}
