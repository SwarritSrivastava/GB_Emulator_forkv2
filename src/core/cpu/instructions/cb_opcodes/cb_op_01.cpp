#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_rl_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB10 
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = (value >> 7) & 1;

    u8 result = (value << 1) | old_carry;

    cpu.reg(ProcessingUnit::Register::B) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rl_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB11
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = (value >> 7) & 1;

    u8 result = (value << 1) | old_carry;

    cpu.reg(ProcessingUnit::Register::C) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rl_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB12
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = (value >> 7) & 1;

    u8 result = (value << 1) | old_carry;

    cpu.reg(ProcessingUnit::Register::D) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rl_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB13
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = (value >> 7) & 1;

    u8 result = (value << 1) | old_carry;

    cpu.reg(ProcessingUnit::Register::E) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rl_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB14
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = (value >> 7) & 1;

    u8 result = (value << 1) | old_carry;

    cpu.reg(ProcessingUnit::Register::H) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rl_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB15
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = (value >> 7) & 1;

    u8 result = (value << 1) | old_carry;

    cpu.reg(ProcessingUnit::Register::L) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rl_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB16
{
    u16 address = cpu.get_hl();
    u8 value = mmu.read(address);

    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = (value >> 7) & 1;

    u8 result = (value << 1) | old_carry;
    mmu.write(address, result);

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(4);
}

int op_rl_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB17
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = (value >> 7) & 1;

    u8 result = (value << 1) | old_carry;

    cpu.reg(ProcessingUnit::Register::A) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rr_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB18
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = value & 1;

    u8 result = (value >> 1) | (old_carry << 7);

    cpu.reg(ProcessingUnit::Register::B) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rr_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB19
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = value & 1;

    u8 result = (value >> 1) | (old_carry << 7);

    cpu.reg(ProcessingUnit::Register::C) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rr_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB1A
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = value & 1;

    u8 result = (value >> 1) | (old_carry << 7);

    cpu.reg(ProcessingUnit::Register::D) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rr_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB1B
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = value & 1;

    u8 result = (value >> 1) | (old_carry << 7);

    cpu.reg(ProcessingUnit::Register::E) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rr_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB1C
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = value & 1;

    u8 result = (value >> 1) | (old_carry << 7);

    cpu.reg(ProcessingUnit::Register::H) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rr_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB1D
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = value & 1;

    u8 result = (value >> 1) | (old_carry << 7);

    cpu.reg(ProcessingUnit::Register::L) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}

int op_rr_hl(ProcessingUnit& cpu,MMU& mmu) // 0xCB1E
{
    u16 address = cpu.get_hl();
    u8 value = mmu.read(address);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = value & 1;

    u8 result = (value >> 1) | (old_carry << 7);
    mmu.write(address,result);

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(4);
}

int op_rr_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB1F
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = value & 1;

    u8 result = (value >> 1) | (old_carry << 7);

    cpu.reg(ProcessingUnit::Register::A) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}
