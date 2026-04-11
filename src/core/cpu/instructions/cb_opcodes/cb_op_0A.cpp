#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

int op_res_4_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBA0
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    
    u8 result = value & 0xEF;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_res_4_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBA1
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    
    u8 result = value & 0xEF;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_res_4_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBA2
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    
    u8 result = value & 0xEF;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_res_4_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBA3
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    
    u8 result = value & 0xEF;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_res_4_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBA4
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    
    u8 result = value & 0xEF;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_res_4_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBA5
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    
    u8 result = value & 0xEF;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_res_4_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBA6
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);
    
    u8 result = value & 0xEF;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_res_4_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBA7
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    
    u8 result = value & 0xEF;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}

int op_res_5_b(ProcessingUnit& cpu, MMU& mmu) // 0xCBA8
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    
    u8 result = value & 0xDF;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_res_5_c(ProcessingUnit& cpu, MMU& mmu) // 0xCBA9
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    
    u8 result = value & 0xDF;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_res_5_d(ProcessingUnit& cpu, MMU& mmu) // 0xCBAA
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    
    u8 result = value & 0xDF;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_res_5_e(ProcessingUnit& cpu, MMU& mmu) // 0xCBAB
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    
    u8 result = value & 0xDF;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_res_5_h(ProcessingUnit& cpu, MMU& mmu) // 0xCBAC
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    
    u8 result = value & 0xDF;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_res_5_l(ProcessingUnit& cpu, MMU& mmu) // 0xCBAD
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    
    u8 result = value & 0xDF;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_res_5_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCBAE
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);
    
    u8 result = value & 0xDF;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_res_5_a(ProcessingUnit& cpu, MMU& mmu) // 0xCBAF
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    
    u8 result = value & 0xDF;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}
