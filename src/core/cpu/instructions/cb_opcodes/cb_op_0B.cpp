#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_res_6_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBB0
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    
    u8 result = value & 0xBF;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_res_6_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBB1
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    
    u8 result = value & 0xBF;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_res_6_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBB2
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    
    u8 result = value & 0xBF;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_res_6_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBB3
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    
    u8 result = value & 0xBF;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_res_6_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBB4
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    
    u8 result = value & 0xBF;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_res_6_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBB5
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    
    u8 result = value & 0xBF;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_res_6_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBB6
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);
    
    u8 result = value & 0xBF;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_res_6_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBB7
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    
    u8 result = value & 0xBF;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}

int op_res_7_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBB8
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    
    u8 result = value & 0x7F;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_res_7_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBB9
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    
    u8 result = value & 0x7F;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_res_7_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBBA
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    
    u8 result = value & 0x7F;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_res_7_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBBB
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    
    u8 result = value & 0x7F;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_res_7_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBBC
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    
    u8 result = value & 0x7F;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_res_7_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBBD
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    
    u8 result = value & 0x7F;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_res_7_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBBE
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);
    
    u8 result = value & 0x7F;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_res_7_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBBF
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    
    u8 result = value & 0x7F;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}
