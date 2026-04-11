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

// --- SET 0 ---
TEST_F(CB_OpcodesCPUTest, SET_0_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_0_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x01);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_0_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_0_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x01);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_0_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_0_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x01);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_0_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_0_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x01);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_0_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_0_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x01);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_0_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_0_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x01);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_0_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0x00);
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_0_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(addr), 0x01);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, SET_0_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_0_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x01);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

// --- SET 1 ---
TEST_F(CB_OpcodesCPUTest, SET_1_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_1_b(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x02);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_1_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_1_c(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x02);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_1_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_1_d(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x02);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_1_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_1_e(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x02);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_1_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_1_h(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x02);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_1_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_1_l(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x02);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, SET_1_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0x00);
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_1_hl(cpu, mmu);

    EXPECT_EQ(mmu.read(addr), 0x02);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, SET_1_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;

    int cycles = op_set_1_a(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x02);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}
