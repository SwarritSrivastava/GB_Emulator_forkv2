#include "../../../../../include/ProcessingUnit.hpp"
#include "../../../../../include/opcodes.hpp"
#include "../../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) ((n) * machine_cycles)

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

DUMMY(op_add_a_b) // 0x80
DUMMY(op_add_a_c) // 0x81
DUMMY(op_add_a_d) // 0x82
DUMMY(op_add_a_e) // 0x83
DUMMY(op_add_a_h) // 0x84
DUMMY(op_add_a_l) // 0x85
DUMMY(op_add_a_hl) // 0x86
DUMMY(op_add_a_a) // 0x87
DUMMY(op_adc_a_b) // 0x88
DUMMY(op_adc_a_c) // 0x89
DUMMY(op_adc_a_d) // 0x8A
DUMMY(op_adc_a_e) // 0x8B
DUMMY(op_adc_a_h) // 0x8C
DUMMY(op_adc_a_l) // 0x8D
DUMMY(op_adc_a_hl) // 0x8E
DUMMY(op_adc_a_a) // 0x8F