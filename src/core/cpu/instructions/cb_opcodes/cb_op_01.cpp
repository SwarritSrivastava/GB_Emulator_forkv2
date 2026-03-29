#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_rl_b(ProcessingUnit& cpu, MMU& mmu) // 0xCB10 
{
    u8 value = cpu.reg(ProcessingUnit::Register::B);
    u8 old_carry = cpu.get_flag_c() ? 1 : 0;
    u8 new_carry = (value >> 7) & 1;

    u8 result = (value << 1) | old_carry;

    cpu.reg(ProcessingUnit::Register::B) = result;
    cpu.setFlag(ProcessingUnit::Flag::Z, result == 0);
    cpu.setFlag(ProcessingUnit::Flag::N, false);
    cpu.setFlag(ProcessingUnit::Flag::H, false);
    cpu.setFlag(ProcessingUnit::Flag::C, new_carry == 1);

    return totalMachineCycles(2);
}
DUMMY(op_rl_c) // 0xCB11
DUMMY(op_rl_d) // 0xCB12
DUMMY(op_rl_e) // 0xCB13
DUMMY(op_rl_h) // 0xCB14
DUMMY(op_rl_l) // 0xCB15
DUMMY(op_rl_hl) // 0xCB16
DUMMY(op_rl_a) // 0xCB17
DUMMY(op_rr_b) // 0xCB18
DUMMY(op_rr_c) // 0xCB19
DUMMY(op_rr_d) // 0xCB1A
DUMMY(op_rr_e) // 0xCB1B
DUMMY(op_rr_h) // 0xCB1C
DUMMY(op_rr_l) // 0xCB1D
DUMMY(op_rr_hl) // 0xCB1E
DUMMY(op_rr_a) // 0xCB1F
