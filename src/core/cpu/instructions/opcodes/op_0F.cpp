#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_ldh_a_a8(ProcessingUnit& cpu, MMU& mmu) // 0xF0
{
    u8 a8 = mmu.read(cpu.inc_pc());

    u16 target = 0xFF00 | a8;
    cpu.reg(ProcessingUnit::Register::A) = mmu.read(target);

    return totalMachineCycles(3);
}

int op_pop_af(ProcessingUnit& cpu, MMU& mmu) // 0xF1 
{
    u8 lb = mmu.read(cpu.get_sp());
    u8 masked_lb = lb & 0xF0;

    cpu.reg(ProcessingUnit::Register::F) = masked_lb;
    cpu.set_sp(cpu.get_sp() + 1);

    u8 hb = mmu.read(cpu.get_sp());
    cpu.reg(ProcessingUnit::Register::A) = hb;
    cpu.set_sp(cpu.get_sp() + 1);

    return totalMachineCycles(3);
}

int op_ld_a_c_f2(ProcessingUnit& cpu, MMU& mmu) // 0xF2 
{
    u8 value = cpu.reg(ProcessingUnit::Register::C);
    u16 target = 0xFF00 | value;

    u8 result = mmu.read(target);

    cpu.reg(ProcessingUnit::Register::A) = result;

    return totalMachineCycles(2);
}

int op_di(ProcessingUnit& cpu, MMU& mmu) // 0xF3
{
    cpu.setIME(false);
    return totalMachineCycles(1);
}

int op_illegal_f4(ProcessingUnit& cpu, MMU& mmu) // 0xF4
{
    return totalMachineCycles(1);
}

int op_push_af(ProcessingUnit& cpu, MMU& mmu) // 0xF5
{
    u8 value_a = cpu.reg(ProcessingUnit::Register::A);
    u8 value_f = cpu.reg(ProcessingUnit::Register::F);

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), value_a);

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), value_f);

    return totalMachineCycles(4);
}

int op_or_d8(ProcessingUnit& cpu, MMU& mmu) // 0xF6
{
    u8 value = mmu.read(cpu.inc_pc());

    u8 result = cpu.reg(ProcessingUnit::Register::A) | value;
    cpu.reg(ProcessingUnit::Register::A) = result;
    
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(2);
}

int op_rst_30(ProcessingUnit& cpu, MMU& mmu) // 0xF7
{
    u16 pc = cpu.get_pc();

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), (pc >> 8) & 0xFF);

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), pc & 0xFF);

    cpu.set_pc(0x30);

    return totalMachineCycles(4);
}

int op_ld_hl_sp_e8(ProcessingUnit& cpu, MMU& mmu) // 0xF8
{
    u8 operand = mmu.read(cpu.inc_pc());
    int8_t e8 = static_cast<int8_t>(operand);
    u16 sp = cpu.get_sp();

    cpu.setFlag(ProcessingUnit::Flag::Z, false);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, (sp & 0x0F) + (operand & 0x0F) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, (sp & 0x00FF) + (operand & 0x00FF) > 0x00FF);

    u16 result = sp + e8;
    cpu.reg(ProcessingUnit::Register::H) = (result >> 8) & 0xFF;
    cpu.reg(ProcessingUnit::Register::L) = result & 0xFF;

    return totalMachineCycles(3);
}
DUMMY(op_ld_sp_hl) // 0xF9
DUMMY(op_ld_a_a16) // 0xFA
DUMMY(op_ei) // 0xFB
int op_illegal_fc(ProcessingUnit& cpu, MMU& mmu) // 0xFC 
{
    return totalMachineCycles(1);
}
int op_illegal_fd(ProcessingUnit& cpu, MMU& mmu) // 0xFD
{
    return totalMachineCycles(1);
}
DUMMY(op_cp_d8) // 0xFE
DUMMY(op_rst_38) // 0xFF
