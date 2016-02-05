;;  Machine description the quatro dsp
;;  Copyright (C) 1999, 2000, 2003, 2004 Free Software Foundation, Inc.
;;  Contributed by Zoran Corporation.

;; This file is part of GNU CC.

;; GNU CC is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation; either version 2, or (at your option)
;; any later version.

;; GNU CC is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GNU CC; see the file COPYING.  If not, write to
;; the Free Software Foundation, 59 Temple Place - Suite 330,
;; Boston, MA 02111-1307, USA.

;; Copyright (c) 2016, The Linux Foundation. All rights reserved.

;; This program is free software; you can redistribute it and/or modify
;; it under the terms of the GNU General Public License version 2 and
;; only version 2 as published by the Free Software Foundation.

;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.

;;- See file "rtl.def" for documentation on define_insn, match_*, et. al.



;; -------------------------------------------------------------------------
;; Constants
;; -------------------------------------------------------------------------
(define_constants [
  (LA         40)
  (EXT_NXT    41)
  (INS_EPOS   42)
  (INS_WIDTH  43)
  (INS_MODE   44)
  (INS_IPOS   45)
  (LI         46)
  (LIT_HIGH   47)

  (UNSPEC_EXTL      1)
  (UNSPEC_INS       2)
  (UNSPEC_SPECREG   3)
  (UNSPEC_SHIFTLEFT 4)
  (UNSPEC_ADD       5)
  (UNSPEC_EXTR		6)
])

;; -------------------------------------------------------------------------
;; Attributes
;; -------------------------------------------------------------------------

; Target CPU.


;;(define_attr name list-of-values default)
; All quatro instructions are four bytes long.
(define_attr "length" "" (const_int 4))

(define_attr "type" "branch,load,store,operate,move" (const_string "operate"))

(define_function_unit "memory" 1 0 (eq_attr "type" "load") 6 0)
;(define_function_unit "alu" 1 1 (eq_attr "type" "operate")  3 0)

;; Branch and call insns require three slots.  Annulling is not
;; supported.
(define_delay (eq_attr "type"  "branch")
             [(eq_attr "type" "!branch") (nil) (nil)
              (eq_attr "type" "!branch") (nil) (nil)
              (eq_attr "type" "!branch") (nil) (nil)])

;; -------------------------------------------------------------------------
;; Test and bit test
;; -------------------------------------------------------------------------

;; -------------------------------------------------------------------------
;; SImode signed integer comparisons
;; -------------------------------------------------------------------------

;; -------------------------------------------------------------------------
;; SImode unsigned integer comparisons
;; -------------------------------------------------------------------------
/*Quatro Compare And Branch*/

; (define_expand "movsicc"
;   [(set (match_operand:SI 0 "register_operand" "")
; 	(if_then_else:SI (match_operand 1 "comparison_operator" "")
; 			 (match_operand:SI 2 "register_operand" "")
; 			 (match_operand:SI 3 "register_operand" "")))]
;   "TARGET_ARM"
;   "
;   {
;     enum rtx_code code = GET_CODE (operands[1]);
;     rtx ccreg = arm_gen_compare_reg (code, arm_compare_op0, arm_compare_op1);

;     operands[1] = gen_rtx (code, VOIDmode, ccreg, const0_rtx);
;   }"
; )

;  (define_insn ""
;    [(set (match_operand:SI 0 "register_operand" "=r")
;  	(if_then_else (ne:SI (match_operand:SI 1 "register_operand" "r") (const_int 0))
;                   (match_dup 1) 
;                (match_operand:SI 2 "register_operand" "r")))] 
;    ""
;    "%0=SELECT(%1!=0,%2,%1)"
;    [(set_attr "type" "operate")])

;  (define_insn ""
;    [(set (match_operand:SI 0 "register_operand" "=r")
;  	(if_then_else (le:SI (match_operand:SI 1 "register_operand" "r") (const_int 0))
;                   (match_dup 1) 
;                (match_operand:SI 2 "register_operand" "r")))] 
;    ""
;    "%0=SELECT(%1<=0,%2,%1)"
;    [(set_attr "type" "operate")])

;  (define_insn ""
;    [(set (match_operand:SI 0 "register_operand" "=r")
;  	(if_then_else (ge:SI (match_operand:SI 1 "register_operand" "r") (const_int 0))
;                   (match_dup 1) 
;                (match_operand:SI 2 "register_operand" "r")))] 
;    ""
;    "%0=SELECT(%1>=0,%2,%1)"
;    [(set_attr "type" "operate")])

;  (define_insn ""
;    [(set (match_operand:SI 0 "register_operand" "=r")
;  	(if_then_else (eq:SI (match_operand:SI 1 "register_operand" "r") (const_int 0))
;                   (match_dup 1) 
;                (match_operand:SI 2 "register_operand" "r")))] 
;    ""
;    "%0=SELECT(%1==0,%2,%1)"
;    [(set_attr "type" "operate")])

;  (define_insn ""
;    [(set (match_operand:SI 0 "register_operand" "=r")
;  	(if_then_else (gt:SI (match_operand:SI 1 "register_operand" "r") (const_int 0))
;                   (match_dup 1) 
;                (match_operand:SI 2 "register_operand" "r")))] 
;    ""
;    "%0=SELECT(%1>0,%2,%1)"
;    [(set_attr "type" "operate")])

;  (define_insn ""
;    [(set (match_operand:SI 0 "register_operand" "=r")
;  	(if_then_else (lt:SI (match_operand:SI 1 "register_operand" "r") (const_int 0))
;                   (match_dup 1) 
;                   (match_operand:SI 2 "register_operand" "r")))] 
;    ""
;    "%0=SELECT(%1<0,%2,%1)"
;    [(set_attr "type" "operate")])


(define_expand "cmpsi"
  [(set (cc0)
	(compare:CC (match_operand:SI 0 "register_operand" "r")
		 (match_operand:SI 1 "nonmemory_operand" "ri")))]
  ""
  "
{
  arch_compare_op0 = operands[0];
  arch_compare_op1 = operands[1];
  DONE;
}")

(define_expand "beq"
  [(set (pc)
	(if_then_else (eq:CC (cc0)
			     (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "
{
  if (operands[0])		/* avoid unused code warning */
    {
      gen_conditional_branch (operands, EQ);
      DONE;
    }
}")

(define_expand "bne"
  [(set (pc)
	(if_then_else (ne:CC (cc0)
			     (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "
{
  if (operands[0])		/* avoid unused code warning */
    {
      gen_conditional_branch (operands, NE);
      DONE;
    }
}")

(define_expand "bgt"
  [(set (pc)
	(if_then_else (gt:CC (cc0)
			     (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "
{
  if (operands[0])		/* avoid unused code warning */
    {
      gen_conditional_branch (operands, GT);
      DONE;
    }
}")

 (define_expand "bge"
   [(set (pc)
 	(if_then_else (ge:CC (cc0)
 			     (const_int 0))
 		      (label_ref (match_operand 0 "" ""))
 		      (pc)))]
   ""
   "
 {
   if (operands[0])		/* avoid unused code warning */
     {
       gen_conditional_branch (operands, GE);
       DONE;
     }
 }")

(define_expand "blt"
  [(set (pc)
	(if_then_else (lt:CC (cc0)
			     (const_int 0))
		      (label_ref (match_operand 0 "" ""))
		      (pc)))]
  ""
  "
{
  if (operands[0])		/* avoid unused code warning */
    {
      gen_conditional_branch (operands, LT);
      DONE;
    }
}")

 (define_expand "ble"
   [(set (pc)
 	(if_then_else (le:CC (cc0)
 			     (const_int 0))
 		      (label_ref (match_operand 0 "" ""))
 		      (pc)))]
   ""
   "
 {
   if (operands[0])		/* avoid unused code warning */
     {
       gen_conditional_branch (operands, LE);
       DONE;
     }
 }")

 (define_expand "bequ"
   [(set (pc)
 	(if_then_else (eq:CC (cc0)
 			     (const_int 0))
 		      (label_ref (match_operand 0 "" ""))
 		      (pc)))]
   ""
   "
 {
   if (operands[0])		/* avoid unused code warning */
     {
       gen_conditional_branch (operands, EQ);
       DONE;
     }
 }")


 (define_expand "bneu"
  [(set (pc)
 	(if_then_else (ne:CC (cc0)
 			     (const_int 0))
 		      (label_ref (match_operand 0 "" ""))
 		      (pc)))]
   ""
   "
 {
   if (operands[0])		/* avoid unused code warning */
     {
       gen_conditional_branch (operands, NE);
       DONE;
     }
 }")


 (define_expand "bgtu"
  [(set (pc)
	(if_then_else (gt:CC (cc0)
			     (const_int 0))
 		      (label_ref (match_operand 0 "" ""))
 		      (pc)))]
   ""
   "
 {
   if (operands[0])		/* avoid unused code warning */
     {
       gen_conditional_branch (operands, GTU);
       DONE;
     }
 }")



 (define_expand "bgeu"
   [(set (pc)
 	(if_then_else (ge:CC (cc0)
 			     (const_int 0))
 		      (label_ref (match_operand 0 "" ""))
 		      (pc)))]
   ""
   "
 {
   if (operands[0])		/* avoid unused code warning */
     {
       gen_conditional_branch (operands, GEU);
       DONE;
     }
 }")

 (define_expand "bltu"
   [(set (pc)
 	(if_then_else (lt:CC (cc0)
 			     (const_int 0))
 		      (label_ref (match_operand 0 "" ""))
 		      (pc)))]
   ""
   "
 {
   if (operands[0])		/* avoid unused code warning */
     {
       gen_conditional_branch (operands, LTU);
       DONE;
     }
 }")

 (define_expand "bleu"
   [(set (pc)
 	(if_then_else (le:CC (cc0)
 			     (const_int 0))
 		      (label_ref (match_operand 0 "" ""))
 		      (pc)))]
   ""
   "
 {
   if (operands[0])		/* avoid unused code warning */
     {
       gen_conditional_branch (operands, LEU);
       DONE;
     }
 }")

(define_insn ""
  [(set (pc) (if_then_else (lt 
                 (match_operand:SI 0 "register_operand" "r")
                    (const_int 0))
        (label_ref (match_operand 1 "" ""))
        (pc)))]
    ""
    "IF (%0.a < 0, %0.a+=0) GOTO %1;%#"
    [(set_attr "type" "branch")])

(define_insn ""
  [(set (pc) (if_then_else (gt 
                 (match_operand:SI 0 "register_operand" "r")
                    (const_int 0))
        (label_ref (match_operand 1 "" ""))
        (pc)))]
    ""
    "IF (%0.a > 0, %0.a+=0) GOTO %1;%#"
    [(set_attr "type" "branch")])

(define_insn ""
  [(set (pc) (if_then_else (ge 
                 (match_operand:SI 0 "register_operand" "r")
                    (const_int 0))
        (label_ref (match_operand 1 "" ""))
        (pc)))]
    ""
    "IF (%0.a >= 0, %0.a+=0) GOTO %1;%#"
    [(set_attr "type" "branch")])


(define_insn ""
  [(set (pc) (if_then_else (le 
                 (match_operand:SI 0 "register_operand" "r")
                    (const_int 0))
        (label_ref (match_operand 1 "" ""))
        (pc)))]
    ""
    "IF (%0.a <= 0, %0.a+=0) GOTO %1;%#"
    [(set_attr "type" "branch")])


(define_insn ""
  [(set (pc) (if_then_else (ne 
                 (match_operand:SI 0 "register_operand" "r")
                    (const_int 0))
        (label_ref (match_operand 1 "" ""))
        (pc)))]
    ""
    "IF (%0.a != 0, %0.a+=0) GOTO %1;%#"
    [(set_attr "type" "branch")])



(define_insn ""
  [(set (pc) (if_then_else (eq 
                 (match_operand:SI 0 "register_operand" "r")
                    (const_int 0))
        (label_ref (match_operand 1 "" ""))
        (pc)))]
    ""
    "IF (%0.a == 0, %0.a+=0) GOTO %1;%#"
    [(set_attr "type" "branch")])





;; -------------------------------------------------------------------------
;; Logical operations
;; -------------------------------------------------------------------------

(define_insn "andsi3"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
        (and:SI (match_operand:SI 1 "register_operand" "r,r")
                (match_operand:SI 2 "quatro_arith_lit9_operand" "r,L")))]
      ""
      "@
    %0 = %1 & %2;
    %0 = %1 & %2;"
   [(set_attr "type" "operate")])


(define_insn "iorsi3"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
        (ior:SI (match_operand:SI 1 "register_operand" "r,r")
                (match_operand:SI 2 "quatro_arith_lit9_operand" "r,L")))]
      ""
      "@
    %0 = %1 | %2;
    %0 = %1 | %2;"
   [(set_attr "type" "operate")])

(define_insn "xorsi3"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
        (xor:SI (match_operand:SI 1 "register_operand" "r,r")
                (match_operand:SI 2 "quatro_arith_lit9_operand" "r,L")))]
      ""
      "@
      %0 = %1 ^ %2;
      %0 = %1 ^ %2;"
   [(set_attr "type" "operate")])


;; Only allow these if the shift count is a convenient constant.
; (define_expand "rotlsi3"
;   [(set (match_operand:SI            0 "register_operand" "")
; 	(rotate:SI (match_operand:SI 1 "register_operand" "")
; 		   (match_operand:SI 2 "const_int_operand" "")))]
;   ""
;   "if (! const_int_operand (operands[2], SImode))
; 	 FAIL;
;   ")

; ;; We can only do constant rotates, which is what this pattern provides.
; ;; The combiner will put it together for us when we do:
; ;;	(x << N) | (x >> (32 - N))
; (define_insn ""
;   [(set (match_operand:SI              0 "register_operand" "=r")
; 	(rotate:SI (match_operand:SI   1 "register_operand"  "0")
; 		     (match_operand:SI 2 "const_int_operand"  "K")))]
;   ""
;   "rotli	%0,%2"
;   [(set_attr "type" "shift")])

(define_expand "ashlsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ashift:SI (match_operand:SI 1 "register_operand" "r")
		   (match_operand:SI 2 "nonmemory_operand" "ir")))]
  ""
  "
  {
    quatro_gen_ashlsi3( operands );
    DONE;
  }
  ")

(define_expand "ashrsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ashiftrt:SI (match_operand:SI 1 "register_operand" "r")
		     (match_operand:SI 2 "nonmemory_operand" "ir")))]
  ""
  "
  {
    quatro_gen_ashrsi3( operands );
    DONE;
  }
  ")

(define_expand "lshrsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(lshiftrt:SI (match_operand:SI 1 "register_operand" "r")
		     (match_operand:SI 2 "nonmemory_operand" "ir")))]
  ""
  "
  {
    quatro_gen_lshrsi3( operands );
    DONE;
  }
  "
  )

(define_insn "one_cmplsi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(not:SI (match_operand:SI 1 "register_operand" "r")))]
  ""
  "%0 = -1 - %1;")

;(define_insn "*mac_si"
;  [(set (match_operand:SI 0 "register_operand" "=r")
;	(plus:SI (mult:SI (match_operand:SI 1 "register_operand" "r")
;			  (match_operand:SI 2 "register_operand" "r"))
;		 (match_dup 0)))]
;  ""
;  "%0 = %1 * %2 + %0; #mac operation"
;  [(set_attr "type" "operate")])

;; -------------------------------------------------------------------------
;; Addition, Subtraction instructions
;; -------------------------------------------------------------------------
;;Make addsi3 accept large const because we are getting an error in gen_add2_insn.
;;I think the addsi needs to be able to handle this on its own like the mov insn.

(define_insn_and_split "*large_address"
  [(set (match_operand:SI 0 "register_operand" "=a,r")
	(plus:SI (match_operand:SI 1 "address_register_operand" "a,a")
		 (match_operand:SI 2 "quatro_non_lit16_operand" "i,i")))]
  ""
  "#"
  ""
  [(set (match_dup 6)
        (match_dup 1))
   (parallel [(unspec:SI [(match_dup 3)
	                  (match_dup 4)] UNSPEC_SPECREG)
	      (clobber (match_dup 3))])
   (parallel [(set (match_dup 7)
	           (unspec:SI [(match_dup 3)
	                       (match_dup 5)] UNSPEC_ADD))
              (use (match_dup 3))
	      (use (match_dup 5))])
   (set (match_dup 6)
	(plus:SI (match_dup 6)
	         (match_dup 7)))
   (set (match_dup 0)
	(match_dup 6))]
   "
   {
   HOST_WIDE_INT int_val = INTVAL (operands[2]);
   operands[6] = gen_rtx_REG (SImode, 31);
   operands[3] = gen_rtx_REG (SImode, LIT_HIGH);
   operands[4] = GEN_INT (int_val >> 16);
   operands[5] = GEN_INT (int_val & 0xFFFF);
   operands[7] = gen_rtx_REG (SImode, LA);
   }
   "
   [(set_attr "type" "operate")])
  
(define_insn_and_split "*int_size_address"
  [(set (match_operand:SI 0 "register_operand" "=a,r")
	(plus:SI (match_operand:SI 1 "address_register_operand" "a,a")
		 (match_operand:SI 2 "quatro_non_lit9_operand" "i,i")))]
  ""
  "#"
  ""
  [(set (match_dup 3)
        (match_dup 1))
   (set (match_dup 4)
	(match_dup 2))
   (set (match_dup 3)
	(plus:SI (match_dup 3)
		 (match_dup 4)))
   (set (match_dup 0)
	(match_dup 3))]
   "
   {
   HOST_WIDE_INT int_val = INTVAL (operands[2]);
   operands[3] = gen_rtx_REG (SImode, 31);
   operands[4] = gen_rtx_REG (SImode, LA);
   }
   "
   [(set_attr "type" "operate")])
(define_split 
  [(set (match_operand:SI 0 "register_operand" "")
	(mem:SI (plus:SI (match_operand:SI 1 "address_register_operand" "")
		 (match_operand:SI 2 "quatro_non_lit9_operand" ""))))]
  ""
  [(set (match_dup 3)
        (match_dup 2))
   (set (match_dup 0)
	(mem:SI (plus:SI (match_dup 1)
		 (match_dup 3))))]
   "
   {
   HOST_WIDE_INT int_val = INTVAL (operands[2]);
   operands[2] = GEN_INT(int_val/4);
   operands[3] = gen_rtx_REG (SImode, 31);
   }
   ")

(define_split 
  [(set (match_operand:SF 0 "register_operand" "")
	(mem:SF (plus:SI (match_operand:SI 1 "address_register_operand" "")
		 (match_operand:SI 2 "quatro_non_lit9_operand" ""))))]
  ""
  [(set (match_dup 3)
        (match_dup 2))
   (set (match_dup 0)
	(mem:SF (plus:SI (match_dup 1)
		 (match_dup 3))))]
   "
   {
   HOST_WIDE_INT int_val = INTVAL (operands[2]);
   operands[2] = GEN_INT(int_val/4);
   operands[3] = gen_rtx_REG (SImode, 31);
   }
   ")

(define_split 
  [(set (mem:SI (plus:SI (match_operand:SI 0 "address_register_operand" "")
		 (match_operand:SI 1 "quatro_non_lit9_operand" ""))) (match_operand:SI 2 "register_operand" ""))]
  ""
  [(set (match_dup 3)
        (match_dup 1))
   (set (mem:SI (plus:SI (match_dup 0)
		 (match_dup 3))) (match_dup 2))]
   "
   {
   HOST_WIDE_INT int_val = INTVAL (operands[1]);
   operands[1] = GEN_INT(int_val/4);
   operands[3] = gen_rtx_REG (SImode, 31);
   }
   ")
 

(define_split 
  [(set (mem:SF (plus:SI (match_operand:SI 0 "address_register_operand" "")
		 (match_operand:SI 1 "quatro_non_lit9_operand" ""))) (match_operand:SF 2 "register_operand" ""))]
  ""
  [(set (match_dup 3)
        (match_dup 1))
   (set (mem:SF (plus:SI (match_dup 0)
		 (match_dup 3))) (match_dup 2))]
   "
   {
   HOST_WIDE_INT int_val = INTVAL (operands[1]);
   operands[1] = GEN_INT(int_val/4);
   operands[3] = gen_rtx_REG (SImode, 31);
   }
   ")
  
(define_insn_and_split "*address_reg_offset"
  [(set (match_operand:SI 0 "register_operand" "=a")
	(plus:SI (match_operand:SI 1 "address_register_operand" "a")
		 (match_operand:SI 2 "register_operand" "g")))]
  ""
  "#"
  ""
  [(set (match_dup 3)
        (match_dup 1))
   (set (match_dup 3)
	(plus:SI (match_dup 3)
	         (match_dup 2)))
   (set (match_dup 0)
	(match_dup 3))]
   "
   {
   operands[3] = gen_rtx_REG (SImode, 31);
   }
   "
   [(set_attr "type" "operate")])
  
(define_insn "areg_add"
  [(set (match_operand:SI 0 "address_register_operand" "=a")
	(plus:SI (match_operand:SI 1 "address_register_operand" "a")
		 (match_operand:SI 2 "const_ok_for_lit9_operand" "L")))]
  ""
  "*
    {
       rtx newops[3];
       newops[0] = operands[0];
       newops[1] = operands[1];
       newops[2] = GEN_INT (INTVAL (operands[2]) / 4);
       output_asm_insn (\"%0 = LONG(%1 + %2);\", newops);
       return \"\";
    }
  "
  [(set_attr "type" "operate")])

(define_insn "addsi3"
  [(set (match_operand:SI 0 "register_operand"                  "=r,r,r,h,h,h")
	(plus:SI (match_operand:SI 1 "register_operand"          "r,r,r,r,r,r")
		 (match_operand:SI 2 "quatro_arith_lit9_operand" "r,L,h,r,L,h")))]
  ""
  "@
   %0 = %1 + %2;
   %0 = %1 + %2;
   %0 = %1 + %2;
   %0 = %1 + %2;
   %0 = %1 + %2;
   %0 = %1 + %2;"
  [(set_attr "type" "operate")])

(define_insn "subsi3"
  [(set (match_operand:SI 0 "register_operand" "=r,r,r,r,h,h,h")
        (minus:SI (match_operand:SI 1 "quatro_arith_lit9_operand" "r,r,r,L,r,r,L")
                  (match_operand:SI 2 "quatro_arith_lit9_operand" "r,L,h,r,h,r,r")))]
     ""
"*return quatro_sub_handle(operands);"
[(set_attr "type" "operate")])

(define_insn "mulsi3"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(mult:SI (match_operand:SI 1 "register_operand" "r,r")
		 (match_operand:SI 2 "quatro_arith_lit9_operand" "r,L")))]
  ""
     "@
     %0 = %1 * %2;
     %0 = %1 * %2;"
[(set_attr "type" "operate")])

(define_insn "negsi2"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(neg:SI (match_operand:SI 1 "register_operand" "0")))]
  ""
  "%0 = %0 * -1;")



; (define_insn "extendhisi2"
;   [(set (match_operand:SI 0 "register_operand" "=r")
; 	(sign_extend:SI (match_operand:HI 1 "register_operand" "0")))]
;   ""
;   "sexth	%0")


; (define_insn "extendqisi2"
;   [(set (match_operand:SI 0 "register_operand" "=r")
; 	(sign_extend:SI (match_operand:QI 1 "register_operand" "0")))]
;   ""
;   "sextb	%0")

; (define_insn "extendqihi2"
;   [(set (match_operand:HI 0 "register_operand" "=r")
; 	(sign_extend:HI (match_operand:QI 1 "register_operand" "0")))]
;   ""
;   "sextb	%0")

;;(define_insn "*movesi_mem"
;;  [(set (match_operand 0 "register_operand" "=r")
;;	(plus (mult (match_operand 1 "register_operand" "r")
;;		    (match_operand 2 "const_int_operand" "i"))
;;	      (match_operand 3 "address_register_operand" "a")))]
;;  ""
;;  "*
;;   {
;;   /*
;;      if (GET_MODE (operands[1] == SImode))
;;          output_asm_insn (\"%0 = MEML(%1 + %2);\", operands);
;;      else if (GET_MODE (operands[2] == HImode))
;;          output_asm_insn (\"%0 = MEMW(%1 + %2);\", operands);
;;      else if (GET_MODE (operands[3] == QImode))
;;          output_asm_insn (\"%0 = MEMB(%1 + %2);\", operands);
;;		    */
;;   }
;;  ")

(define_expand "movsi"
  [(set (match_operand:SI 0 "general_operand" "")
	(match_operand:SI 1 "general_operand"     ""))]
  ""
  "{  
     if (!no_new_pseudos && GET_CODE (operands[1]) == CONST_INT)
     {
       /* allocate a temporary destination */
       rtx temp_reg = gen_reg_rtx (SImode);
       
       if (-32768 > INTVAL (operands[1]) || 32768 <= INTVAL (operands[1]))
       {
         rtx reg_LIT_HIGH = gen_rtx_REG (SImode, LIT_HIGH);
         emit_insn (gen_unspec_specreg (reg_LIT_HIGH, GEN_INT (INTVAL (operands[1]) >> 16)));
	 emit_insn (gen_unspec_add (temp_reg, reg_LIT_HIGH, GEN_INT (INTVAL (operands[1]) & 0xFFFF)));
	 operands[1] = temp_reg;
       }
     }
     if (GET_CODE (operands[0]) == MEM && !REG_P (operands[1]))
       operands[1] = force_reg (SImode, operands[1]);
   }")

(define_insn ""
  [(set (match_operand:SI 0 "register_operand" "")
        (match_operand:SI 1 "quatro_config_operand" ""))
  ]
  ""
  "* return quatro_output_movsi_specreg(operands);"
   [(set_attr "type" "move")])

(define_insn ""
  [(set (match_operand:SI 0 "quatro_config_operand" "")
        (match_operand:SI 1 "general_operand" ""))
  ]
  ""
  "* return quatro_output_movsi_specreg(operands);"
   [(set_attr "type" "move")])

(define_insn ""
  [(set (match_operand:SI 0 "general_operand" "=r,r,r,r,m,a,h,r")
        (match_operand:SI 1 "general_operand"  "r,m,i,a,r,r,ir,h"))
  ]
  "(register_operand (operands[0], SImode)
       || register_operand (operands[1], SImode))"
  "* return quatro_output_movsi(operands);"
   [(set_attr "type" "move,load,move,move,store,move,move,move")])

(define_expand "movhi"
  [(set (match_operand:HI 0 "general_operand" "")
	(match_operand:HI 1 "general_operand"     ""))]
  ""
  "{  
       if (GET_CODE (operands[0]) == MEM)
           operands[1] = force_reg (HImode, operands[1]);
           
   }")


(define_insn ""
  [(set (match_operand:HI 0 "general_operand" "=r,r,r,r,m,a,q,r,h,r")
        (match_operand:HI 1 "general_operand"  "r,m,i,a,r,r,ir,q,ir,h"))]
  "(register_operand (operands[0], HImode)
       || register_operand (operands[1], HImode))"
  "* return quatro_output_move(insn, operands, HImode);"
   [(set_attr "type" "move,load,move,move,store,move,move,move,move,move")])



(define_expand "movqi"
  [(set (match_operand:QI 0 "general_operand" "")
	(match_operand:QI 1 "general_operand"     ""))]
  ""
  "{  
       if (GET_CODE (operands[0]) == MEM)
           operands[1] = force_reg (QImode, operands[1]);
           
   }")



(define_insn ""
  [(set (match_operand:QI 0 "general_operand" "=r,r,r,r,m,a,q,r,h,r")
        (match_operand:QI 1 "general_operand"  "r,m,i,a,r,r,ir,q,ir,h"))]
  "(register_operand (operands[0], QImode)
       || register_operand (operands[1], QImode))"
  "* return quatro_output_move(insn, operands, QImode);"
   [(set_attr "type" "move,load,move,move,store,move,move,move,move,move")])



(define_expand "movsf"
  [(set (match_operand:SF 0 "general_operand" "")
	(match_operand:SF 1 "general_operand" ""))]
  ""
  "
{
  if (GET_CODE (operands[0]) == MEM)
    operands[1] = force_reg (SFmode, operands[1]);
}")

;(define_insn ""
;  [(set (match_operand:SF 0 "general_operand" "=r,r,m")
;	(match_operand:SF 1 "general_operand"  "r,m,r"))]
;  ""
;  "@
;	%0 = %1;
;    %0.a = MEML(%1);
;	MEML(%0) = %1.a;"
;   [(set_attr "type" "move,load,store")])

(define_insn ""
  [(set (match_operand:SF 0 "general_operand" "=r,r,m,r,a")
	(match_operand:SF 1 "general_operand"  "rF,m,r,a,r"))]
  "(register_operand (operands[0], SFmode) || register_operand (operands[1], SFmode))"
  "* return quatro_output_movsf (insn, operands, SFmode);"
   [(set_attr "type" "move,load,store,load,store")])

(define_insn "indirect_jump"
  [(set (pc) (match_operand 0 "register_operand" "r"))]
  ""
  "*return output_jump (operands);"
   [(set_attr "type" "branch")])

  
(define_insn "jump"
  [(set (pc)
        (label_ref (match_operand 0 "" "")))]
  ""
  "*return output_jump (operands);"
   [(set_attr "type" "branch")])

(define_expand "call"
  ;; operands[1] is stack_size_rtx
  ;; operands[2] is next_arg_register
  [(call (match_operand:SI 0 "call_operand" "")
		    (match_operand 1 "" ""))]

  ""
  "")

(define_insn "*call_via_reg"
  [(call (mem:SI (match_operand:SI 0 "register_operand" "r"))
	 (match_operand 1 "" ""))]
  ""
  "* return output_call_via_reg(operands);"
  [(set_attr "type" "branch")])


(define_insn "*call_via_label"
  [(call (mem:SI (match_operand:SI 0 "call_address_operand" ""))
	 (match_operand 1 "" ""))]
  ""
  "CALL(%0); #call_via_label %#"
  [(set_attr "type" "branch")])

(define_expand "call_value"
  ;; operand 2 is stack_size_rtx
  ;; operand 3 is next_arg_register
  [(set (match_operand 0 "register_operand" "=r")
		   (call (match_operand:SI 1 "call_operand" "")
			 (match_operand 2 "" "")))]
  ""
  "")

(define_insn "*call_value_via_reg"
  [(set (match_operand 0 "register_operand" "=r")
	(call (mem:SI (match_operand:SI 1 "register_operand" "r"))
	      (match_operand 2 "" "")))]
  ""
  "* return output_call_value_via_reg(operands);"
  [(set_attr "type" "branch")])

(define_insn "*call_value_via_label"
  [(set (match_operand 0 "register_operand" "=r")
	(call (mem:SI (match_operand:SI 1 "call_address_operand" ""))
	      (match_operand 2 "" "")))]
  ""
  ; The %~ is necessary in case this insn gets conditionalized and the previous
  ; insn is the cc setter.
  "CALL(%1); #*call_value_via_label %#"
  [(set_attr "type" "branch")])


; (define_insn "call"
;   [(call (match_operand:SI 0 "call_address_operand" "ri")
;          (match_operand:SI 1 "" ""))]
;          ""
;          "* return output_call(operands);"
;          [(set_attr "type" "branch")])

;  (define_insn "call_value"
;    [(set (match_operand 0 "register_operand" "=r")
;          (call (match_operand:SI 1 "call_address_operand" "ri")
;                (match_operand:SI 2 "" "")))]
;    ""
;    "* return output_call_value(operands);"
;   [(set_attr "type" "branch")])

;; -------------------------------------------------------------------------
;; UNSPEC shifts 
;; -------------------------------------------------------------------------

(define_insn "unspec_specreg"                 ;;;;;;;;;;;;;;;;;;;; to remove
  [(unspec:SI [(match_operand:SI 0 "quatro_trans_operand" "t")
	       (match_operand:SI 1 "immediate_operand" "i")] UNSPEC_SPECREG)
   (clobber (match_dup 0))
  ]
  ""
  "SPECREG(%0) = %1;"
  [(set_attr "type" "operate")])
  
(define_insn "unspec_extract"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (match_operand:SI 2 "nonmemory_operand" "ri")] UNSPEC_EXTL))
   (use (reg:SI EXT_NXT))
  ]
  ""
  "%0 = EXTL(%1,%2,32);"
  [(set_attr "type" "operate")])

(define_insn "unspec_rightextract"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (match_operand:SI 2 "register_operand" "r")] UNSPEC_EXTR))
   (use (reg:SI EXT_NXT))
  ]
  ""
  "%0 = EXTR(%1,%2) + 0;"
  [(set_attr "type" "operate")])
  

(define_insn "unspec_insert"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")] UNSPEC_INS))
   (use (reg:SI INS_IPOS))
   (use (reg:SI INS_EPOS))
   (use (reg:SI INS_WIDTH))
   (use (reg:SI INS_MODE))
   (use (reg:SI LI))
  ]
  ""
  "%0 = INS(%1,LI);"
  [(set_attr "type" "operate")])

(define_insn "unspec_shiftleft"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(unspec:SI [(match_operand:SI 1 "register_operand" "r")
		    (match_operand:SI 2 "const_ok_for_shift_operand" "i")] UNSPEC_SHIFTLEFT))
  ]
  ""
  "%0 = (%1<<%2);"
  [(set_attr "type" "operate")])
  
(define_insn "unspec_add"
  [(set (match_operand:SI 0 "register_operand" "=r,h")
	(unspec:SI [(match_operand:SI 1 "quatro_trans_operand" "t,t")
		    (match_operand:SI 2 "const_ok_for_ulit16_operand" "i,i")] UNSPEC_ADD))
   (use (match_dup 1))
   (use (match_dup 2))]
  ""
  "%0.a = %1.a + %2;"
  [(set_attr "type" "operate")])

;(define_peephole
;  [(set (match_operand:SI 0 "register_operand" "=r")
;	(mult:SI (match_operand:SI 1 "register_operand" "r")
;		 (match_operand:SI 2 "register_operand" "ri")))
;   (set (match_operand:SI 3 "register_operand" "=r")
;	(plus:SI (match_operand:SI 4 "register_operand" "%r")
;		 (match_dup 0)))]
;  ""
;  "%3 = %1 * %2 + %4")

(define_insn "*mac_si_rir"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI (mult:SI (match_operand:SI 1 "register_operand" "%r")
			  (match_operand:SI 2 "const_ok_for_lit9_operand" "i"))
		 (match_operand:SI 3 "register_operand" "r")))]
  ""
  "%0 = %1 * %2 + %3; #mac operation r_simm9_r"
  [(set_attr "type" "operate")])

(define_insn "*mac_si_rri"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(plus:SI (mult:SI (match_operand:SI 1 "register_operand" "%r")
			  (match_operand:SI 2 "register_operand" "r"))
		 (match_operand:SI 3 "const_ok_for_lit9_operand" "i")))]
  ""
  "%0 = %1 * %2 + %3; #mac operation r_r_simm9"
  [(set_attr "type" "operate")])

(define_insn "*msub_si_rir"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI (mult:SI (match_operand:SI 1 "register_operand" "%r")
			  (match_operand:SI 2 "const_ok_for_lit9_operand" "i"))
		 (match_operand:SI 3 "register_operand" "r")))]
  ""
  "%0 = %1 * %2 - %3; #mult-sub operation r_simm9_r"
  [(set_attr "type" "operate")])

(define_insn "*msub_si_rri"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(minus:SI (mult:SI (match_operand:SI 1 "register_operand" "%r")
			  (match_operand:SI 2 "register_operand" "r"))
		 (match_operand:SI 3 "const_ok_for_lit9_operand" "i")))]
  ""
  "%0 = %1 * %2 - %3; #mult-sub operation r_r_simm9"
  [(set_attr "type" "operate")])

(define_insn "*mlogand_si_rir"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(and:SI (mult:SI (match_operand:SI 1 "register_operand" "%r")
			  (match_operand:SI 2 "const_ok_for_lit9_operand" "i"))
		 (match_operand:SI 3 "register_operand" "r")))]
  ""
  "%0 = %1 * %2 & %3; #mult-logical-and operation r_simm9_r"
  [(set_attr "type" "operate")])

(define_insn "*mlogand_si_rri"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(and:SI (mult:SI (match_operand:SI 1 "register_operand" "%r")
			  (match_operand:SI 2 "register_operand" "r"))
		 (match_operand:SI 3 "const_ok_for_lit9_operand" "i")))]
  ""
  "%0 = %1 * %2 & %3; #mult-logical-and operation r_r_simm9"
  [(set_attr "type" "operate")])

(define_insn "*mlogor_si_rir"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (mult:SI (match_operand:SI 1 "register_operand" "%r")
			  (match_operand:SI 2 "const_ok_for_lit9_operand" "i"))
		 (match_operand:SI 3 "register_operand" "r")))]
  ""
  "%0 = %1 * %2 | %3; #mult-logical-or operation r_simm9_r"
  [(set_attr "type" "operate")])

(define_insn "*mlogor_si_rri"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(ior:SI (mult:SI (match_operand:SI 1 "register_operand" "%r")
			  (match_operand:SI 2 "register_operand" "r"))
		 (match_operand:SI 3 "const_ok_for_lit9_operand" "i")))]
  ""
  "%0 = %1 * %2 | %3; #mult-logical-or operation r_r_simm9"
  [(set_attr "type" "operate")])

(define_insn "*mlogxor_si_rir"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(xor:SI (mult:SI (match_operand:SI 1 "register_operand" "%r")
			  (match_operand:SI 2 "const_ok_for_lit9_operand" "i"))
		 (match_operand:SI 3 "register_operand" "r")))]
  ""
  "%0 = %1 * %2 ^ %3; #mult-logical-xor operation r_simm9_r"
  [(set_attr "type" "operate")])

(define_insn "*mlogxor_si_rri"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(xor:SI (mult:SI (match_operand:SI 1 "register_operand" "%r")
			  (match_operand:SI 2 "register_operand" "r"))
		 (match_operand:SI 3 "const_ok_for_lit9_operand" "i")))]
  ""
  "%0 = %1 * %2 ^ %3; #mult-logical-xor operation r_r_simm9"
  [(set_attr "type" "operate")])

;; ------------------------------------------------------------------------
;; Misc insns
;; ------------------------------------------------------------------------


(define_insn "nop"
  [(const_int 0)]
  ""
  "NOP;")

(define_insn "tablejump"
  [(set (pc)
	(match_operand:SI 0 "register_operand" "r"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  "* return output_call_via_reg (operands);")


(define_expand "prologue"
     [(const_int 0)]
     ""
     "{ 
          quatro_expand_prolog ();
          DONE;
      }")

(define_expand "epilogue"
     [(return)]
     ""
     "{
          quatro_expand_epilog (); 
      }")


(define_insn ""
 [(return)]
 "reload_completed"
 "RETURN;  %#"
[(set_attr "type" "branch")])

