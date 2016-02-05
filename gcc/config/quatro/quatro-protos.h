/* Prototypes for exported functions defined in quatro.c
   Copyright (C) 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.
   Contributed by Zoran Corporation.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA. 
 
Copyright (c) 2016, The Linux Foundation. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 and
only version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/


#ifdef RTX_CODE
extern rtx arch_compare_op0;
extern rtx arch_compare_op1;
extern struct rtx_def * branch_cmp[2];  /* operands for compare */
#endif /* RTX_CODE */

#ifdef RTX_CODE
extern int              call_address_operand                PARAMS ((rtx, enum machine_mode));
extern int              address_register_operand            PARAMS ((rtx, enum machine_mode));
extern int              call_operand                        PARAMS ((rtx, enum machine_mode)); 
extern int              symbolic_operand                    PARAMS ((rtx, enum machine_mode));
extern const char *     quatro_output_movsi                 PARAMS ((rtx *));
extern const char *     quatro_output_movsi_specreg         PARAMS ((rtx *));
extern const char *     quatro_output_move                  PARAMS ((rtx, rtx *, enum machine_mode));
extern const char *     quatro_output_movsf                 PARAMS ((rtx, rtx *, enum machine_mode));
extern int              quatro_arith_lit9_operand           PARAMS ((rtx, enum machine_mode));
extern int              quatro_arith_lit16_operand          PARAMS ((rtx, enum machine_mode));
extern int              const_ok_for_lit9_operand           PARAMS ((rtx, enum machine_mode));
extern int              const_ok_for_ulit16_operand         PARAMS ((rtx, enum machine_mode));
extern int              const_ok_for_memlisi_operand        PARAMS ((rtx, enum machine_mode));
extern int              const_ok_for_shift_operand          PARAMS ((rtx, enum machine_mode));
extern int              quatro_memli_operand                PARAMS ((rtx, enum machine_mode));
extern int              quatro_arith_complit_operand        PARAMS ((rtx, enum machine_mode));
extern void             quatro_expand_prolog                PARAMS ((void));
extern void             quatro_expand_epilog                PARAMS ((void));
extern int              quatro_return_in_memory             PARAMS ((tree));
extern int              quatro_function_value_regno_p       PARAMS ((int));
extern int              quatro_function_arg_regno_p         PARAMS ((int));
extern void             gen_conditional_branch              PARAMS ((rtx *, enum rtx_code));
extern void             quatro_gen_ashrsi3                  PARAMS ((rtx *));
extern void             quatro_gen_lshrsi3                  PARAMS ((rtx *));
extern void             quatro_gen_ashlsi3                  PARAMS ((rtx *));
extern const char *     output_jump                         PARAMS ((rtx *));
extern const char *     output_call_value_via_reg           PARAMS ((rtx *));
extern const char *     output_call_via_reg                 PARAMS ((rtx *));
extern int              reg_ok_for_base_p                   PARAMS ((rtx));
extern int              reg_ok_for_index_p                  PARAMS ((rtx));
extern int              reg_ok_for_base_p_strict            PARAMS ((rtx));
extern int              reg_ok_for_index_p_strict           PARAMS ((rtx));
extern int              is_base_register_rtx                PARAMS ((int, rtx));
extern int              is_index_register_rtx               PARAMS ((int, rtx));
extern int              quatro_legit_addr                   PARAMS ((int, enum machine_mode, rtx));
extern int              quatro_address_cost                 PARAMS ((rtx));
extern const char *     quatro_sub_handle                   PARAMS ((rtx *));
extern int              quatro_config_operand               PARAMS ((rtx, enum machine_mode));
extern int              quatro_trans_operand                PARAMS ((rtx, enum machine_mode));
extern rtx              quatro_legitimize_address           PARAMS ((rtx, rtx, enum machine_mode));
extern rtx              quatro_legitimize_reload_address    PARAMS ((rtx, enum machine_mode, int, int, int));
extern int              quatro_non_lit9_operand             PARAMS ((rtx, enum machine_mode));
extern int              quatro_non_lit16_operand            PARAMS ((rtx, enum machine_mode));
extern void             quatro_output_addr_vec_elt          PARAMS ((FILE *, int));
extern int              quatro_must_pass_on_stack           PARAMS ((enum machine_mode, tree));
extern void             quatro_print_operand_address        PARAMS ((FILE *, rtx));
extern void             quatro_print_operand                PARAMS ((FILE *, rtx, int));
extern int              quatro_must_pass_on_stack           PARAMS ((enum machine_mode, tree));
extern int              quatro_num_arg_regs                 PARAMS ((enum machine_mode, tree));
extern rtx              quatro_function_value               PARAMS ((tree, tree));
extern rtx              quatro_function_arg                 PARAMS ((CUMULATIVE_ARGS, enum machine_mode, tree, int));
extern int              quatro_function_arg_partial_nregs   PARAMS ((CUMULATIVE_ARGS, enum machine_mode, tree, int));
extern int              quatro_dllexport_name_p             PARAMS ((const char *));
extern int              quatro_dllimport_name_p             PARAMS ((const char *));
extern void             quatro_encode_section_info          PARAMS ((tree));
extern void             quatro_unique_section               PARAMS ((tree, int));
extern int              quatro_naked_function_p             PARAMS ((void));
extern void             quatro_asm_file_start               PARAMS ((FILE *, const char *));
#endif /* RTX_CODE */

#ifdef GCC_C_PRAGMA_H
extern void quatro_pr_reg PARAMS ((cpp_reader *));
#endif

