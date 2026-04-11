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

TEST_F(CB_OpcodesCPUTest, RES_2_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xFF;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    int cycles = op_res_2_b(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xFB);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_2_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xFF;
    int cycles = op_res_2_c(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xFB);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_2_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xFF;
    int cycles = op_res_2_d(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0xFB);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_2_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xFF;
    int cycles = op_res_2_e(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xFB);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_2_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xFF;
    int cycles = op_res_2_h(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xFB);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_2_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xFF;
    int cycles = op_res_2_l(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0xFB);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_2_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xFF);
    int cycles = op_res_2_hl(cpu, mmu);
    EXPECT_EQ(mmu.read(addr), 0xFB);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, RES_2_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xFF;
    int cycles = op_res_2_a(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFB);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_3_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xFF;
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    int cycles = op_res_3_b(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xF7);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_3_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xFF;
    int cycles = op_res_3_c(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xF7);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_3_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xFF;
    int cycles = op_res_3_d(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0xF7);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_3_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xFF;
    int cycles = op_res_3_e(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xF7);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_3_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xFF;
    int cycles = op_res_3_h(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xF7);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_3_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xFF;
    int cycles = op_res_3_l(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0xF7);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_3_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xFF);
    int cycles = op_res_3_hl(cpu, mmu);
    EXPECT_EQ(mmu.read(addr), 0xF7);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, RES_3_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xFF;
    int cycles = op_res_3_a(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xF7);
    EXPECT_EQ(cycles, 8);
}
