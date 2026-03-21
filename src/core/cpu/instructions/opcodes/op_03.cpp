#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_jr_nc(ProcessingUnit& cpu, MMU& mmu) // 0x30
{
    cpu.inc_pc();
    const int8_t offset = static_cast<int8_t>(mmu.read(cpu.inc_pc()));

    if (!cpu.get_flag_c()) {
        cpu.set_pc(static_cast<u16>(cpu.get_pc() + offset));
        return totalMachineCycles(3);
    }

    return totalMachineCycles(2);
}
int op_ld_sp_d16(ProcessingUnit& cpu, MMU& mmu) // 0x31
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());
    cpu.set_sp(static_cast<u16>((hi << 8) | lo));

    return totalMachineCycles(3);
}
int op_ld_hld_a(ProcessingUnit& cpu, MMU& mmu) // 0x32
{
    const u16 hl = cpu.get_hl();
    mmu.write(hl, cpu.reg(ProcessingUnit::Register::A));

    const u16 newValue = hl - 1;
    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>((newValue >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(newValue & 0xFF);

    return totalMachineCycles(2);
}
int op_inc_sp(ProcessingUnit& cpu, MMU& mmu) // 0x33
{
    cpu.set_sp(cpu.get_sp() + 1);

    return totalMachineCycles(2);
}
int op_inc_hl_ptr(ProcessingUnit& cpu, MMU& mmu) // 0x34
{
    const u16 addr = cpu.get_hl();
    const u8 oldValue = mmu.read(addr);
    const u8 newValue = oldValue + 1;
    mmu.write(addr, newValue);

    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x0F);

    return totalMachineCycles(3);
}
int op_dec_hl_ptr(ProcessingUnit& cpu, MMU& mmu) // 0x35
{
    const u16 addr = cpu.get_hl();
    const u8 oldValue = mmu.read(addr);
    const u8 newValue = oldValue - 1;
    mmu.write(addr, newValue);

    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x00);

    return totalMachineCycles(3);
}
int op_ld_hl_d8(ProcessingUnit& cpu, MMU& mmu) // 0x36
{
    cpu.inc_pc();
    mmu.write(cpu.get_hl(), mmu.read(cpu.inc_pc()));

    return totalMachineCycles(3);
}
int op_scf(ProcessingUnit& cpu, MMU& mmu) // 0x37
{
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, true);

    return totalMachineCycles(1);
}
int op_jr_c(ProcessingUnit& cpu, MMU& mmu) // 0x38
{
    cpu.inc_pc();
    const int8_t offset = static_cast<int8_t>(mmu.read(cpu.inc_pc()));

    if (cpu.get_flag_c()) {
        cpu.set_pc(static_cast<u16>(cpu.get_pc() + offset));
        return totalMachineCycles(3);
    }

    return totalMachineCycles(2);
}
int op_add_hl_sp(ProcessingUnit& cpu, MMU& mmu) // 0x39
{
    const u16 hl = cpu.get_hl();
    const u16 sp = cpu.get_sp();
    const u32 sum = static_cast<u32>(hl) + sp;

    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>((sum >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(sum & 0xFF);

    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, ((hl & 0x0FFF) + (sp & 0x0FFF)) > 0x0FFF);
    cpu.setFlag(ProcessingUnit::Flag::C, sum > 0xFFFF);

    return totalMachineCycles(2);
}
int op_ld_a_hld(ProcessingUnit& cpu, MMU& mmu) // 0x3A
{
    const u16 hl = cpu.get_hl();
    cpu.reg(ProcessingUnit::Register::A) = mmu.read(hl);

    const u16 newValue = hl - 1;
    cpu.reg(ProcessingUnit::Register::H) = static_cast<u8>((newValue >> 8) & 0xFF);
    cpu.reg(ProcessingUnit::Register::L) = static_cast<u8>(newValue & 0xFF);

    return totalMachineCycles(2);
}
int op_dec_sp(ProcessingUnit& cpu, MMU& mmu) // 0x3B
{
    cpu.set_sp(cpu.get_sp() - 1);

    return totalMachineCycles(2);
}
int op_inc_a(ProcessingUnit& cpu, MMU& mmu) // 0x3C
{
    const u8 oldValue = cpu.reg(ProcessingUnit::Register::A);
    const u8 newValue = oldValue + 1;
    cpu.reg(ProcessingUnit::Register::A) = newValue;

    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x0F);

    return totalMachineCycles(1);
}
int op_dec_a(ProcessingUnit& cpu, MMU& mmu) // 0x3D
{
    const u8 oldValue = cpu.reg(ProcessingUnit::Register::A);
    const u8 newValue = oldValue - 1;
    cpu.reg(ProcessingUnit::Register::A) = newValue;

    cpu.setFlag(ProcessingUnit::Flag::Z, newValue == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, true);
    cpu.setFlag(ProcessingUnit::Flag::H, (oldValue & 0x0F) == 0x00);

    return totalMachineCycles(1);
}
int op_ld_a_d8(ProcessingUnit& cpu, MMU& mmu) // 0x3E
{
    cpu.inc_pc();
    cpu.reg(ProcessingUnit::Register::A) = mmu.read(cpu.inc_pc());

    return totalMachineCycles(2);
}
DUMMY(op_ccf) // 0x3F
