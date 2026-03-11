#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_nop(ProcessingUnit &cpu, MMU &mmu)  // 0x00
{
    return totalMachineCycles(1);
}

int op_ld_bc_d16(ProcessingUnit &cpu, MMU &mmu) // 0x01
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());

    cpu.reg(ProcessingUnit::Register::B) = hi;
    cpu.reg(ProcessingUnit::Register::C) = lo;

    return totalMachineCycles(3);
}

int op_ld_bc_a(ProcessingUnit &cpu, MMU &mmu) // 0x02
{
    const u16 addr = cpu.get_bc();      // BC contains the target address
    const u8 value = cpu.reg(ProcessingUnit::Register::A);

    mmu.write(addr, value);

    return totalMachineCycles(2);
}

int op_inc_bc(ProcessingUnit& cpu, MMU& mmu) // 0x03
{
    const u16 newValue = cpu.get_bc() + 1;

    cpu.reg(ProcessingUnit::Register::B) = (newValue >> 8) & 0xFF;
    cpu.reg(ProcessingUnit::Register::C) = newValue & 0xFF;

    return totalMachineCycles(2);
}

int op_inc_b(ProcessingUnit& cpu, MMU& mmu) // 0x04
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::B) + 1;
    cpu.reg(ProcessingUnit::Register::B) = newValue;

    return totalMachineCycles(1);
}

int op_dec_b(ProcessingUnit& cpu, MMU& mmu) // 0x05
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::B) - 1;
    cpu.reg(ProcessingUnit::Register::B) = newValue;

    return totalMachineCycles(1);
}

int op_ld_b_d8(ProcessingUnit& cpu, MMU& mmu) // 0x06
{
    cpu.inc_pc();
    const u8 newValue = mmu.read(cpu.inc_pc());

    cpu.reg(ProcessingUnit::Register::B) = newValue;

    return totalMachineCycles(2);
}

int op_rlca(ProcessingUnit& cpu, MMU& mmu) // 0x07
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);

    const u8 carry = (a >> 7) & 1;
    const u8 result = (a << 1) | carry;

    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, false);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry);

    return totalMachineCycles(1);
}

int op_ld_a16_sp(ProcessingUnit& cpu, MMU& mmu) // 0x08
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());

    const u16 addr = (hi << 8) | lo;

    mmu.write(addr, cpu.get_sp() & 0xFF);
    mmu.write(addr + 1, cpu.get_sp() >> 8);

    return totalMachineCycles(5);
}

int op_add_hl_bc(ProcessingUnit& cpu, MMU& mmu) // 0x09
{
    // a = a + b
    const u16 sum = cpu.get_hl() + cpu.get_bc();
    cpu.reg(ProcessingUnit::Register::H) = (sum >> 8) & 0xFF;
    cpu.reg(ProcessingUnit::Register::L) = sum & 0xFF;

    return totalMachineCycles(2);
}

int op_ld_a_bc(ProcessingUnit& cpu, MMU& mmu) // 0x0A
{
    const u16 addr = cpu.get_bc();
    cpu.reg(ProcessingUnit::Register::A) = mmu.read(addr);

    return totalMachineCycles(2);
}

int op_dec_bc(ProcessingUnit& cpu, MMU& mmu) // 0x0B
{
    u16 bc = cpu.get_bc();
    bc--;

    cpu.reg(ProcessingUnit::Register::B) = bc >> 8;
    cpu.reg(ProcessingUnit::Register::C) = bc & 0xFF;

    return totalMachineCycles(2);
}

int op_inc_c(ProcessingUnit& cpu, MMU& mmu) // 0x0C
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::C) + 1;
    cpu.reg(ProcessingUnit::Register::C) = newValue;

    return totalMachineCycles(1);
}

int op_dec_c(ProcessingUnit& cpu, MMU& mmu) // 0x0D
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::C) - 1;
    cpu.reg(ProcessingUnit::Register::C) = newValue;

    return totalMachineCycles(1);
}
int op_ld_c_d8(ProcessingUnit& cpu, MMU& mmu) // 0x0E
{
    const u8 newValue = mmu.read(cpu.inc_pc());
    cpu.reg(ProcessingUnit::Register::C) = newValue;

    return totalMachineCycles(2);
}

int op_rrca(ProcessingUnit& cpu, MMU& mmu) // 0x0F
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);

    const u8 carry = a & 1;
    const u8 result = (a >> 1) | (carry << 7);

    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, false);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry);

    return totalMachineCycles(1);
}









