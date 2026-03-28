#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_rlc_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB00
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    
    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1) | carry;

    cpu.reg(ProcessingUnit::Register::B) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rlc_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB01
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1) | carry;

    cpu.reg(ProcessingUnit::Register::C) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rlc_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB02
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1) | carry;

    cpu.reg(ProcessingUnit::Register::D) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int  op_rlc_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB03
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1) | carry;

    cpu.reg(ProcessingUnit::Register::E) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rlc_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB04
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1) | carry;

    cpu.reg(ProcessingUnit::Register::H) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rlc_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB05
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1) | carry;

    cpu.reg(ProcessingUnit::Register::L) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rlc_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB06
{
    u16 address = cpu.get_hl();
    u8 value = mmu.read(address);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1) | carry;

    mmu.write(address, result);
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(4);
}

int op_rlc_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB07
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1) | carry;

    cpu.reg(ProcessingUnit::Register::A) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rrc_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB08
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (carry << 7);

    cpu.reg(ProcessingUnit::Register::B) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rrc_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB09
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (carry << 7);

    cpu.reg(ProcessingUnit::Register::C) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rrc_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB0A
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (carry << 7);

    cpu.reg(ProcessingUnit::Register::D) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rrc_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB0B
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (carry << 7);

    cpu.reg(ProcessingUnit::Register::E) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rrc_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB0C
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (carry << 7);

    cpu.reg(ProcessingUnit::Register::H) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rrc_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB0D
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (carry << 7);

    cpu.reg(ProcessingUnit::Register::L) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_rrc_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB0E
{
    u16 address = cpu.get_hl();
    u8 value = mmu.read(address);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (carry << 7);

    mmu.write(address, result);
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(4);
}

int op_rrc_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB0F
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (carry << 7);

    cpu.reg(ProcessingUnit::Register::A) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}
