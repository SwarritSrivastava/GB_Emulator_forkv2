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

// --- SET 4 ---
TEST_F(CB_OpcodesCPUTest, SET_4_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_4_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x10);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_4_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_4_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x10);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_4_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_4_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x10);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_4_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_4_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x10);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_4_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_4_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x10);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_4_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_4_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x10);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_4_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0x00);
    cpu.normalizeFlags();

    int cycles = op_set_4_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(addr), 0x10);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, SET_4_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_4_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x10);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

// --- SET 5 ---
TEST_F(CB_OpcodesCPUTest, SET_5_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_5_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x20);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_5_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_5_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x20);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_5_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_5_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x20);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_5_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_5_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x20);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_5_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_5_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x20);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_5_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_5_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x20);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_5_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0x00);
    cpu.normalizeFlags();

    int cycles = op_set_5_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(addr), 0x20);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, SET_5_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_5_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x20);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}
