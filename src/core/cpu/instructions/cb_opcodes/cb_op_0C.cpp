#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_set_0_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBC0
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 result = value | 0x01;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_set_0_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBC1
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 result = value | 0x01;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_set_0_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBC2
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 result = value | 0x01;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_set_0_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBC3
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 result = value | 0x01;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_set_0_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBC4
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 result = value | 0x01;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_set_0_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBC5
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 result = value | 0x01;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_set_0_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBC6
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);

    u8 result = value | 0x01;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_set_0_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBC7
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 result = value | 0x01;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}

int op_set_1_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBC8
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 result = value | 0x02;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_set_1_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBC9
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 result = value | 0x02;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_set_1_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBCA
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 result = value | 0x02;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_set_1_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBCB
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 result = value | 0x02;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_set_1_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBCC
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);

    u8 result = value | 0x02;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_set_1_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBCD
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);

    u8 result = value | 0x02;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_set_1_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBCE
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);

    u8 result = value | 0x02;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_set_1_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBCF
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);

    u8 result = value | 0x02;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}
