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
DUMMY(op_scf) // 0x37
DUMMY(op_jr_c) // 0x38
DUMMY(op_add_hl_sp) // 0x39
DUMMY(op_ld_a_hld) // 0x3A
DUMMY(op_dec_sp) // 0x3B
DUMMY(op_inc_a) // 0x3C
DUMMY(op_dec_a) // 0x3D
DUMMY(op_ld_a_d8) // 0x3E
DUMMY(op_ccf) // 0x3F
