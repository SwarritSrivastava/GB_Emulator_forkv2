#include "../../../../include/ProcessingUnit.hpp"
#include "../../../../include/opcodes.hpp"
#include "../../../../include/mmu.hpp"

constexpr int machine_cycles = 4;
#define totalMachineCycles(n) n*machine_cycles

#define DUMMY(name) int name(ProcessingUnit&, MMU&) { return totalMachineCycles(1); }

int op_nop(ProcessingUnit &cpu, MMU &mmu)  // 0x00
{
    return totalMachineCycles(1);
}

int op_ld_bc_d16(ProcessingUnit &cpu, MMU &mmu) // 0x01
{
    const u8 lo = mmu.read(cpu.inc_pc());
    const u8 hi = mmu.read(cpu.inc_pc());

    cpu.reg(ProcessingUnit::Register::B) = hi;
    cpu.reg(ProcessingUnit::Register::C) = lo;

    return totalMachineCycles(3);
}

int op_halt(ProcessingUnit &cpu, MMU &mmu) // 0x76
{
    cpu.setHalt(true);
    return totalMachineCycles(1);
}

DUMMY(op_ld_bc_a) // 0x02
DUMMY(op_inc_bc) // 0x03
DUMMY(op_inc_b) // 0x04
DUMMY(op_dec_b) // 0x05
DUMMY(op_ld_b_d8) // 0x06
DUMMY(op_rlca) // 0x07
DUMMY(op_ld_a16_sp) // 0x08
DUMMY(op_add_hl_bc) // 0x09
DUMMY(op_ld_a_bc) // 0x0A
DUMMY(op_dec_bc) // 0x0B
DUMMY(op_inc_c) // 0x0C
DUMMY(op_dec_c) // 0x0D
DUMMY(op_ld_c_d8) // 0x0E
DUMMY(op_rrca) // 0x0F
DUMMY(op_stop) // 0x10
DUMMY(op_ld_de_d16) // 0x11
DUMMY(op_ld_de_a) // 0x12
DUMMY(op_inc_de) // 0x13
DUMMY(op_inc_d) // 0x14
DUMMY(op_dec_d) // 0x15
DUMMY(op_ld_d_d8) // 0x16
DUMMY(op_rla) // 0x17
DUMMY(op_jr_r8) // 0x18
DUMMY(op_add_hl_de) // 0x19
DUMMY(op_ld_a_de) // 0x1A
DUMMY(op_dec_de) // 0x1B
DUMMY(op_inc_e) // 0x1C
DUMMY(op_dec_e) // 0x1D
DUMMY(op_ld_e_d8) // 0x1E
DUMMY(op_rra) // 0x1F
DUMMY(op_jr_nz) // 0x20
DUMMY(op_ld_hl_d16) // 0x21
DUMMY(op_ld_hli_a) // 0x22
DUMMY(op_inc_hl) // 0x23
DUMMY(op_inc_h) // 0x24
DUMMY(op_dec_h) // 0x25
DUMMY(op_ld_h_d8) // 0x26
DUMMY(op_daa) // 0x27
DUMMY(op_jr_z) // 0x28
DUMMY(op_add_hl_hl) // 0x29
DUMMY(op_ld_a_hli) // 0x2A
DUMMY(op_dec_hl) // 0x2B
DUMMY(op_inc_l) // 0x2C
DUMMY(op_dec_l) // 0x2D
DUMMY(op_ld_l_d8) // 0x2E
DUMMY(op_cpl) // 0x2F
DUMMY(op_jr_nc) // 0x30
DUMMY(op_ld_sp_d16) // 0x31
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
DUMMY(op_ld_b_b) // 0x40
DUMMY(op_ld_b_c) // 0x41
DUMMY(op_ld_b_d) // 0x42
DUMMY(op_ld_b_e) // 0x43
DUMMY(op_ld_b_h) // 0x44
DUMMY(op_ld_b_l) // 0x45
DUMMY(op_ld_b_hl) // 0x46
DUMMY(op_ld_b_a) // 0x47
DUMMY(op_ld_c_b) // 0x48
DUMMY(op_ld_c_c) // 0x49
DUMMY(op_ld_c_d) // 0x4A
DUMMY(op_ld_c_e) // 0x4B
DUMMY(op_ld_c_h) // 0x4C
DUMMY(op_ld_c_l) // 0x4D
DUMMY(op_ld_c_hl) // 0x4E
DUMMY(op_ld_c_a_4f) // 0x4F
DUMMY(op_ld_d_b) // 0x50
DUMMY(op_ld_d_c) // 0x51
DUMMY(op_ld_d_d) // 0x52
DUMMY(op_ld_d_e) // 0x53
DUMMY(op_ld_d_h) // 0x54
DUMMY(op_ld_d_l) // 0x55
DUMMY(op_ld_d_hl) // 0x56
DUMMY(op_ld_d_a) // 0x57
DUMMY(op_ld_e_b) // 0x58
DUMMY(op_ld_e_c) // 0x59
DUMMY(op_ld_e_d) // 0x5A
DUMMY(op_ld_e_e) // 0x5B
DUMMY(op_ld_e_h) // 0x5C
DUMMY(op_ld_e_l) // 0x5D
DUMMY(op_ld_e_hl) // 0x5E
DUMMY(op_ld_e_a) // 0x5F
DUMMY(op_ld_h_b) // 0x60
DUMMY(op_ld_h_c) // 0x61
DUMMY(op_ld_h_d) // 0x62
DUMMY(op_ld_h_e) // 0x63
DUMMY(op_ld_h_h) // 0x64
DUMMY(op_ld_h_l) // 0x65
DUMMY(op_ld_h_hl) // 0x66
DUMMY(op_ld_h_a) // 0x67
DUMMY(op_ld_l_b) // 0x68
DUMMY(op_ld_l_c) // 0x69
DUMMY(op_ld_l_d) // 0x6A
DUMMY(op_ld_l_e) // 0x6B
DUMMY(op_ld_l_h) // 0x6C
DUMMY(op_ld_l_l) // 0x6D
DUMMY(op_ld_l_hl) // 0x6E
DUMMY(op_ld_l_a) // 0x6F
DUMMY(op_ld_hl_b) // 0x70
DUMMY(op_ld_hl_c) // 0x71
DUMMY(op_ld_hl_d) // 0x72
DUMMY(op_ld_hl_e) // 0x73
DUMMY(op_ld_hl_h) // 0x74
DUMMY(op_ld_hl_l) // 0x75
DUMMY(op_ld_hl_a) // 0x77
DUMMY(op_ld_a_b) // 0x78
DUMMY(op_ld_a_c_79) // 0x79
DUMMY(op_ld_a_d) // 0x7A
DUMMY(op_ld_a_e) // 0x7B
DUMMY(op_ld_a_h) // 0x7C
DUMMY(op_ld_a_l) // 0x7D
DUMMY(op_ld_a_hl) // 0x7E
DUMMY(op_ld_a_a) // 0x7F
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
DUMMY(op_sub_b) // 0x90
DUMMY(op_sub_c) // 0x91
DUMMY(op_sub_d) // 0x92
DUMMY(op_sub_e) // 0x93
DUMMY(op_sub_h) // 0x94
DUMMY(op_sub_l) // 0x95
DUMMY(op_sub_hl) // 0x96
DUMMY(op_sub_a) // 0x97
DUMMY(op_sbc_a_b) // 0x98
DUMMY(op_sbc_a_c) // 0x99
DUMMY(op_sbc_a_d) // 0x9A
DUMMY(op_sbc_a_e) // 0x9B
DUMMY(op_sbc_a_h) // 0x9C
DUMMY(op_sbc_a_l) // 0x9D
DUMMY(op_sbc_a_hl) // 0x9E
DUMMY(op_sbc_a_a) // 0x9F
DUMMY(op_and_b) // 0xA0
DUMMY(op_and_c) // 0xA1
DUMMY(op_and_d) // 0xA2
DUMMY(op_and_e) // 0xA3
DUMMY(op_and_h) // 0xA4
DUMMY(op_and_l) // 0xA5
DUMMY(op_and_hl) // 0xA6
DUMMY(op_and_a) // 0xA7
DUMMY(op_xor_b) // 0xA8
DUMMY(op_xor_c) // 0xA9
DUMMY(op_xor_d) // 0xAA
DUMMY(op_xor_e) // 0xAB
DUMMY(op_xor_h) // 0xAC
DUMMY(op_xor_l) // 0xAD
DUMMY(op_xor_hl) // 0xAE
DUMMY(op_xor_a) // 0xAF
DUMMY(op_or_b) // 0xB0
DUMMY(op_or_c) // 0xB1
DUMMY(op_or_d) // 0xB2
DUMMY(op_or_e) // 0xB3
DUMMY(op_or_h) // 0xB4
DUMMY(op_or_l) // 0xB5
DUMMY(op_or_hl) // 0xB6
DUMMY(op_or_a) // 0xB7
DUMMY(op_cp_b) // 0xB8
DUMMY(op_cp_c) // 0xB9
DUMMY(op_cp_d) // 0xBA
DUMMY(op_cp_e) // 0xBB
DUMMY(op_cp_h) // 0xBC
DUMMY(op_cp_l) // 0xBD
DUMMY(op_cp_hl) // 0xBE
DUMMY(op_cp_a) // 0xBF
DUMMY(op_ret_nz) // 0xC0
DUMMY(op_pop_bc) // 0xC1
DUMMY(op_jp_nz) // 0xC2
DUMMY(op_jp) // 0xC3
DUMMY(op_call_nz) // 0xC4
DUMMY(op_push_bc) // 0xC5
DUMMY(op_add_a_d8) // 0xC6
DUMMY(op_rst_00) // 0xC7
DUMMY(op_ret_z) // 0xC8
DUMMY(op_ret) // 0xC9
DUMMY(op_jp_z) // 0xCA
DUMMY(op_cb_prefix) // 0xCB
DUMMY(op_call_z) // 0xCC
DUMMY(op_call) // 0xCD
DUMMY(op_adc_a_d8) // 0xCE
DUMMY(op_rst_08) // 0xCF
DUMMY(op_ret_nc) // 0xD0
DUMMY(op_pop_de) // 0xD1
DUMMY(op_jp_nc) // 0xD2
DUMMY(op_illegal_d3) // 0xD3
DUMMY(op_call_nc) // 0xD4
DUMMY(op_push_de) // 0xD5
DUMMY(op_sub_d8) // 0xD6
DUMMY(op_rst_10) // 0xD7
DUMMY(op_ret_c) // 0xD8
DUMMY(op_reti) // 0xD9
DUMMY(op_jp_c) // 0xDA
DUMMY(op_illegal_db) // 0xDB
DUMMY(op_call_c) // 0xDC
DUMMY(op_illegal_dd) // 0xDD
DUMMY(op_sbc_a_d8) // 0xDE
DUMMY(op_rst_18) // 0xDF
DUMMY(op_ldh_a8_a) // 0xE0
DUMMY(op_pop_hl) // 0xE1
DUMMY(op_ld_c_a_e2) // 0xE2
DUMMY(op_illegal_e3) // 0xE3
DUMMY(op_illegal_e4) // 0xE4
DUMMY(op_push_hl) // 0xE5
DUMMY(op_and_d8) // 0xE6
DUMMY(op_rst_20) // 0xE7
DUMMY(op_add_sp_e8) // 0xE8
DUMMY(op_jp_hl) // 0xE9
DUMMY(op_ld_a16_a) // 0xEA
DUMMY(op_illegal_eb) // 0xEB
DUMMY(op_illegal_ec) // 0xEC
DUMMY(op_illegal_ed) // 0xED
DUMMY(op_xor_d8) // 0xEE
DUMMY(op_rst_28) // 0xEF
DUMMY(op_ldh_a_a8) // 0xF0
DUMMY(op_pop_af) // 0xF1
DUMMY(op_ld_a_c_f2) // 0xF2
DUMMY(op_di) // 0xF3
DUMMY(op_illegal_f4) // 0xF4
DUMMY(op_push_af) // 0xF5
DUMMY(op_or_d8) // 0xF6
DUMMY(op_rst_30) // 0xF7
DUMMY(op_ld_hl_sp_e8) // 0xF8
DUMMY(op_ld_sp_hl) // 0xF9
DUMMY(op_ld_a_a16) // 0xFA
DUMMY(op_ei) // 0xFB
DUMMY(op_illegal_fc) // 0xFC
DUMMY(op_illegal_fd) // 0xFD
DUMMY(op_cp_d8) // 0xFE
DUMMY(op_rst_38) // 0xFF