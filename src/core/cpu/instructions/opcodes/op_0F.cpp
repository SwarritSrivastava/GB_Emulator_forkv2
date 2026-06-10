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
    throw std::runtime_error("Unimplemented opcode at PC: 0x" + std::to_string(cpu.last_pc));
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
    const u16 sp = cpu.get_sp();
    const u8 offset_raw = mmu.read(cpu.inc_pc());
    const auto offset = static_cast<int8_t>(offset_raw);
    const u16 result = static_cast<u16>(sp + offset);

    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>((result >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(result & 0xFF);

    cpu.setFlag(ProcessingUnit::Flag::Z, false);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((sp & 0x0F) + (offset_raw & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, ((sp & 0xFF) + (offset_raw & 0xFF)) > 0xFF);

    return totalMachineCycles(3);
}

int op_ld_sp_hl(ProcessingUnit& cpu, MMU& mmu) // 0xF9
{
    cpu.set_sp(cpu.get_hl());
    return totalMachineCycles(2);
}

int op_ld_a_a16(ProcessingUnit& cpu, MMU& mmu) // 0xFA
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());
    const u16 addr = static_cast<u16>((hi << 8) | lo);

    cpu.reg(ProcessingUnit::Register::A) = mmu.read(addr);

    return totalMachineCycles(4);
}

int op_ei(ProcessingUnit& cpu, MMU& mmu) // 0xFB
{
    cpu.setIME(true);
    return totalMachineCycles(1);
}
int op_illegal_fc(ProcessingUnit& cpu, MMU& mmu) // 0xFC 
{
    throw std::runtime_error("Unimplemented opcode at PC: 0x" + std::to_string(cpu.last_pc));
}

int op_illegal_fd(ProcessingUnit& cpu, MMU& mmu) // 0xFD
{
    throw std::runtime_error("Unimplemented opcode at PC: 0x" + std::to_string(cpu.last_pc));
}

int op_cp_d8(ProcessingUnit& cpu, MMU& mmu) // 0xFE
{
    const u8 a = cpu.reg(ProcessingUnit::Register::A);
    const u8 d8 = mmu.read(cpu.inc_pc());
    const int result = a - d8;
    const int half_carry = (a & 0x0F) - (d8 & 0x0F);

    cpu.setFlag(ProcessingUnit::Flag::Z, (result & 0xFF) == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, half_carry < 0);
    cpu.setFlag(ProcessingUnit::Flag::C, result < 0);

    return totalMachineCycles(2);
}

int op_rst_38(ProcessingUnit& cpu, MMU& mmu) // 0xFF
{
    const u16 pc = cpu.get_pc();

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), (pc >> 8) & 0xFF);

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), pc & 0xFF);

    cpu.set_pc(0x38);

    return totalMachineCycles(4);
}
