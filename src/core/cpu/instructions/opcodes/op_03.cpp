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
DUMMY(op_ld_hld_a) // 0x32
DUMMY(op_inc_sp) // 0x33
DUMMY(op_inc_hl_ptr) // 0x34
DUMMY(op_dec_hl_ptr) // 0x35
DUMMY(op_ld_hl_d8) // 0x36
DUMMY(op_scf) // 0x37
DUMMY(op_jr_c) // 0x38
DUMMY(op_add_hl_sp) // 0x39
DUMMY(op_ld_a_hld) // 0x3A
DUMMY(op_dec_sp) // 0x3B
DUMMY(op_inc_a) // 0x3C
DUMMY(op_dec_a) // 0x3D
DUMMY(op_ld_a_d8) // 0x3E
DUMMY(op_ccf) // 0x3F
