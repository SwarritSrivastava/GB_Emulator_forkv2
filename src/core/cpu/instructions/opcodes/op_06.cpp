#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_ld_h_b(ProcessingUnit& cpu, MMU& mmu) // 0x60
{
    cpu.reg(ProcessingUnit::Register::H) = cpu.reg(ProcessingUnit::Register::B);

    return totalMachineCycles(1);
}

int op_ld_h_c(ProcessingUnit& cpu, MMU& mmu) // 0x61
{
    cpu.reg(ProcessingUnit::Register::H) = cpu.reg(ProcessingUnit::Register::C);

    return totalMachineCycles(1);
}

int op_ld_h_d(ProcessingUnit& cpu, MMU& mmu) // 0x62
{
    cpu.reg(ProcessingUnit::Register::H) = cpu.reg(ProcessingUnit::Register::D);

    return totalMachineCycles(1);
}

int op_ld_h_e(ProcessingUnit& cpu, MMU& mmu) // 0x63
{
    cpu.reg(ProcessingUnit::Register::H) = cpu.reg(ProcessingUnit::Register::E);

    return totalMachineCycles(1);
}

int op_ld_h_h(ProcessingUnit& cpu, MMU& mmu) // 0x64
{
    cpu.reg(ProcessingUnit::Register::H) = cpu.reg(ProcessingUnit::Register::H);

    return totalMachineCycles(1);
}

int op_ld_h_l(ProcessingUnit& cpu, MMU& mmu) // 0x65
{
    cpu.reg(ProcessingUnit::Register::H) = cpu.reg(ProcessingUnit::Register::L);

    return totalMachineCycles(1);
}

int op_ld_h_hl(ProcessingUnit& cpu, MMU& mmu) // 0x66
{
    cpu.reg(ProcessingUnit::Register::H) = mmu.read(cpu.get_hl());

    return totalMachineCycles(2);
}

int op_ld_h_a(ProcessingUnit& cpu, MMU& mmu) // 0x67
{
    cpu.reg(ProcessingUnit::Register::H) = cpu.reg(ProcessingUnit::Register::A);

    return totalMachineCycles(1);
}

int op_ld_l_b(ProcessingUnit& cpu, MMU& mmu) // 0x68
{
    cpu.reg(ProcessingUnit::Register::L) = cpu.reg(ProcessingUnit::Register::B);

    return totalMachineCycles(1);
}

int op_ld_l_c(ProcessingUnit& cpu, MMU& mmu) // 0x69
{
    cpu.reg(ProcessingUnit::Register::L) = cpu.reg(ProcessingUnit::Register::C);

    return totalMachineCycles(1);
}

int op_ld_l_d(ProcessingUnit& cpu, MMU& mmu) // 0x6A
{
    cpu.reg(ProcessingUnit::Register::L) = cpu.reg(ProcessingUnit::Register::D);

    return totalMachineCycles(1);
}

int op_ld_l_e(ProcessingUnit& cpu, MMU& mmu) // 0x6B
{
    cpu.reg(ProcessingUnit::Register::L) = cpu.reg(ProcessingUnit::Register::E);

    return totalMachineCycles(1);
}

int op_ld_l_h(ProcessingUnit& cpu, MMU& mmu) // 0x6C
{
    cpu.reg(ProcessingUnit::Register::L) = cpu.reg(ProcessingUnit::Register::H);

    return totalMachineCycles(1);
}

int op_ld_l_l(ProcessingUnit& cpu, MMU& mmu) // 0x6D
{
    cpu.reg(ProcessingUnit::Register::L) = cpu.reg(ProcessingUnit::Register::L);

    return totalMachineCycles(1);
}

int op_ld_l_hl(ProcessingUnit& cpu, MMU& mmu) // 0x6E
{
    cpu.reg(ProcessingUnit::Register::L) = mmu.read(cpu.get_hl());

    return totalMachineCycles(2);
}

int op_ld_l_a(ProcessingUnit& cpu, MMU& mmu) // 0x6F
{
    cpu.reg(ProcessingUnit::Register::L) = cpu.reg(ProcessingUnit::Register::A);

    return totalMachineCycles(1);
}
