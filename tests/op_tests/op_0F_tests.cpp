#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, LDH_A_A8_LoadsValue)
{
    u8 im_val = 0x42;
    u8 exp_val = 0xDE;
    u16 target = 0xFF00 | im_val;
    u16 init_pc = 0xC001;

    cpu.set_pc(init_pc);

    mmu.write(init_pc , im_val);
    mmu.write(target, exp_val);

    int cycles = op_ldh_a_a8(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), exp_val);
    EXPECT_EQ(cpu.get_pc(), init_pc + 1);
    EXPECT_EQ(cycles, 12);
}

TEST_F(OpcodesCPUTest, POP_AF_PopsValueFromStackAndMasksFlags)
{
    u16 initial_sp = 0xD000;
    u8 val_f = 0xAB; // 1010 1011
    u8 val_a = 0x12;
    
    cpu.set_sp(initial_sp);
    mmu.write(initial_sp, val_f);
    mmu.write(initial_sp + 1, val_a);

    int cycles = op_pop_af(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), val_a);
    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::F), 0xA0);
    EXPECT_EQ(cpu.get_sp(), initial_sp + 2);
    EXPECT_EQ(cycles, 12);
}

TEST_F(OpcodesCPUTest, LD_A_C_F2_LoadsValueFromHighMemory)
{
    u8 c_val = 0x42;
    u8 exp_val = 0xDE;
    u16 target = 0xFF00 | c_val;
    u16 initial_pc = 0x0100;

    cpu.reg(ProcessingUnit::Register::C) = c_val;
    mmu.write(target, exp_val);
    cpu.set_pc(initial_pc);

    int cycles = op_ld_a_c_f2(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), exp_val);
    EXPECT_EQ(cpu.get_pc(), initial_pc);
    EXPECT_EQ(cycles, 8);
}

TEST_F(OpcodesCPUTest, DI_F3_DisablesInterrupts)
{
    cpu.setIME(true);
    ASSERT_TRUE(cpu.getIME());

    int cycles = op_di(cpu, mmu);

    EXPECT_FALSE(cpu.getIME());
    EXPECT_EQ(cycles, 4);
}

TEST_F(OpcodesCPUTest, PUSH_AF_PushesValueToStack)
{
    u16 initial_sp = 0xD000;
    u8 val_a = 0x12;
    u8 val_f = 0x80;
    
    cpu.reg(ProcessingUnit::Register::A) = val_a;
    cpu.reg(ProcessingUnit::Register::F) = val_f;
    cpu.set_sp(initial_sp);

    int cycles = op_push_af(cpu, mmu);

    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), val_a);
    EXPECT_EQ(mmu.read(initial_sp - 2), val_f);
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, OR_D8_TakesOR_A) 
{
    cpu.reg(ProcessingUnit::Register::A) = 0x55;
    u16 init_pc = 0xD000;
    
    cpu.set_pc(init_pc);
    mmu.write(init_pc, 0xAA);
    int cycles = op_or_d8(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0xFF);
    EXPECT_EQ(cpu.get_pc(), init_pc + 1);
    EXPECT_EQ(cpu.get_flag_z(), false);
    EXPECT_EQ(cpu.get_flag_n(), false);
    EXPECT_EQ(cpu.get_flag_h(), false);
    EXPECT_EQ(cpu.get_flag_c(), false);
    EXPECT_EQ(cycles, 8);

    cpu.reg(ProcessingUnit::Register::A) = 0x00;
    u16 init_pc2 = 0xD100;
    
    cpu.set_pc(init_pc2);
    mmu.write(init_pc2, 0x00);
    op_or_d8(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x00);
    EXPECT_EQ(cpu.get_pc(), init_pc2 + 1);
    EXPECT_EQ(cpu.get_flag_z(), true);
}

TEST_F(OpcodesCPUTest, RST_30_PushesPCAndJumps)
{
    u16 initial_pc = 0xC001;
    u16 initial_sp = 0xD000;
    
    cpu.set_pc(initial_pc);
    cpu.set_sp(initial_sp);

    int cycles = op_rst_30(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), 0x30);
    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), (initial_pc >> 8) & 0xFF);
    EXPECT_EQ(mmu.read(initial_sp - 2), initial_pc & 0xFF);
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, LD_HL_SP_E8_AddsSignedOffsetAndUpdatesFlags)
{
    u16 initial_sp = 0xFFFA;
    u16 initial_pc = 0xC000;
    u8 offset = 0x0A;

    cpu.set_sp(initial_sp);
    cpu.set_pc(initial_pc);
    cpu.reg(ProcessingUnit::Register::F) = 0xF0;
    mmu.write(initial_pc, offset);

    int cycles = op_ld_hl_sp_e8(cpu, mmu);

    EXPECT_EQ(cpu.get_hl(), static_cast<u16>(initial_sp + 10));
    EXPECT_EQ(cpu.get_pc(), initial_pc + 1);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_FALSE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_TRUE(cpu.get_flag_c());
    EXPECT_EQ(cycles, 12);
}

TEST_F(OpcodesCPUTest, LD_SP_HL_CopiesRegisterPair)
{
    u16 hl = 0xC123;
    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>(hl >> 8);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(hl & 0xFF);

    int cycles = op_ld_sp_hl(cpu, mmu);

    EXPECT_EQ(cpu.get_sp(), hl);
    EXPECT_EQ(cycles, 8);
}

TEST_F(OpcodesCPUTest, LD_A_A16_LoadsFromAbsoluteAddress)
{
    u16 initial_pc = 0xC010;
    u16 addr = 0xD234;
    u8 value = 0x7E;

    cpu.set_pc(initial_pc);
    mmu.write(initial_pc, static_cast<u8>(addr & 0xFF));
    mmu.write(initial_pc + 1, static_cast<u8>((addr >> 8) & 0xFF));
    mmu.write(addr, value);

    int cycles = op_ld_a_a16(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), value);
    EXPECT_EQ(cpu.get_pc(), initial_pc + 2);
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, EI_FB_EnablesInterrupts)
{
    cpu.setIME(false);
    ASSERT_FALSE(cpu.getIME());

    int cycles = op_ei(cpu, mmu);

    EXPECT_TRUE(cpu.getIME());
    EXPECT_EQ(cycles, 4);
}

TEST_F(OpcodesCPUTest, CP_D8_SetsFlagsWithoutChangingA)
{
    u16 initial_pc = 0xC020;
    cpu.set_pc(initial_pc);
    cpu.reg(ProcessingUnit::Register::A) = 0x3C;
    mmu.write(initial_pc, 0x2F);

    int cycles = op_cp_d8(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x3C);
    EXPECT_EQ(cpu.get_pc(), initial_pc + 1);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cycles, 8);

    cpu.set_pc(initial_pc + 1);
    mmu.write(initial_pc + 1, 0x3C);
    op_cp_d8(cpu, mmu);

    EXPECT_TRUE(cpu.get_flag_z());
}

TEST_F(OpcodesCPUTest, RST_38_PushesPCAndJumps)
{
    u16 initial_pc = 0xC123;
    u16 initial_sp = 0xD200;

    cpu.set_pc(initial_pc);
    cpu.set_sp(initial_sp);

    int cycles = op_rst_38(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), 0x38);
    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), (initial_pc >> 8) & 0xFF);
    EXPECT_EQ(mmu.read(initial_sp - 2), initial_pc & 0xFF);
    EXPECT_EQ(cycles, 16);
}
