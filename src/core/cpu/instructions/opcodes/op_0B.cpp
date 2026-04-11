#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_or_b(ProcessingUnit& cpu, MMU& mmu) // 0xB0
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) | cpu.reg(ProcessingUnit::Register::B);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_or_c(ProcessingUnit& cpu, MMU& mmu) // 0xB1
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) | cpu.reg(ProcessingUnit::Register::C);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_or_d(ProcessingUnit& cpu, MMU& mmu) // 0xB2
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) | cpu.reg(ProcessingUnit::Register::D);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_or_e(ProcessingUnit& cpu, MMU& mmu) // 0xB3
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) | cpu.reg(ProcessingUnit::Register::E);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_or_h(ProcessingUnit& cpu, MMU& mmu) // 0xB4
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) | cpu.reg(ProcessingUnit::Register::H);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_or_l(ProcessingUnit& cpu, MMU& mmu) // 0xB5
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) | cpu.reg(ProcessingUnit::Register::L);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_or_hl(ProcessingUnit& cpu, MMU& mmu) // 0xB6
{
    const u8 value = mmu.read(cpu.get_hl());
    const u8 result = cpu.reg(ProcessingUnit::Register::A) | value;
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(2);
}

int op_or_a(ProcessingUnit& cpu, MMU& mmu) // 0xB7
{
    const u8 result = cpu.reg(ProcessingUnit::Register::A) | cpu.reg(ProcessingUnit::Register::A);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(1);
}

int op_cp_b(ProcessingUnit& cpu, MMU& mmu) // 0xB8
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 b = cpu.reg(ProcessingUnit::Register::B);
    const int result = a - b;
    const int half_carry = (a & 0x0F) - (b & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    return totalMachineCycles(1);
}

int op_cp_c(ProcessingUnit& cpu, MMU& mmu) // 0xB9
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 c = cpu.reg(ProcessingUnit::Register::C);
    const int result = a - c;
    const int half_carry = (a & 0x0F) - (c & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    return totalMachineCycles(1);
}

int op_cp_d(ProcessingUnit& cpu, MMU& mmu) // 0xBA
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 d = cpu.reg(ProcessingUnit::Register::D);
    const int result = a - d;
    const int half_carry = (a & 0x0F) - (d & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    return totalMachineCycles(1);
}

int op_cp_e(ProcessingUnit& cpu, MMU& mmu) // 0xBB
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 e = cpu.reg(ProcessingUnit::Register::E);
    const int result = a - e;
    const int half_carry = (a & 0x0F) - (e & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    return totalMachineCycles(1);
}

int op_cp_h(ProcessingUnit& cpu, MMU& mmu) // 0xBC
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 h = cpu.reg(ProcessingUnit::Register::H);
    const int result = a - h;
    const int half_carry = (a & 0x0F) - (h & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    return totalMachineCycles(1);
}

int op_cp_l(ProcessingUnit& cpu, MMU& mmu) // 0xBD
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 l = cpu.reg(ProcessingUnit::Register::L);
    const int result = a - l;
    const int half_carry = (a & 0x0F) - (l & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    return totalMachineCycles(1);
}

int op_cp_hl(ProcessingUnit& cpu, MMU& mmu) // 0xBE
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 value = mmu.read(cpu.get_hl());
    const int result = a - value;
    const int half_carry = (a & 0x0F) - (value & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    return totalMachineCycles(2);
}

int op_cp_a(ProcessingUnit& cpu, MMU& mmu) // 0xBF
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const int result = a - a;
    const int half_carry = (a & 0x0F) - (a & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    return totalMachineCycles(1);
}
