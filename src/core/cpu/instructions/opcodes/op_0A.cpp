#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_and_b(ProcessingUnit& cpu, MMU& mmu) // 0xA0
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) & cpu.reg(ProcessingUnit::Register::B);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_and_c(ProcessingUnit& cpu, MMU& mmu) // 0xA1
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) & cpu.reg(ProcessingUnit::Register::C);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_and_d(ProcessingUnit& cpu, MMU& mmu) // 0xA2
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) & cpu.reg(ProcessingUnit::Register::D);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_and_e(ProcessingUnit& cpu, MMU& mmu) // 0xA3
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) & cpu.reg(ProcessingUnit::Register::E);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_and_h(ProcessingUnit& cpu, MMU& mmu) // 0xA4
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) & cpu.reg(ProcessingUnit::Register::H);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_and_l(ProcessingUnit& cpu, MMU& mmu) // 0xA5
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) & cpu.reg(ProcessingUnit::Register::L);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_and_hl(ProcessingUnit& cpu, MMU& mmu) // 0xA6
{
    const u8 value = mmu.read(cpu.get_hl());
    const u8 result = cpu.reg(ProcessingUnit::Register::A) & value;
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(2);
}

int op_and_a(ProcessingUnit& cpu, MMU& mmu) // 0xA7
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) & cpu.reg(ProcessingUnit::Register::A);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_xor_b(ProcessingUnit& cpu, MMU& mmu) // 0xA8
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) ^ cpu.reg(ProcessingUnit::Register::B);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_xor_c(ProcessingUnit& cpu, MMU& mmu) // 0xA9
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) ^ cpu.reg(ProcessingUnit::Register::C);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_xor_d(ProcessingUnit& cpu, MMU& mmu) // 0xAA
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) ^ cpu.reg(ProcessingUnit::Register::D);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_xor_e(ProcessingUnit& cpu, MMU& mmu) // 0xAB
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) ^ cpu.reg(ProcessingUnit::Register::E);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_xor_h(ProcessingUnit& cpu, MMU& mmu) // 0xAC
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) ^ cpu.reg(ProcessingUnit::Register::H);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_xor_l(ProcessingUnit& cpu, MMU& mmu) // 0xAD
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) ^ cpu.reg(ProcessingUnit::Register::L);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_xor_hl(ProcessingUnit& cpu, MMU& mmu) // 0xAE
{
    const u8 value = mmu.read(cpu.get_hl());
    const u8 result = cpu.reg(ProcessingUnit::Register::A) ^ value;
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(2);
}

int op_xor_a(ProcessingUnit& cpu, MMU& mmu) // 0xAF
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) ^ cpu.reg(ProcessingUnit::Register::A);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}
