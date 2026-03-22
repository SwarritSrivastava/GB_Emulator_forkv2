#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_ld_d_b(ProcessingUnit& cpu, MMU& mmu) // 0x50
{
    cpu.reg(ProcessingUnit::Register::D) = cpu.reg(ProcessingUnit::Register::B);

    return totalMachineCycles(1);
}

int op_ld_d_c(ProcessingUnit& cpu, MMU& mmu) // 0x51
{
    cpu.reg(ProcessingUnit::Register::D) = cpu.reg(ProcessingUnit::Register::C);

    return totalMachineCycles(1);
}

int op_ld_d_d(ProcessingUnit& cpu, MMU& mmu) // 0x52
{
    cpu.reg(ProcessingUnit::Register::D) = cpu.reg(ProcessingUnit::Register::D);

    return totalMachineCycles(1);
}

int op_ld_d_e(ProcessingUnit& cpu, MMU& mmu) // 0x53
{
    cpu.reg(ProcessingUnit::Register::D) = cpu.reg(ProcessingUnit::Register::E);

    return totalMachineCycles(1);
}

int op_ld_d_h(ProcessingUnit& cpu, MMU& mmu) // 0x54
{
    cpu.reg(ProcessingUnit::Register::D) = cpu.reg(ProcessingUnit::Register::H);

    return totalMachineCycles(1);
}

int op_ld_d_l(ProcessingUnit& cpu, MMU& mmu) // 0x55
{
    cpu.reg(ProcessingUnit::Register::D) = cpu.reg(ProcessingUnit::Register::L);

    return totalMachineCycles(1);
}

int op_ld_d_hl(ProcessingUnit& cpu, MMU& mmu) // 0x56
{
    cpu.reg(ProcessingUnit::Register::D) = mmu.read(cpu.get_hl());

    return totalMachineCycles(2);
}

int op_ld_d_a(ProcessingUnit& cpu, MMU& mmu) // 0x57
{
    cpu.reg(ProcessingUnit::Register::D) = cpu.reg(ProcessingUnit::Register::A);

    return totalMachineCycles(1);
}

int op_ld_e_b(ProcessingUnit& cpu, MMU& mmu) // 0x58
{
    cpu.reg(ProcessingUnit::Register::E) = cpu.reg(ProcessingUnit::Register::B);

    return totalMachineCycles(1);
}

int op_ld_e_c(ProcessingUnit& cpu, MMU& mmu) // 0x59
{
    cpu.reg(ProcessingUnit::Register::E) = cpu.reg(ProcessingUnit::Register::C);

    return totalMachineCycles(1);
}

int op_ld_e_d(ProcessingUnit& cpu, MMU& mmu) // 0x5A
{
    cpu.reg(ProcessingUnit::Register::E) = cpu.reg(ProcessingUnit::Register::D);

    return totalMachineCycles(1);
}

int op_ld_e_e(ProcessingUnit& cpu, MMU& mmu) // 0x5B
{
    cpu.reg(ProcessingUnit::Register::E) = cpu.reg(ProcessingUnit::Register::E);

    return totalMachineCycles(1);
}

int op_ld_e_h(ProcessingUnit& cpu, MMU& mmu) // 0x5C
{
    cpu.reg(ProcessingUnit::Register::E) = cpu.reg(ProcessingUnit::Register::H);

    return totalMachineCycles(1);
}

int op_ld_e_l(ProcessingUnit& cpu, MMU& mmu) // 0x5D
{
    cpu.reg(ProcessingUnit::Register::E) = cpu.reg(ProcessingUnit::Register::L);

    return totalMachineCycles(1);
}

int op_ld_e_hl(ProcessingUnit& cpu, MMU& mmu) // 0x5E
{
    cpu.reg(ProcessingUnit::Register::E) = mmu.read(cpu.get_hl());

    return totalMachineCycles(2);
}

int op_ld_e_a(ProcessingUnit& cpu, MMU& mmu) // 0x5F
{
    cpu.reg(ProcessingUnit::Register::E) = cpu.reg(ProcessingUnit::Register::A);

    return totalMachineCycles(1);
}
