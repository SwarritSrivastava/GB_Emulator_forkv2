#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, LDH_A8_A_WritesToHighMemory)
{
    u8 a8 = 0x42;
    u16 target = 0xFF00 | a8;
    u16 init_pc = 0xC010;

    cpu.reg(ProcessingUnit::Register::A) = 0xDE;
    cpu.set_pc(init_pc);
    mmu.write(init_pc, a8);

    int cycles = op_ldh_a8_a(cpu, mmu);

    EXPECT_EQ(mmu.read(target), 0xDE);
    EXPECT_EQ(cpu.get_pc(), init_pc + 1);
    EXPECT_EQ(cycles, 12);
}

TEST_F(OpcodesCPUTest, POP_HL_PopsValueFromStack)
{
    u16 initial_sp = 0xD000;
    u8 val_l = 0x34;
    u8 val_h = 0x12;

    cpu.set_sp(initial_sp);
    mmu.write(initial_sp, val_l);
    mmu.write(initial_sp + 1, val_h);

    int cycles = op_pop_hl(cpu, mmu);

    EXPECT_EQ(cpu.get_hl(), static_cast<u16>(0x1234));
    EXPECT_EQ(cpu.get_sp(), initial_sp + 2);
    EXPECT_EQ(cycles, 12);
}

TEST_F(OpcodesCPUTest, LD_C_A_E2_WritesToHighMemory)
{
    u8 c_val = 0x77;
    u16 target = 0xFF00 | c_val;

    cpu.reg(ProcessingUnit::Register::C) = c_val;
    cpu.reg(ProcessingUnit::Register::A) = 0x5A;

    int cycles = op_ld_c_a_e2(cpu, mmu);

    EXPECT_EQ(mmu.read(target), 0x5A);
    EXPECT_EQ(cycles, 8);
}

TEST_F(OpcodesCPUTest, PUSH_HL_PushesValueToStack)
{
    u16 initial_sp = 0xD100;
    cpu.set_sp(initial_sp);
    cpu.reg(ProcessingUnit::Register::H) = 0x12;
    cpu.reg(ProcessingUnit::Register::L) = 0x34;

    int cycles = op_push_hl(cpu, mmu);

    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), 0x12);
    EXPECT_EQ(mmu.read(initial_sp - 2), 0x34);
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, AND_D8_UpdatesFlags)
{
    u16 init_pc = 0xC020;
    cpu.set_pc(init_pc);
    cpu.reg(ProcessingUnit::Register::A) = 0xF0;
    mmu.write(init_pc, 0x0F);

    int cycles = op_and_d8(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_EQ(cpu.get_pc(), init_pc + 1);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cycles, 8);
}

TEST_F(OpcodesCPUTest, RST_20_PushesPCAndJumps)
{
    u16 initial_pc = 0xC001;
    u16 initial_sp = 0xD200;

    cpu.set_pc(initial_pc);
    cpu.set_sp(initial_sp);

    int cycles = op_rst_20(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), 0x20);
    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), (initial_pc >> 8) & 0xFF);
    EXPECT_EQ(mmu.read(initial_sp - 2), initial_pc & 0xFF);
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, ADD_SP_E8_AddsSignedOffsetAndUpdatesFlags)
{
    u16 initial_sp = 0xFFF8;
    u16 initial_pc = 0xC030;
    u8 offset = 0x0A;

    cpu.set_sp(initial_sp);
    cpu.set_pc(initial_pc);
    cpu.normalizeFlags();
    mmu.write(initial_pc, offset);

    int cycles = op_add_sp_e8(cpu, mmu);

    EXPECT_EQ(cpu.get_sp(), static_cast<u16>(initial_sp + 10));
    EXPECT_EQ(cpu.get_pc(), initial_pc + 1);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, JP_HL_JumpsToRegisterPair)
{
    cpu.reg(ProcessingUnit::Register::H) = 0xC1;
    cpu.reg(ProcessingUnit::Register::L) = 0x23;

    int cycles = op_jp_hl(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), 0xC123);
    EXPECT_EQ(cycles, 4);
}

TEST_F(OpcodesCPUTest, LD_A16_A_WritesToAbsoluteAddress)
{
    u16 initial_pc = 0xC040;
    u16 addr = 0xD234;

    cpu.set_pc(initial_pc);
    cpu.reg(ProcessingUnit::Register::A) = 0x7E;
    mmu.write(initial_pc, static_cast<u8>(addr & 0xFF));
    mmu.write(initial_pc + 1, static_cast<u8>((addr >> 8) & 0xFF));

    int cycles = op_ld_a16_a(cpu, mmu);

    EXPECT_EQ(mmu.read(addr), 0x7E);
    EXPECT_EQ(cpu.get_pc(), initial_pc + 2);
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, XOR_D8_UpdatesFlags)
{
    u16 init_pc = 0xC050;
    cpu.set_pc(init_pc);
    cpu.reg(ProcessingUnit::Register::A) = 0xAA;
    mmu.write(init_pc, 0xAA);

    int cycles = op_xor_d8(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_EQ(cpu.get_pc(), init_pc + 1);
    EXPECT_TRUE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_FALSE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cycles, 8);
}

TEST_F(OpcodesCPUTest, RST_28_PushesPCAndJumps)
{
    u16 initial_pc = 0xC123;
    u16 initial_sp = 0xD200;

    cpu.set_pc(initial_pc);
    cpu.set_sp(initial_sp);

    int cycles = op_rst_28(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), 0x28);
    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), (initial_pc >> 8) & 0xFF);
    EXPECT_EQ(mmu.read(initial_sp - 2), initial_pc & 0xFF);
    EXPECT_EQ(cycles, 16);
}
