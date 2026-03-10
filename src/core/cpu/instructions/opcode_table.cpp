#include "../../../../include/opcode_table.hpp"
#include "../../../../include/ProcessingUnit.hpp"
#include "../../../../include/mmu.hpp"
#include "../../../../include/opcodes.hpp"

OpcodeFunc instructionTable[256] = {

    // 0x00
    op_nop, op_ld_bc_d16, op_ld_bc_a, op_inc_bc,
    op_inc_b, op_dec_b, op_ld_b_d8, op_rlca,
    op_ld_a16_sp, op_add_hl_bc, op_ld_a_bc, op_dec_bc,
    op_inc_c, op_dec_c, op_ld_c_d8, op_rrca,

    // 0x10
    op_stop, op_ld_de_d16, op_ld_de_a, op_inc_de,
    op_inc_d, op_dec_d, op_ld_d_d8, op_rla,
    op_jr_r8, op_add_hl_de, op_ld_a_de, op_dec_de,
    op_inc_e, op_dec_e, op_ld_e_d8, op_rra,

    // 0x20
    op_jr_nz, op_ld_hl_d16, op_ld_hli_a, op_inc_hl,
    op_inc_h, op_dec_h, op_ld_h_d8, op_daa,
    op_jr_z, op_add_hl_hl, op_ld_a_hli, op_dec_hl,
    op_inc_l, op_dec_l, op_ld_l_d8, op_cpl,

    // 0x30
    op_jr_nc, op_ld_sp_d16, op_ld_hld_a, op_inc_sp,
    op_inc_hl_ptr, op_dec_hl_ptr, op_ld_hl_d8, op_scf,
    op_jr_c, op_add_hl_sp, op_ld_a_hld, op_dec_sp,
    op_inc_a, op_dec_a, op_ld_a_d8, op_ccf,

    // 0x40
    op_ld_b_b, op_ld_b_c, op_ld_b_d, op_ld_b_e,
    op_ld_b_h, op_ld_b_l, op_ld_b_hl, op_ld_b_a,
    op_ld_c_b, op_ld_c_c, op_ld_c_d, op_ld_c_e,
    op_ld_c_h, op_ld_c_l, op_ld_c_hl, op_ld_c_a_4f,

    // 0x50
    op_ld_d_b, op_ld_d_c, op_ld_d_d, op_ld_d_e,
    op_ld_d_h, op_ld_d_l, op_ld_d_hl, op_ld_d_a,
    op_ld_e_b, op_ld_e_c, op_ld_e_d, op_ld_e_e,
    op_ld_e_h, op_ld_e_l, op_ld_e_hl, op_ld_e_a,

    // 0x60
    op_ld_h_b, op_ld_h_c, op_ld_h_d, op_ld_h_e,
    op_ld_h_h, op_ld_h_l, op_ld_h_hl, op_ld_h_a,
    op_ld_l_b, op_ld_l_c, op_ld_l_d, op_ld_l_e,
    op_ld_l_h, op_ld_l_l, op_ld_l_hl, op_ld_l_a,

    // 0x70
    op_ld_hl_b, op_ld_hl_c, op_ld_hl_d, op_ld_hl_e,
    op_ld_hl_h, op_ld_hl_l, op_halt, op_ld_hl_a,
    op_ld_a_b, op_ld_a_c_79, op_ld_a_d, op_ld_a_e,
    op_ld_a_h, op_ld_a_l, op_ld_a_hl, op_ld_a_a,

    // 0x80
    op_add_a_b, op_add_a_c, op_add_a_d, op_add_a_e,
    op_add_a_h, op_add_a_l, op_add_a_hl, op_add_a_a,
    op_adc_a_b, op_adc_a_c, op_adc_a_d, op_adc_a_e,
    op_adc_a_h, op_adc_a_l, op_adc_a_hl, op_adc_a_a,

    // 0x90
    op_sub_b, op_sub_c, op_sub_d, op_sub_e,
    op_sub_h, op_sub_l, op_sub_hl, op_sub_a,
    op_sbc_a_b, op_sbc_a_c, op_sbc_a_d, op_sbc_a_e,
    op_sbc_a_h, op_sbc_a_l, op_sbc_a_hl, op_sbc_a_a,

    // 0xA0
    op_and_b, op_and_c, op_and_d, op_and_e,
    op_and_h, op_and_l, op_and_hl, op_and_a,
    op_xor_b, op_xor_c, op_xor_d, op_xor_e,
    op_xor_h, op_xor_l, op_xor_hl, op_xor_a,

    // 0xB0
    op_or_b, op_or_c, op_or_d, op_or_e,
    op_or_h, op_or_l, op_or_hl, op_or_a,
    op_cp_b, op_cp_c, op_cp_d, op_cp_e,
    op_cp_h, op_cp_l, op_cp_hl, op_cp_a,

    // 0xC0
    op_ret_nz, op_pop_bc, op_jp_nz, op_jp,
    op_call_nz, op_push_bc, op_add_a_d8, op_rst_00,
    op_ret_z, op_ret, op_jp_z, op_cb_prefix,
    op_call_z, op_call, op_adc_a_d8, op_rst_08,

    // 0xD0
    op_ret_nc, op_pop_de, op_jp_nc, op_illegal_d3,
    op_call_nc, op_push_de, op_sub_d8, op_rst_10,
    op_ret_c, op_reti, op_jp_c, op_illegal_db,
    op_call_c, op_illegal_dd, op_sbc_a_d8, op_rst_18,

    // 0xE0
    op_ldh_a8_a, op_pop_hl, op_ld_c_a_e2, op_illegal_e3,
    op_illegal_e4, op_push_hl, op_and_d8, op_rst_20,
    op_add_sp_e8, op_jp_hl, op_ld_a16_a, op_illegal_eb,
    op_illegal_ec, op_illegal_ed, op_xor_d8, op_rst_28,

    // 0xF0
    op_ldh_a_a8, op_pop_af, op_ld_a_c_f2, op_di,
    op_illegal_f4, op_push_af, op_or_d8, op_rst_30,
    op_ld_hl_sp_e8, op_ld_sp_hl, op_ld_a_a16, op_ei,
    op_illegal_fc, op_illegal_fd, op_cp_d8, op_rst_38
};
