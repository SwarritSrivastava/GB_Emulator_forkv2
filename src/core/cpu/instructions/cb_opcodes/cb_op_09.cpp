#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

int op_res_2_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB90
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    
    u8 result = value & 0xFB;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_res_2_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB91
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    
    u8 result = value & 0xFB;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_res_2_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB92
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    
    u8 result = value & 0xFB;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_res_2_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB93
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    
    u8 result = value & 0xFB;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_res_2_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB94
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    
    u8 result = value & 0xFB;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_res_2_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB95
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    
    u8 result = value & 0xFB;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_res_2_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB96
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);
    
    u8 result = value & 0xFB;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_res_2_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB97
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    
    u8 result = value & 0xFB;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}

int op_res_3_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB98
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    
    u8 result = value & 0xF7;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_res_3_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB99
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    
    u8 result = value & 0xF7;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_res_3_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB9A
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    
    u8 result = value & 0xF7;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_res_3_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB9B
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    
    u8 result = value & 0xF7;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_res_3_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB9C
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    
    u8 result = value & 0xF7;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_res_3_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB9D
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    
    u8 result = value & 0xF7;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_res_3_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB9E
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);
    
    u8 result = value & 0xF7;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_res_3_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB9F
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    
    u8 result = value & 0xF7;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}
