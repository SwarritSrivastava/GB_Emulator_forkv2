#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/cb_opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_sla_b) // 0xCB20
DUMMY(op_sla_c) // 0xCB21
DUMMY(op_sla_d) // 0xCB22
DUMMY(op_sla_e) // 0xCB23
DUMMY(op_sla_h) // 0xCB24
DUMMY(op_sla_l) // 0xCB25
DUMMY(op_sla_hl) // 0xCB26
DUMMY(op_sla_a) // 0xCB27
DUMMY(op_sra_b) // 0xCB28
DUMMY(op_sra_c) // 0xCB29
DUMMY(op_sra_d) // 0xCB2A
DUMMY(op_sra_e) // 0xCB2B
DUMMY(op_sra_h) // 0xCB2C
DUMMY(op_sra_l) // 0xCB2D
DUMMY(op_sra_hl) // 0xCB2E
DUMMY(op_sra_a) // 0xCB2F
