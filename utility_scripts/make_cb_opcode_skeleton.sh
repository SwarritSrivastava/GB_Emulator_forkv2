#!/usr/bin/env bash

set -e

cb_opcodes=(
    op_rlc_b op_rlc_c op_rlc_d op_rlc_e op_rlc_h op_rlc_l op_rlc_hl op_rlc_a
    op_rrc_b op_rrc_c op_rrc_d op_rrc_e op_rrc_h op_rrc_l op_rrc_hl op_rrc_a

    op_rl_b op_rl_c op_rl_d op_rl_e op_rl_h op_rl_l op_rl_hl op_rl_a
    op_rr_b op_rr_c op_rr_d op_rr_e op_rr_h op_rr_l op_rr_hl op_rr_a

    op_sla_b op_sla_c op_sla_d op_sla_e op_sla_h op_sla_l op_sla_hl op_sla_a
    op_sra_b op_sra_c op_sra_d op_sra_e op_sra_h op_sra_l op_sra_hl op_sra_a

    op_swap_b op_swap_c op_swap_d op_swap_e op_swap_h op_swap_l op_swap_hl op_swap_a
    op_srl_b op_srl_c op_srl_d op_srl_e op_srl_h op_srl_l op_srl_hl op_srl_a

    op_bit_0_b op_bit_0_c op_bit_0_d op_bit_0_e op_bit_0_h op_bit_0_l op_bit_0_hl op_bit_0_a
    op_bit_1_b op_bit_1_c op_bit_1_d op_bit_1_e op_bit_1_h op_bit_1_l op_bit_1_hl op_bit_1_a

    op_bit_2_b op_bit_2_c op_bit_2_d op_bit_2_e op_bit_2_h op_bit_2_l op_bit_2_hl op_bit_2_a
    op_bit_3_b op_bit_3_c op_bit_3_d op_bit_3_e op_bit_3_h op_bit_3_l op_bit_3_hl op_bit_3_a

    op_bit_4_b op_bit_4_c op_bit_4_d op_bit_4_e op_bit_4_h op_bit_4_l op_bit_4_hl op_bit_4_a
    op_bit_5_b op_bit_5_c op_bit_5_d op_bit_5_e op_bit_5_h op_bit_5_l op_bit_5_hl op_bit_5_a

    op_bit_6_b op_bit_6_c op_bit_6_d op_bit_6_e op_bit_6_h op_bit_6_l op_bit_6_hl op_bit_6_a
    op_bit_7_b op_bit_7_c op_bit_7_d op_bit_7_e op_bit_7_h op_bit_7_l op_bit_7_hl op_bit_7_a

    op_res_0_b op_res_0_c op_res_0_d op_res_0_e op_res_0_h op_res_0_l op_res_0_hl op_res_0_a
    op_res_1_b op_res_1_c op_res_1_d op_res_1_e op_res_1_h op_res_1_l op_res_1_hl op_res_1_a

    op_res_2_b op_res_2_c op_res_2_d op_res_2_e op_res_2_h op_res_2_l op_res_2_hl op_res_2_a
    op_res_3_b op_res_3_c op_res_3_d op_res_3_e op_res_3_h op_res_3_l op_res_3_hl op_res_3_a

    op_res_4_b op_res_4_c op_res_4_d op_res_4_e op_res_4_h op_res_4_l op_res_4_hl op_res_4_a
    op_res_5_b op_res_5_c op_res_5_d op_res_5_e op_res_5_h op_res_5_l op_res_5_hl op_res_5_a

    op_res_6_b op_res_6_c op_res_6_d op_res_6_e op_res_6_h op_res_6_l op_res_6_hl op_res_6_a
    op_res_7_b op_res_7_c op_res_7_d op_res_7_e op_res_7_h op_res_7_l op_res_7_hl op_res_7_a

    op_set_0_b op_set_0_c op_set_0_d op_set_0_e op_set_0_h op_set_0_l op_set_0_hl op_set_0_a
    op_set_1_b op_set_1_c op_set_1_d op_set_1_e op_set_1_h op_set_1_l op_set_1_hl op_set_1_a

    op_set_2_b op_set_2_c op_set_2_d op_set_2_e op_set_2_h op_set_2_l op_set_2_hl op_set_2_a
    op_set_3_b op_set_3_c op_set_3_d op_set_3_e op_set_3_h op_set_3_l op_set_3_hl op_set_3_a

    op_set_4_b op_set_4_c op_set_4_d op_set_4_e op_set_4_h op_set_4_l op_set_4_hl op_set_4_a
    op_set_5_b op_set_5_c op_set_5_d op_set_5_e op_set_5_h op_set_5_l op_set_5_hl op_set_5_a

    op_set_6_b op_set_6_c op_set_6_d op_set_6_e op_set_6_h op_set_6_l op_set_6_hl op_set_6_a
    op_set_7_b op_set_7_c op_set_7_d op_set_7_e op_set_7_h op_set_7_l op_set_7_hl op_set_7_a
)

# Detect duplicates
declare -A seen

for i in "${!cb_opcodes[@]}"; do
    name="${cb_opcodes[$i]}"

    if [[ -n "${seen[$name]}" ]]; then
        final_name="${name}_0xcb$(printf "%02X" "$i")"
    else
        seen[$name]=1
        final_name="$name"
    fi

    printf "int %s(ProcessingUnit &processing_unit, MMU &mmu); // 0xCB%02X\n" "$final_name" "$i"
done

echo

# Generate STUBS
unset seen
declare -A seen

for i in "${!cb_opcodes[@]}"; do
    name="${cb_opcodes[$i]}"

    if [[ -n "${seen[$name]}" ]]; then
        final_name="${name}_0xcb$(printf "%02X" "$i")"
    else
        seen[$name]=1
        final_name="$name"
    fi

    printf "STUB(%s) // 0xCB%02X\n" "$final_name" "$i"
done
