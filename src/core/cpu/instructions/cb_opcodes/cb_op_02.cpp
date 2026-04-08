#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_sla_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB20
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1);

    cpu.reg(ProcessingUnit::Register::B) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sla_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB21
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1);

    cpu.reg(ProcessingUnit::Register::C) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sla_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB22
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1);

    cpu.reg(ProcessingUnit::Register::D) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sla_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB23
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1);

    cpu.reg(ProcessingUnit::Register::E) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sla_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB24
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1);

    cpu.reg(ProcessingUnit::Register::H) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sla_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB25
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1);

    cpu.reg(ProcessingUnit::Register::L) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sla_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB26
{
    u16 address = cpu.get_hl();
    u8 value = mmu.read(address);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1);

    mmu.write(address, result);
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(4);
}

int op_sla_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB27
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 carry = (value >> 7) & 1;
    u8 result = (value << 1);

    cpu.reg(ProcessingUnit::Register::A) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sra_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB28
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (value & 0x80);

    cpu.reg(ProcessingUnit::Register::B) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sra_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB29
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (value & 0x80);

    cpu.reg(ProcessingUnit::Register::C) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sra_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB2A
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (value & 0x80);

    cpu.reg(ProcessingUnit::Register::D) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sra_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB2B
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (value & 0x80);

    cpu.reg(ProcessingUnit::Register::E) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sra_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB2C
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (value & 0x80);

    cpu.reg(ProcessingUnit::Register::H) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sra_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB2D
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (value & 0x80);

    cpu.reg(ProcessingUnit::Register::L) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}

int op_sra_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB2E
{
    u16 address = cpu.get_hl();
    u8 value = mmu.read(address);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (value & 0x80);

    mmu.write(address, result);
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(4);
}

int op_sra_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB2F
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 carry = value & 1;
    u8 result = (value >> 1) | (value & 0x80);

    cpu.reg(ProcessingUnit::Register::A) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);

    return totalMachineCycles(2);
}