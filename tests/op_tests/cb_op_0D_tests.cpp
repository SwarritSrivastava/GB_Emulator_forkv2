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

// --- SET 2 ---
TEST_F(CB_OpcodesCPUTest, SET_2_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_2_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x04);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_2_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_2_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x04);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_2_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_2_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x04);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_2_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_2_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x04);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_2_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_2_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x04);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_2_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_2_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x04);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_2_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0x00);
    cpu.normalizeFlags();

    int cycles = op_set_2_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(addr), 0x04);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, SET_2_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_2_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x04);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

// --- SET 3 ---
TEST_F(CB_OpcodesCPUTest, SET_3_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_3_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x08);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_3_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_3_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x08);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_3_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_3_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x08);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_3_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_3_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x08);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_3_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_3_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x08);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_3_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_3_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x08);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_3_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0x00);
    cpu.normalizeFlags();

    int cycles = op_set_3_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(addr), 0x08);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, SET_3_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.normalizeFlags();

    int cycles = op_set_3_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x08);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}
