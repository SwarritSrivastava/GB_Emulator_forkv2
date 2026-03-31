#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, ADD_A_B_AddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x8A;
    cpu.reg(ProcessingUnit::Register::B) = 0x23;
    cpu.reg(ProcessingUnit::Register::F) = 0x80; // initial flags shouldn't matter

    const int cycles = op_add_a_b(cpu, mmu);
    EXPECT_EQ(cycles, 4);

    // sum = 0x8A + 0x23 = 0xAD
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xAD);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::B), 0x23); // unchanged
    EXPECT_EQ(cpu.get_flag_z(), 0); // result != 0
    EXPECT_EQ(cpu.get_flag_n(), 0); // always reset
    EXPECT_EQ(cpu.get_flag_h(), 0); // (0xA + 0x3 = 0xD) → no half carry
    EXPECT_EQ(cpu.get_flag_c(), 0); // no overflow beyond 0xFF
}

TEST_F(OpcodesCPUTest, ADD_A_C_AddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x8A;
    cpu.reg(ProcessingUnit::Register::C) = 0x23;
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_add_a_c(cpu, mmu);
    EXPECT_EQ(cycles, 4);

    // 0x8A + 0x23 = 0xAD
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xAD);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::C), 0x23);

    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}

TEST_F(OpcodesCPUTest, ADD_A_D_AddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x8A;
    cpu.reg(ProcessingUnit::Register::D) = 0x23;
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_add_a_d(cpu, mmu);
    EXPECT_EQ(cycles, 4);

    // 0x8A + 0x23 = 0xAD
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xAD);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::D), 0x23);

    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}

TEST_F(OpcodesCPUTest, ADD_A_E_AddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x8A;
    cpu.reg(ProcessingUnit::Register::E) = 0x23;
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_add_a_e(cpu, mmu);
    EXPECT_EQ(cycles, 4);

    // 0x8A + 0x23 = 0xAD
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xAD);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::E), 0x23);

    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}

TEST_F(OpcodesCPUTest, ADD_A_H_AddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x8A;
    cpu.reg(ProcessingUnit::Register::H) = 0x23;
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_add_a_h(cpu, mmu);
    EXPECT_EQ(cycles, 4);

    // 0x8A + 0x23 = 0xAD
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xAD);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::H), 0x23);

    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}

TEST_F(OpcodesCPUTest, ADD_A_LAddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x8A;
    cpu.reg(ProcessingUnit::Register::L) = 0x23;
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_add_a_l(cpu, mmu);
    EXPECT_EQ(cycles, 4);

    // 0x8A + 0x23 = 0xAD
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xAD);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::L), 0x23);

    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}

TEST_F(OpcodesCPUTest, ADD_A_HL_AddsMemoryAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x8A;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;

    mmu.write(0xC000, 0x23); // value at (HL)

    const int cycles = op_add_a_hl(cpu, mmu);
    EXPECT_EQ(cycles, 8);

    // 0x8A + 0x23 = 0xAD
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::A)), 0xAD);

    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 0);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}

TEST_F(OpcodesCPUTest, ADD_A_HL_EdgeCase1) // Half Carry from memory
{
    cpu.reg(ProcessingUnit::Register::A) = 0x0F;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;

    mmu.write(0xC000, 0x01); // value at (HL)

    const int cycles = op_add_a_hl(cpu, mmu);
    EXPECT_EQ(cycles, 8);

    // 0x8A + 0x23 = 0xAD
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::A)), 0x10);

    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}

TEST_F(OpcodesCPUTest, ADD_A_HL_EdgeCase2) // Carry zero from memory
{
    cpu.reg(ProcessingUnit::Register::A) = 0xFF;
    cpu.reg(ProcessingUnit::Register::H) = 0xC0;
    cpu.reg(ProcessingUnit::Register::L) = 0x00;

    mmu.write(0xC000, 0x01); // value at (HL)

    const int cycles = op_add_a_hl(cpu, mmu);
    EXPECT_EQ(cycles, 8);

    // 0x8A + 0x23 = 0xAD
    EXPECT_EQ(static_cast<int>(cpu.reg(ProcessingUnit::Register::A)), 0x00);

    EXPECT_EQ(cpu.get_flag_z(), 1);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 1);
}

TEST_F(OpcodesCPUTest, ADD_A_A_AddsAndUpdatesFlags)
{
    cpu.reg(ProcessingUnit::Register::A) = 0x4A;
    cpu.reg(ProcessingUnit::Register::F) = 0x80;

    const int cycles = op_add_a_a(cpu, mmu);
    EXPECT_EQ(cycles, 4);

    // 0x4A + 0x43 = 0x94
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x94);

    EXPECT_EQ(cpu.get_flag_z(), 0);
    EXPECT_EQ(cpu.get_flag_n(), 0);
    EXPECT_EQ(cpu.get_flag_h(), 1);
    EXPECT_EQ(cpu.get_flag_c(), 0);
}