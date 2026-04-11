#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_add_a_b(ProcessingUnit& cpu, MMU& mmu) // 0x80
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 b = cpu.reg(ProcessingUnit::Register::B);
    const u16 sum = a + b;
    const u8 result = static_cast<u8>(sum & 0xFF);

    cpu.reg(ProcessingUnit::Register::A) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (b & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

    return totalMachineCycles(1);
}

int op_add_a_c(ProcessingUnit& cpu, MMU& mmu) // 0x81
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 c = cpu.reg(ProcessingUnit::Register::C);
    const u16 sum = a + c;

    const u8 result = static_cast<u8>(sum & 0xFF);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (c & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

    return totalMachineCycles(1);
}

int op_add_a_d(ProcessingUnit& cpu, MMU& mmu) // 0x82
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 d = cpu.reg(ProcessingUnit::Register::D);
    const u16 sum = a + d;

    const u8 result = static_cast<u8>(sum & 0xFF);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (d & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

    return totalMachineCycles(1);
}

int op_add_a_e(ProcessingUnit& cpu, MMU& mmu) // 0x83
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 e = cpu.reg(ProcessingUnit::Register::E);
    const u16 sum = a + e;

    const u8 result = static_cast<u8>(sum & 0xFF);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (e & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

    return totalMachineCycles(1);
}

int op_add_a_h(ProcessingUnit& cpu, MMU& mmu) // 0x82
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 h = cpu.reg(ProcessingUnit::Register::H);
    const u16 sum = a + h;

    const u8 result = static_cast<u8>(sum & 0xFF);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (h & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

    return totalMachineCycles(1);
}

int op_add_a_l(ProcessingUnit& cpu, MMU& mmu) // 0x85
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 l = cpu.reg(ProcessingUnit::Register::L);
    const u16 sum = a + l;

    const u8 result = static_cast<u8>(sum & 0xFF);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (l & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

    return totalMachineCycles(1);
}

int op_add_a_hl(ProcessingUnit& cpu, MMU& mmu) // 0x86
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u16 hl = cpu.get_hl();
    const u8 value = mmu.read(hl);
    const u16 sum = a + value;

    const u8 result = static_cast<u8>(sum & 0xFF);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (value & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

    return totalMachineCycles(2);
}

int op_add_a_a(ProcessingUnit& cpu, MMU& mmu) // 0x87
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u16 sum = a + a;

    const u8 result = static_cast<u8>(sum & 0xFF);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (a & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

    return totalMachineCycles(1);
}

int op_adc_a_b(ProcessingUnit& cpu, MMU& mmu) // 0x88
{
	const u8 a = cpu.reg(ProcessingUnit::Register::A);
	const u8 b = cpu.reg(ProcessingUnit::Register::B);
	const bool carry = cpu.get_flag_c();
	const u16 sum = a + b + carry;

	const u8 result = static_cast<u8>(sum & 0xFF);
	cpu.reg(ProcessingUnit::Register::A) = result;

	cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
	cpu.setFlag(ProcessingUnit::Flag::N, false);
	cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (b & 0x0F) + carry) > 0x0F);
	cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

	return totalMachineCycles(1);
}

int op_adc_a_c(ProcessingUnit& cpu, MMU& mmu) // 0x89
{
	const u8 a = cpu.reg(ProcessingUnit::Register::A);
	const u8 b = cpu.reg(ProcessingUnit::Register::C);
	const bool carry = cpu.get_flag_c();
	const u16 sum = a + b + carry;

	const u8 result = static_cast<u8>(sum & 0xFF);
	cpu.reg(ProcessingUnit::Register::A) = result;

	cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
	cpu.setFlag(ProcessingUnit::Flag::N, false);
	cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (b & 0x0F) + carry) > 0x0F);
	cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

	return totalMachineCycles(1);
}

int op_adc_a_d(ProcessingUnit& cpu, MMU& mmu) // 0x8A
{
	const u8 a = cpu.reg(ProcessingUnit::Register::A);
	const u8 b = cpu.reg(ProcessingUnit::Register::D);
	const bool carry = cpu.get_flag_c();
	const u16 sum = a + b + carry;

	const u8 result = static_cast<u8>(sum & 0xFF);
	cpu.reg(ProcessingUnit::Register::A) = result;

	cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
	cpu.setFlag(ProcessingUnit::Flag::N, false);
	cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (b & 0x0F) + carry) > 0x0F);
	cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

	return totalMachineCycles(1);
}

int op_adc_a_e(ProcessingUnit& cpu, MMU& mmu) // 0x8B
{
	const u8 a = cpu.reg(ProcessingUnit::Register::A);
	const u8 b = cpu.reg(ProcessingUnit::Register::E);
	const bool carry = cpu.get_flag_c();
	const u16 sum = a + b + carry;

	const u8 result = static_cast<u8>(sum & 0xFF);
	cpu.reg(ProcessingUnit::Register::A) = result;

	cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
	cpu.setFlag(ProcessingUnit::Flag::N, false);
	cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (b & 0x0F) + carry) > 0x0F);
	cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

	return totalMachineCycles(1);
}

int op_adc_a_h(ProcessingUnit& cpu, MMU& mmu) // 0x8C
{
	const u8 a = cpu.reg(ProcessingUnit::Register::A);
	const u8 b = cpu.reg(ProcessingUnit::Register::H);
	const bool carry = cpu.get_flag_c();
	const u16 sum = a + b + carry;

	const u8 result = static_cast<u8>(sum & 0xFF);
	cpu.reg(ProcessingUnit::Register::A) = result;

	cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
	cpu.setFlag(ProcessingUnit::Flag::N, false);
	cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (b & 0x0F) + carry) > 0x0F);
	cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

	return totalMachineCycles(1);
}

int op_adc_a_l(ProcessingUnit& cpu, MMU& mmu) // 0x8D
{
	const u8 a = cpu.reg(ProcessingUnit::Register::A);
	const u8 b = cpu.reg(ProcessingUnit::Register::L);
	const bool carry = cpu.get_flag_c();
	const u16 sum = a + b + carry;

	const u8 result = static_cast<u8>(sum & 0xFF);
	cpu.reg(ProcessingUnit::Register::A) = result;

	cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
	cpu.setFlag(ProcessingUnit::Flag::N, false);
	cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (b & 0x0F) + carry) > 0x0F);
	cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

	return totalMachineCycles(1);
}

int op_adc_a_hl(ProcessingUnit& cpu, MMU& mmu) // 0x8E
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u16 hl = cpu.get_hl();
    const u8 value = mmu.read(hl);
	const bool carry = cpu.get_flag_c();
    const u16 sum = a + value + carry;

    const u8 result = static_cast<u8>(sum & 0xFF);
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (value & 0x0F) + carry) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

    return totalMachineCycles(2);
}

int op_adc_a_a(ProcessingUnit& cpu, MMU& mmu) // 0x8F
{
	const u8 a = cpu.reg(ProcessingUnit::Register::A);
	const bool carry = cpu.get_flag_c();
	const u16 sum = 2 * a + carry;

	const u8 result = static_cast<u8>(sum & 0xFF);
	cpu.reg(ProcessingUnit::Register::A) = result;

	cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
	cpu.setFlag(ProcessingUnit::Flag::N, false);
	cpu.setFlag(ProcessingUnit::Flag::H, ((a & 0x0F) + (a & 0x0F) + carry) > 0x0F);
	cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFF);

	return totalMachineCycles(1);
}