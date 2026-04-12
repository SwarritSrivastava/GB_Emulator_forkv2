#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_set_4_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBE0
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 result = value | 0x10;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_set_4_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBE1
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 result = value | 0x10;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_set_4_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBE2
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 result = value | 0x10;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_set_4_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBE3
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 result = value | 0x10;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_set_4_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBE4
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 result = value | 0x10;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_set_4_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBE5
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 result = value | 0x10;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_set_4_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBE6
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);

    u8 result = value | 0x10;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_set_4_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBE7
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 result = value | 0x10;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}

int op_set_5_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBE8
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 result = value | 0x20;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_set_5_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBE9
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 result = value | 0x20;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_set_5_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBEA
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 result = value | 0x20;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_set_5_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBEB
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 result = value | 0x20;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_set_5_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBEC
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 result = value | 0x20;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_set_5_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBED
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 result = value | 0x20;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_set_5_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBEE
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);

    u8 result = value | 0x20;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_set_5_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBEF
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 result = value | 0x20;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}
