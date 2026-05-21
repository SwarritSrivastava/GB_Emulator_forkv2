#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_ldh_a8_a(ProcessingUnit& cpu, MMU& mmu) // 0xE0
{
    const u8 a8 = mmu.read(cpu.inc_pc());
    const u16 target = 0xFF00 | a8;

    mmu.write(target, cpu.reg(ProcessingUnit::Register::A));

    return totalMachineCycles(3);
}

int op_pop_hl(ProcessingUnit& cpu, MMU& mmu) // 0xE1
{
    const u16 sp = cpu.get_sp();
    const u8 lo = mmu.read(sp);
    const u8 hi = mmu.read(sp + 1);

    cpu.reg(ProcessingUnit::Register::L) = lo;
    cpu.reg(ProcessingUnit::Register::H) = hi;
    cpu.set_sp(sp + 2);

    return totalMachineCycles(3);
}

int op_ld_c_a_e2(ProcessingUnit& cpu, MMU& mmu) // 0xE2
{
    const u16 target = 0xFF00 | cpu.reg(ProcessingUnit::Register::C);
    mmu.write(target, cpu.reg(ProcessingUnit::Register::A));

    return totalMachineCycles(2);
}

int op_illegal_e3(ProcessingUnit& cpu, MMU& mmu) // 0xE3
{
    return totalMachineCycles(1);
}

int op_illegal_e4(ProcessingUnit& cpu, MMU& mmu) // 0xE4
{
    return totalMachineCycles(1);
}

int op_push_hl(ProcessingUnit& cpu, MMU& mmu) // 0xE5
{
    const u8 h = cpu.reg(ProcessingUnit::Register::H);
    const u8 l = cpu.reg(ProcessingUnit::Register::L);
    const u16 sp = cpu.get_sp();

    mmu.write(sp - 1, h);
    mmu.write(sp - 2, l);
    cpu.set_sp(sp - 2);

    return totalMachineCycles(4);
}

int op_and_d8(ProcessingUnit& cpu, MMU& mmu) // 0xE6
{
    const u8 d8 = mmu.read(cpu.inc_pc());
    const u8 result = cpu.reg(ProcessingUnit::Register::A) & d8;
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, true);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(2);
}

int op_rst_20(ProcessingUnit& cpu, MMU& mmu) // 0xE7
{
    const u16 pc = cpu.get_pc();

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), (pc >> 8) & 0xFF);

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), pc & 0xFF);

    cpu.set_pc(0x20);

    return totalMachineCycles(4);
}

int op_add_sp_e8(ProcessingUnit& cpu, MMU& mmu) // 0xE8
{
    const u16 sp = cpu.get_sp();
    const u8 offset_raw = mmu.read(cpu.inc_pc());
    const auto offset = static_cast<int8_t>(offset_raw);
    const u16 result = static_cast<u16>(sp + offset);

    cpu.set_sp(result);

    cpu.setFlag(ProcessingUnit::Flag::Z, false);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((sp & 0x0F) + (offset_raw & 0x0F)) > 0x0F);
    cpu.setFlag(ProcessingUnit::Flag::C, ((sp & 0xFF) + (offset_raw & 0xFF)) > 0xFF);

    return totalMachineCycles(4);
}

int op_jp_hl(ProcessingUnit& cpu, MMU& mmu) // 0xE9
{
    cpu.set_pc(cpu.get_hl());
    return totalMachineCycles(1);
}

int op_ld_a16_a(ProcessingUnit& cpu, MMU& mmu) // 0xEA
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());
    const u16 addr = static_cast<u16>((hi << 8) | lo);

    mmu.write(addr, cpu.reg(ProcessingUnit::Register::A));

    return totalMachineCycles(4);
}

int op_illegal_eb(ProcessingUnit& cpu, MMU& mmu) // 0xEB
{
    return totalMachineCycles(1);
}

int op_illegal_ec(ProcessingUnit& cpu, MMU& mmu) // 0xEC
{
    return totalMachineCycles(1);
}

int op_illegal_ed(ProcessingUnit& cpu, MMU& mmu) // 0xED
{
    return totalMachineCycles(1);
}

int op_xor_d8(ProcessingUnit& cpu, MMU& mmu) // 0xEE
{
    const u8 d8 = mmu.read(cpu.inc_pc());
    const u8 result = cpu.reg(ProcessingUnit::Register::A) ^ d8;
    cpu.reg(ProcessingUnit::Register::A) = result;

    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, false);

    return totalMachineCycles(2);
}

int op_rst_28(ProcessingUnit& cpu, MMU& mmu) // 0xEF
{
    const u16 pc = cpu.get_pc();

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), (pc >> 8) & 0xFF);

    cpu.set_sp(cpu.get_sp() - 1);
    mmu.write(cpu.get_sp(), pc & 0xFF);

    cpu.set_pc(0x28);

    return totalMachineCycles(4);
}
