#include <gtest/gtest.h>
#include "../../include/mmu.hpp"
#include "../../include/ProcessingUnit.hpp"
#include "../../include/opcodes.hpp"

class OpcodesCPUTest : public testing::Test {
protected:
    ProcessingUnit cpu;
    MMU mmu;
};

TEST_F(OpcodesCPUTest, RET_NC_ReturnsWhenCarryClear)
{
    u16 initial_sp = 0xD000;
    u16 target = 0xC123;

    cpu.set_sp(initial_sp);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    mmu.write(initial_sp, static_cast<u8>(target & 0xFF));
    mmu.write(initial_sp + 1, static_cast<u8>((target >> 8) & 0xFF));

    int cycles = op_ret_nc(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), target);
    EXPECT_EQ(cpu.get_sp(), initial_sp + 2);
    EXPECT_EQ(cycles, 20);
}

TEST_F(OpcodesCPUTest, POP_DE_PopsValueFromStack)
{
    u16 initial_sp = 0xD010;
    cpu.set_sp(initial_sp);
    mmu.write(initial_sp, 0x78);
    mmu.write(initial_sp + 1, 0x56);

    int cycles = op_pop_de(cpu, mmu);

    EXPECT_EQ(cpu.get_de(), static_cast<u16>(0x5678));
    EXPECT_EQ(cpu.get_sp(), initial_sp + 2);
    EXPECT_EQ(cycles, 12);
}

TEST_F(OpcodesCPUTest, JP_NC_JumpsWhenCarryClear)
{
    u16 initial_pc = 0xC020;
    u16 target = 0xD234;

    cpu.set_pc(initial_pc);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    mmu.write(initial_pc, static_cast<u8>(target & 0xFF));
    mmu.write(initial_pc + 1, static_cast<u8>((target >> 8) & 0xFF));

    int cycles = op_jp_nc(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), target);
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, CALL_NC_PushesPCAndJumps)
{
    u16 initial_pc = 0xC030;
    u16 initial_sp = 0xD100;
    u16 target = 0xC555;

    cpu.set_pc(initial_pc);
    cpu.set_sp(initial_sp);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    mmu.write(initial_pc, static_cast<u8>(target & 0xFF));
    mmu.write(initial_pc + 1, static_cast<u8>((target >> 8) & 0xFF));

    int cycles = op_call_nc(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), target);
    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), static_cast<u8>((initial_pc + 2) >> 8));
    EXPECT_EQ(mmu.read(initial_sp - 2), static_cast<u8>((initial_pc + 2) & 0xFF));
    EXPECT_EQ(cycles, 24);
}

TEST_F(OpcodesCPUTest, PUSH_DE_PushesValueToStack)
{
    u16 initial_sp = 0xD200;
    cpu.set_sp(initial_sp);
    cpu.reg(ProcessingUnit::Register::D) = 0x12;
    cpu.reg(ProcessingUnit::Register::E) = 0x34;

    int cycles = op_push_de(cpu, mmu);

    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), 0x12);
    EXPECT_EQ(mmu.read(initial_sp - 2), 0x34);
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, SUB_D8_SubtractsImmediate)
{
    u16 initial_pc = 0xC040;
    cpu.set_pc(initial_pc);
    cpu.reg(ProcessingUnit::Register::A) = 0x10;
    mmu.write(initial_pc, 0x01);

    int cycles = op_sub_d8(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x0F);
    EXPECT_EQ(cpu.get_pc(), initial_pc + 1);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cycles, 8);
}

TEST_F(OpcodesCPUTest, RST_10_PushesPCAndJumps)
{
    u16 initial_pc = 0xC123;
    u16 initial_sp = 0xD300;

    cpu.set_pc(initial_pc);
    cpu.set_sp(initial_sp);

    int cycles = op_rst_10(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), 0x10);
    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), (initial_pc >> 8) & 0xFF);
    EXPECT_EQ(mmu.read(initial_sp - 2), initial_pc & 0xFF);
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, RET_C_ReturnsWhenCarrySet)
{
    u16 initial_sp = 0xD400;
    u16 target = 0xC321;

    cpu.set_sp(initial_sp);
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    mmu.write(initial_sp, static_cast<u8>(target & 0xFF));
    mmu.write(initial_sp + 1, static_cast<u8>((target >> 8) & 0xFF));

    int cycles = op_ret_c(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), target);
    EXPECT_EQ(cpu.get_sp(), initial_sp + 2);
    EXPECT_EQ(cycles, 20);
}

TEST_F(OpcodesCPUTest, RETI_PopsPCAndEnablesIME)
{
    u16 initial_sp = 0xD410;
    u16 target = 0xC456;

    cpu.set_sp(initial_sp);
    cpu.setIME(false);
    mmu.write(initial_sp, static_cast<u8>(target & 0xFF));
    mmu.write(initial_sp + 1, static_cast<u8>((target >> 8) & 0xFF));

    int cycles = op_reti(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), target);
    EXPECT_EQ(cpu.get_sp(), initial_sp + 2);
    EXPECT_TRUE(cpu.getIME());
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, JP_C_JumpsWhenCarrySet)
{
    u16 initial_pc = 0xC050;
    u16 target = 0xD999;

    cpu.set_pc(initial_pc);
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    mmu.write(initial_pc, static_cast<u8>(target & 0xFF));
    mmu.write(initial_pc + 1, static_cast<u8>((target >> 8) & 0xFF));

    int cycles = op_jp_c(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), target);
    EXPECT_EQ(cycles, 16);
}

TEST_F(OpcodesCPUTest, CALL_C_PushesPCAndJumps)
{
    u16 initial_pc = 0xC060;
    u16 initial_sp = 0xD500;
    u16 target = 0xC777;

    cpu.set_pc(initial_pc);
    cpu.set_sp(initial_sp);
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    mmu.write(initial_pc, static_cast<u8>(target & 0xFF));
    mmu.write(initial_pc + 1, static_cast<u8>((target >> 8) & 0xFF));

    int cycles = op_call_c(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), target);
    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), static_cast<u8>((initial_pc + 2) >> 8));
    EXPECT_EQ(mmu.read(initial_sp - 2), static_cast<u8>((initial_pc + 2) & 0xFF));
    EXPECT_EQ(cycles, 24);
}

TEST_F(OpcodesCPUTest, SBC_A_D8_SubtractsImmediateWithCarry)
{
    u16 initial_pc = 0xC070;
    cpu.set_pc(initial_pc);
    cpu.reg(ProcessingUnit::Register::A) = 0x10;
    cpu.setFlag(ProcessingUnit::Flag::C, true);
    mmu.write(initial_pc, 0x01);

    int cycles = op_sbc_a_d8(cpu, mmu);

    EXPECT_EQ(cpu.reg(ProcessingUnit::Register::A), 0x0E);
    EXPECT_EQ(cpu.get_pc(), initial_pc + 1);
    EXPECT_FALSE(cpu.get_flag_z());
    EXPECT_TRUE(cpu.get_flag_n());
    EXPECT_TRUE(cpu.get_flag_h());
    EXPECT_FALSE(cpu.get_flag_c());
    EXPECT_EQ(cycles, 8);
}

TEST_F(OpcodesCPUTest, RST_18_PushesPCAndJumps)
{
    u16 initial_pc = 0xC123;
    u16 initial_sp = 0xD600;

    cpu.set_pc(initial_pc);
    cpu.set_sp(initial_sp);

    int cycles = op_rst_18(cpu, mmu);

    EXPECT_EQ(cpu.get_pc(), 0x18);
    EXPECT_EQ(cpu.get_sp(), initial_sp - 2);
    EXPECT_EQ(mmu.read(initial_sp - 1), (initial_pc >> 8) & 0xFF);
    EXPECT_EQ(mmu.read(initial_sp - 2), initial_pc & 0xFF);
    EXPECT_EQ(cycles, 16);
}
