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

TEST_F(OpcodesCPUTest, LD_HL_SP_S8_Verification)
{
    cpu.set_sp(0x1000);
    cpu.set_pc(0xD000); 
    mmu.write(0xD000, 0x02); 

    int cycles = op_ld_hl_sp_e8(cpu, mmu);

    EXPECT_EQ(cpu.get_hl(), 0x1002);
    EXPECT_EQ(cpu.get_flag_z(), false);
    EXPECT_EQ(cpu.get_flag_n(), false);
    EXPECT_EQ(cycles, 12);
    cpu.set_sp(0x1000);
    cpu.set_pc(0xD100);
    mmu.write(0xD100, 0xFE); // -2

    op_ld_hl_sp_e8(cpu, mmu);

    EXPECT_EQ(cpu.get_hl(), 0x0FFE);
    EXPECT_EQ(cpu.get_flag_h(), false);
    EXPECT_EQ(cpu.get_flag_c(), false);
}