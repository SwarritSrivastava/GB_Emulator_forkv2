#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_res_0_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB80
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    
    u8 result = value & 0xFE;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_res_0_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB81
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    
    u8 result = value & 0xFE;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_res_0_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB82
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    
    u8 result = value & 0xFE;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_res_0_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB83
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    
    u8 result = value & 0xFE;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_res_0_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB84
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    
    u8 result = value & 0xFE;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_res_0_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB85
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    
    u8 result = value & 0xFE;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_res_0_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB86
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);
    
    u8 result = value & 0xFE;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_res_0_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB87
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    
    u8 result = value & 0xFE;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}

int op_res_1_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB88
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    
    u8 result = value & 0xFD;
    cpu.reg(ProcessingUnit::Register::B) = result;

    return totalMachineCycles(2);
}

int op_res_1_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB89
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    
    u8 result = value & 0xFD;
    cpu.reg(ProcessingUnit::Register::C) = result;

    return totalMachineCycles(2);
}

int op_res_1_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB8A
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    
    u8 result = value & 0xFD;
    cpu.reg(ProcessingUnit::Register::D) = result;

    return totalMachineCycles(2);
}

int op_res_1_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB8B
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    
    u8 result = value & 0xFD;
    cpu.reg(ProcessingUnit::Register::E) = result;

    return totalMachineCycles(2);
}

int op_res_1_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB8C
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    
    u8 result = value & 0xFD;
    cpu.reg(ProcessingUnit::Register::H) = result;

    return totalMachineCycles(2);
}

int op_res_1_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB8D
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    
    u8 result = value & 0xFD;
    cpu.reg(ProcessingUnit::Register::L) = result;

    return totalMachineCycles(2);
}

int op_res_1_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB8E
{
    u16 addr = cpu.get_hl();
    u8 value = mmu.read(addr);
    
    u8 result = value & 0xFD;
    mmu.write(addr, result);

    return totalMachineCycles(4);
}

int op_res_1_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB8F
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    
    u8 result = value & 0xFD;
    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}
