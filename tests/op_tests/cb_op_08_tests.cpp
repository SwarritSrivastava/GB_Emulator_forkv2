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

TEST_F(CB_OpcodesCPUTest, RES_0_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xFF;
    int cycles = op_res_0_b(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xFE);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_0_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xFF;
    int cycles = op_res_0_c(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xFE);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_0_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xFF;
    int cycles = op_res_0_d(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0xFE);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_0_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xFF;
    int cycles = op_res_0_e(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xFE);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_0_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xFF;
    int cycles = op_res_0_h(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xFE);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_0_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xFF;
    int cycles = op_res_0_l(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0xFE);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_0_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xFF);
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    int cycles = op_res_0_hl(cpu, mmu);
    EXPECT_EQ(mmu.read(addr), 0xFE);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xF0);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, RES_0_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xFF;
    int cycles = op_res_0_a(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFE);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_1_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0xFF;
    int cycles = op_res_1_b(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xFD);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_1_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0xFF;
    int cycles = op_res_1_c(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xFD);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_1_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0xFF;
    int cycles = op_res_1_d(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0xFD);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_1_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0xFF;
    int cycles = op_res_1_e(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xFD);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_1_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0xFF;
    int cycles = op_res_1_h(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xFD);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_1_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0xFF;
    int cycles = op_res_1_l(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0xFD);
    EXPECT_EQ(cycles, 8);
}

TEST_F(CB_OpcodesCPUTest, RES_1_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;
    mmu.write(addr, 0xFF);
    int cycles = op_res_1_hl(cpu, mmu);
    EXPECT_EQ(mmu.read(addr), 0xFD);
    EXPECT_EQ(cycles, 16);
}

TEST_F(CB_OpcodesCPUTest, RES_1_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0xFF;
    int cycles = op_res_1_a(cpu, mmu);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFD);
    EXPECT_EQ(cycles, 8);
}
