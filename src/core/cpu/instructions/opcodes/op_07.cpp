#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_ld_hl_b(ProcessingUnit& cpu, MMU& mmu) // 0x70
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::B);
    const u16 addr = cpu.get_hl();
    mmu.write(addr, newValue);

    return totalMachineCycles(2);
}

int op_ld_hl_c(ProcessingUnit& cpu, MMU& mmu) // 0x71
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::C);
    const u16 addr = cpu.get_hl();
    mmu.write(addr, newValue);

    return totalMachineCycles(2);
}

int op_ld_hl_d(ProcessingUnit& cpu, MMU& mmu) // 0x72
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::D);
    const u16 addr = cpu.get_hl();
    mmu.write(addr, newValue);

    return totalMachineCycles(2);
}

int op_ld_hl_e(ProcessingUnit& cpu, MMU& mmu) // 0x73
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::E);
    const u16 addr = cpu.get_hl();
    mmu.write(addr, newValue);

    return totalMachineCycles(2);
}

int op_ld_hl_h(ProcessingUnit& cpu, MMU& mmu) // 0x74
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::H);
    const u16 addr = cpu.get_hl();
    mmu.write(addr, newValue);

    return totalMachineCycles(2);
}

int op_ld_hl_l(ProcessingUnit& cpu, MMU& mmu) // 0x75
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::L);
    const u16 addr = cpu.get_hl();
    mmu.write(addr, newValue);

    return totalMachineCycles(2);
}

int op_halt(ProcessingUnit &cpu, MMU &mmu) // 0x76
{
    cpu.setHalt(true);
    return totalMachineCycles(1);
}

int op_ld_hl_a(ProcessingUnit& cpu, MMU& mmu) // 0x77
{
    const u8 newValue = cpu.reg(ProcessingUnit::Register::A);
    const u16 addr = cpu.get_hl();
    mmu.write(addr, newValue);

    return totalMachineCycles(2);
}

int op_ld_a_b(ProcessingUnit& cpu, MMU& mmu) // 0x78
{
    cpu.reg(ProcessingUnit::Register::A) = cpu.reg(ProcessingUnit::Register::B);

    return totalMachineCycles(1);
}

int op_ld_a_c_79(ProcessingUnit& cpu, MMU& mmu) // 0x79
{
    cpu.reg(ProcessingUnit::Register::A) = cpu.reg(ProcessingUnit::Register::C);

    return totalMachineCycles(1);
}

int op_ld_a_d(ProcessingUnit& cpu, MMU& mmu) // 0x7A
{
    cpu.reg(ProcessingUnit::Register::A) = cpu.reg(ProcessingUnit::Register::D);

    return totalMachineCycles(1);
}

int op_ld_a_e(ProcessingUnit& cpu, MMU& mmu) // 0x7B
{
    cpu.reg(ProcessingUnit::Register::A) = cpu.reg(ProcessingUnit::Register::E);

    return totalMachineCycles(1);
}

int op_ld_a_h(ProcessingUnit& cpu, MMU& mmu) // 0x7C
{
    cpu.reg(ProcessingUnit::Register::A) = cpu.reg(ProcessingUnit::Register::H);

    return totalMachineCycles(1);
}

int op_ld_a_l(ProcessingUnit& cpu, MMU& mmu) // 0x7D
{
    cpu.reg(ProcessingUnit::Register::A) = cpu.reg(ProcessingUnit::Register::L);

    return totalMachineCycles(1);
}

int op_ld_a_hl(ProcessingUnit& cpu, MMU& mmu) // 0x7E
{
    cpu.reg(ProcessingUnit::Register::A) = mmu.read(cpu.get_hl());

    return totalMachineCycles(2);
}

int op_ld_a_a(ProcessingUnit& cpu, MMU& mmu) // 0x7F
{
    cpu.reg(ProcessingUnit::Register::A) = cpu.reg(ProcessingUnit::Register::A);

    return totalMachineCycles(1);
}
