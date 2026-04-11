#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_sub_b(ProcessingUnit &cpu, MMU &mmu) // 0x90
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 b = cpu.reg(ProcessingUnit::Register::B);
    const int result = a - b;
    const int half_carry = (a & 0x0F) - (b & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sub_c(ProcessingUnit &cpu, MMU &mmu) // 0x91
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 c = cpu.reg(ProcessingUnit::Register::C);
    const int result = a - c;
    const int half_carry = (a & 0x0F) - (c & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sub_d(ProcessingUnit &cpu, MMU &mmu) // 0x92
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 d = cpu.reg(ProcessingUnit::Register::D);
    const int result = a - d;
    const int half_carry = (a & 0x0F) - (d & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sub_e(ProcessingUnit &cpu, MMU &mmu) // 0x93
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 e = cpu.reg(ProcessingUnit::Register::E);
    const int result = a - e;
    const int half_carry = (a & 0x0F) - (e & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sub_h(ProcessingUnit &cpu, MMU &mmu) // 0x94
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 h = cpu.reg(ProcessingUnit::Register::H);
    const int result = a - h;
    const int half_carry = (a & 0x0F) - (h & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sub_l(ProcessingUnit &cpu, MMU &mmu) // 0x95
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 l = cpu.reg(ProcessingUnit::Register::L);
    const int result = a - l;
    const int half_carry = (a & 0x0F) - (l & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sub_hl(ProcessingUnit &cpu, MMU &mmu) // 0x96
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 value = mmu.read(cpu.get_hl());
    const int result = a - value;
    const int half_carry = (a & 0x0F) - (value & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(2);
}

int op_sub_a(ProcessingUnit &cpu, MMU &mmu) // 0x97
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const int result = a - a;
    const int half_carry = (a & 0x0F) - (a & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sbc_a_b(ProcessingUnit &cpu, MMU &mmu) // 0x98
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 b = cpu.reg(ProcessingUnit::Register::B);
    const u8 carry = cpu.get_flag_c() ? 1 : 0;
    const int result = a - b - carry;
    const int half_carry = (a & 0x0F) - (b & 0x0F) - carry;

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sbc_a_c(ProcessingUnit &cpu, MMU &mmu) // 0x99
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 c = cpu.reg(ProcessingUnit::Register::C);
    const u8 carry = cpu.get_flag_c() ? 1 : 0;
    const int result = a - c - carry;
    const int half_carry = (a & 0x0F) - (c & 0x0F) - carry;

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sbc_a_d(ProcessingUnit &cpu, MMU &mmu) // 0x9A
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 d = cpu.reg(ProcessingUnit::Register::D);
    const u8 carry = cpu.get_flag_c() ? 1 : 0;
    const int result = a - d - carry;
    const int half_carry = (a & 0x0F) - (d & 0x0F) - carry;

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sbc_a_e(ProcessingUnit &cpu, MMU &mmu) // 0x9B
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 e = cpu.reg(ProcessingUnit::Register::E);
    const u8 carry = cpu.get_flag_c() ? 1 : 0;
    const int result = a - e - carry;
    const int half_carry = (a & 0x0F) - (e & 0x0F) - carry;

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sbc_a_h(ProcessingUnit &cpu, MMU &mmu) // 0x9C
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 h = cpu.reg(ProcessingUnit::Register::H);
    const u8 carry = cpu.get_flag_c() ? 1 : 0;
    const int result = a - h - carry;
    const int half_carry = (a & 0x0F) - (h & 0x0F) - carry;

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sbc_a_l(ProcessingUnit &cpu, MMU &mmu) // 0x9D
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 l = cpu.reg(ProcessingUnit::Register::L);
    const u8 carry = cpu.get_flag_c() ? 1 : 0;
    const int result = a - l - carry;
    const int half_carry = (a & 0x0F) - (l & 0x0F) - carry;

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}

int op_sbc_a_hl(ProcessingUnit &cpu, MMU &mmu) // 0x9E
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 value = mmu.read(cpu.get_hl());
    const u8 carry = cpu.get_flag_c() ? 1 : 0;
    const int result = a - value - carry;
    const int half_carry = (a & 0x0F) - (value & 0x0F) - carry;

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(2);
}

int op_sbc_a_a(ProcessingUnit &cpu, MMU &mmu) // 0x9F
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 carry = cpu.get_flag_c() ? 1 : 0;
    const int result = a - a - carry;
    const int half_carry = (a & 0x0F) - (a & 0x0F) - carry;

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    cpu.reg(ProcessingUnit::Register::A) = static_cast<u8>(result);
    return totalMachineCycles(1);
}
