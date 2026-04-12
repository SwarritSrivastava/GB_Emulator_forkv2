#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_set_2_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBD0
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 result = value | 0x04;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_set_2_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBD1
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 result = value | 0x04;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_set_2_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBD2
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 result = value | 0x04;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_set_2_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBD3
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 result = value | 0x04;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_set_2_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBD4
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 result = value | 0x04;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_set_2_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBD5
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 result = value | 0x04;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_set_2_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBD6
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);

    u8 result = value | 0x04;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_set_2_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBD7
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 result = value | 0x04;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}

int op_set_3_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBD8
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 result = value | 0x08;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_set_3_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBD9
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 result = value | 0x08;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_set_3_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBDA
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 result = value | 0x08;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_set_3_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBDB
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 result = value | 0x08;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_set_3_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBDC
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 result = value | 0x08;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_set_3_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBDD
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 result = value | 0x08;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_set_3_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBDE
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);

    u8 result = value | 0x08;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_set_3_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBDF
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 result = value | 0x08;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}
