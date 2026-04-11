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

TEST_F(CB_OpcodesCPUTest, SLA_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x80;
    int cycles = op_sla_b(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());

    cpu.reg(ProcessingUnit::Register::B) = 0x45;
    cycles = op_sla_b(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x8A);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SLA_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x80;
    int cycles = op_sla_c(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SLA_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x80;
    int cycles = op_sla_d(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SLA_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x80;
    int cycles = op_sla_e(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SLA_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x80;
    int cycles = op_sla_h(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SLA_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x80;
    int cycles = op_sla_l(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SLA_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = (addr >> 8) & 0xFF;
    cpu.reg(ProcessingUnit::Register::L) = addr & 0xFF;
    mmu.write(addr, 0x81);
    int cycles = op_sla_hl(cpu, mmu);
    EXPECT_EQ(cycles, 16);
    EXPECT_EQ(mmu.read(addr), 0x02);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SLA_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x80;
    int cycles = op_sla_a(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SRA_B) {
    cpu.reg(ProcessingUnit::Register::B) = 0x81;
    int cycles = op_sra_b(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0xC0);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());

    cpu.reg(ProcessingUnit::Register::B) = 0x01;
    cycles = op_sra_b(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x00);
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SRA_C) {
    cpu.reg(ProcessingUnit::Register::C) = 0x81;
    int cycles = op_sra_c(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0xC0);
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, SRA_D) {
    cpu.reg(ProcessingUnit::Register::D) = 0x81;
    int cycles = op_sra_d(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0xC0);
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, SRA_E) {
    cpu.reg(ProcessingUnit::Register::E) = 0x81;
    int cycles = op_sra_e(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0xC0);
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, SRA_H) {
    cpu.reg(ProcessingUnit::Register::H) = 0x81;
    int cycles = op_sra_h(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0xC0);
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, SRA_L) {
    cpu.reg(ProcessingUnit::Register::L) = 0x81;
    int cycles = op_sra_l(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0xC0);
    EXPECT_TRUE(cpu.get_flag_c());
}

TEST_F(CB_OpcodesCPUTest, SRA_HL) {
    u16 addr = 0xC000;
    cpu.reg(ProcessingUnit::Register::H) = (addr >> 8) & 0xFF;
    cpu.reg(ProcessingUnit::Register::L) = addr & 0xFF;
    mmu.write(addr, 0x80);
    int cycles = op_sra_hl(cpu, mmu);
    EXPECT_EQ(cycles, 16);
    EXPECT_EQ(mmu.read(addr), 0xC0);
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_FALSE(cpu.get_flag_z());
}

TEST_F(CB_OpcodesCPUTest, SRA_A) {
    cpu.reg(ProcessingUnit::Register::A) = 0x81;
    int cycles = op_sra_a(cpu, mmu);
    EXPECT_EQ(cycles, 8);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xC0);
    EXPECT_TRUE(cpu.get_flag_c());
}
