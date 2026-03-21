#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_ld_b_b(ProcessingUnit& cpu, MMU& mmu) // 0x40
{
    cpu.reg(ProcessingUnit::Register::B) = cpu.reg(ProcessingUnit::Register::B);

    return totalMachineCycles(1);
}
int op_ld_b_c(ProcessingUnit& cpu, MMU& mmu) // 0x41
{
    cpu.reg(ProcessingUnit::Register::B) = cpu.reg(ProcessingUnit::Register::C);

    return totalMachineCycles(1);
}
int op_ld_b_d(ProcessingUnit& cpu, MMU& mmu) // 0x42
{
    cpu.reg(ProcessingUnit::Register::B) = cpu.reg(ProcessingUnit::Register::D);

    return totalMachineCycles(1);
}
int op_ld_b_e(ProcessingUnit& cpu, MMU& mmu) // 0x43
{
    cpu.reg(ProcessingUnit::Register::B) = cpu.reg(ProcessingUnit::Register::E);

    return totalMachineCycles(1);
}
int op_ld_b_h(ProcessingUnit& cpu, MMU& mmu) // 0x44
{
    cpu.reg(ProcessingUnit::Register::B) = cpu.reg(ProcessingUnit::Register::H);

    return totalMachineCycles(1);
}
int op_ld_b_l(ProcessingUnit& cpu, MMU& mmu) // 0x45
{
    cpu.reg(ProcessingUnit::Register::B) = cpu.reg(ProcessingUnit::Register::L);

    return totalMachineCycles(1);
}
int op_ld_b_hl(ProcessingUnit& cpu, MMU& mmu) // 0x46
{
    cpu.reg(ProcessingUnit::Register::B) = mmu.read(cpu.get_hl());

    return totalMachineCycles(2);
}
DUMMY(op_ld_b_a) // 0x47
DUMMY(op_ld_c_b) // 0x48
DUMMY(op_ld_c_c) // 0x49
DUMMY(op_ld_c_d) // 0x4A
DUMMY(op_ld_c_e) // 0x4B
DUMMY(op_ld_c_h) // 0x4C
DUMMY(op_ld_c_l) // 0x4D
DUMMY(op_ld_c_hl) // 0x4E
DUMMY(op_ld_c_a_4f) // 0x4F
