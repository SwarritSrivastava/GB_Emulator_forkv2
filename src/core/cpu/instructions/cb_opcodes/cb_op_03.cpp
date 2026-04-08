#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_swap_b(ProcessingUnit& cpu , MMU& mmu) // 0xCB30
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);

    u8 result = (value >> 4) | (value << 4);

    cpu.reg(ProcessingUnit::Register::B) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(2);
}

int op_swap_c(ProcessingUnit& cpu , MMU& mmu) // 0xCB31
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);

    u8 result = (value >> 4) | (value << 4);

    cpu.reg(ProcessingUnit::Register::C) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(2);
}

int op_swap_d(ProcessingUnit& cpu , MMU& mmu) // 0xCB32
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);

    u8 result = (value >> 4) | (value << 4);

    cpu.reg(ProcessingUnit::Register::D) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(2);
}

int op_swap_e(ProcessingUnit& cpu , MMU& mmu) // 0xCB33
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);

    u8 result = (value >> 4) | (value << 4);
    
    cpu.reg(ProcessingUnit::Register::E) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    
    return totalMachineCycles(2);
}

int op_swap_h(ProcessingUnit& cpu , MMU& mmu) // 0xCB34
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    
    u8 result = (value >> 4) | (value << 4);
    
    cpu.reg(ProcessingUnit::Register::H) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    
    return totalMachineCycles(2);
}

int op_swap_l(ProcessingUnit& cpu , MMU& mmu) // 0xCB35
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    
    u8 result = (value >> 4) | (value << 4);
    
    cpu.reg(ProcessingUnit::Register::L) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    
    return totalMachineCycles(2);
}

int op_swap_hl(ProcessingUnit& cpu , MMU& mmu) // 0xCB36
{
    u16 address = cpu.get_hl();
    
    u8 value = mmu.read(address);
    u8 result = (value >> 4) | (value << 4);
    
    mmu.write(address, result);
    
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    
    return totalMachineCycles(4);
}

int op_swap_a(ProcessingUnit& cpu , MMU& mmu) // 0xCB37
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    
    u8 result = (value >> 4) | (value << 4);
    
    cpu.reg(ProcessingUnit::Register::A) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);
    
    return totalMachineCycles(2);
}

int op_srl_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB38 
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    
    u8 carry = value & 0x01;
    u8 result = value >> 1;
    
    cpu.reg(ProcessingUnit::Register::B) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);
    
    return totalMachineCycles(2);
}

int op_srl_c(ProcessingUnit& cpu, MMU& mmu) // 0xCB39
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    
    u8 carry = value & 0x01;
    u8 result = value >> 1;
    
    cpu.reg(ProcessingUnit::Register::C) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);
    
    return totalMachineCycles(2);
}

int op_srl_d(ProcessingUnit& cpu, MMU& mmu) // 0xCB3A
{
    u8 value = cpu.reg(ProcessingUnit::Register::D);
    
    u8 carry = value & 0x01;
    u8 result = value >> 1;
    
    cpu.reg(ProcessingUnit::Register::D) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);
    
    return totalMachineCycles(2);
}

int op_srl_e(ProcessingUnit& cpu, MMU& mmu) // 0xCB3B
{
    u8 value = cpu.reg(ProcessingUnit::Register::E);
    
    u8 carry = value & 0x01;
    u8 result = value >> 1;
    
    cpu.reg(ProcessingUnit::Register::E) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);
    
    return totalMachineCycles(2);
}

int op_srl_h(ProcessingUnit& cpu, MMU& mmu) // 0xCB3C
{
    u8 value = cpu.reg(ProcessingUnit::Register::H);
    
    u8 carry = value & 0x01;
    u8 result = value >> 1;
    
    cpu.reg(ProcessingUnit::Register::H) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);
    
    return totalMachineCycles(2);
}

int op_srl_l(ProcessingUnit& cpu, MMU& mmu) // 0xCB3D
{
    u8 value = cpu.reg(ProcessingUnit::Register::L);
    
    u8 carry = value & 0x01;
    u8 result = value >> 1;
    
    cpu.reg(ProcessingUnit::Register::L) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);
    
    return totalMachineCycles(2);
}

int op_srl_hl(ProcessingUnit& cpu, MMU& mmu) // 0xCB3E
{
    u16 address = cpu.get_hl();
    
    u8 value = mmu.read(address);
    u8 carry = value & 0x01;
    u8 result = value >> 1;
    
    mmu.write(address, result);
    
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);
    
    return totalMachineCycles(4);
}

int op_srl_a(ProcessingUnit& cpu, MMU& mmu) // 0xCB3F
{
    u8 value = cpu.reg(ProcessingUnit::Register::A);
    
    u8 carry = value & 0x01;
    u8 result = value >> 1;
    
    cpu.reg(ProcessingUnit::Register::A) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, carry == 1);
    
    return totalMachineCycles(2);
}