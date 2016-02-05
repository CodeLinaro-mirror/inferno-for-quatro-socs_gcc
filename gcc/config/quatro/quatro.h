/* Definitions of target machine for GNU compiler,
   for Quatro OTI-4110 processor.
   Copyright (C) 1993, 1999, 2000, 2001, 2003, 2004 Free Software Foundation, Inc.

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

#ifndef __QUATRO__H
#define __QUATRO__H

#define CPP_PREDEFINES "-D__quatro__ -Acpu=quatro -Amachine=quatro"

/* Can only count on 16 bits of availability; change to long would affect
   many architecture specific files (other architectures...).  */
extern int target_flags;

#define HARDLIT_BIT	   (1 << 0) /* Build in-line literals using 2 insns */
#define ALIGN8_BIT	   (1 << 1) /* Max alignment goes to 8 instead of 4 */
#define DIV_BIT		   (1 << 2) /* Generate divide instructions */
#define RELAX_IMM_BIT	   (1 << 3) /* Arbitrary immediates in and, or, tst */
#define W_FIELD_BIT	   (1 << 4) /* Generate bit insv/extv using SImode */
#define	OVERALIGN_FUNC_BIT (1 << 5) /* Align functions to 4 byte boundary */
#define CGDATA_BIT	   (1 << 6) /* Generate callgraph data */
#define SLOW_BYTES_BIT     (1 << 7) /* Slow byte access */
#define LITTLE_END_BIT     (1 << 8) /* Generate little endian code */
#define M340_BIT           (1 << 9) /* Generate code for the m340 */
#define DUMP_ADDRESSING_BIT           (1 << 10) /* Generate code for the m340 */
#define BIG_LMEM_BIT     (1 << 8) /* Generate little endian code */

#define TARGET_DEFAULT 0 


#ifndef MULTILIB_DEFAULTS
#define MULTILIB_DEFAULTS { "mlittle-endian", "m340" }
#endif

#define TARGET_HARDLIT		(target_flags & HARDLIT_BIT)
/* The ability to have 4 byte alignment is being suppressed for now.
   If this ability is reenabled, you must enable the definition below
   *and* edit t-mcore to enable multilibs for 4 byte alignment code.  */
#define TARGET_8ALIGN           0
#define TARGET_DIV              (target_flags & DIV_BIT)
#define TARGET_RELAX_IMM        (target_flags & RELAX_IMM_BIT)
#define TARGET_W_FIELD          (target_flags & W_FIELD_BIT)
#define TARGET_OVERALIGN_FUNC   (target_flags & OVERALIGN_FUNC_BIT)
#define TARGET_CG_DATA          (target_flags & CGDATA_BIT)
#define TARGET_CG_DATA          (target_flags & CGDATA_BIT)
#define TARGET_SLOW_BYTES       (target_flags & SLOW_BYTES_BIT)
#define TARGET_LITTLE_END       (target_flags & LITTLE_END_BIT)
#define TARGET_M340             (target_flags & M340_BIT)
#define TARGET_DUMP_ADDRESSING  (target_flags & DUMP_ADDRESSING_BIT)
#define TARGET_BIG_LMEM         (target_flags & BIG_LMEM_BIT)

#define TARGET_SWITCHES							\
{ {"hardlit", 	            HARDLIT_BIT,				\
     N_("Inline constants if it can be done in 2 insns or less") },	\
  {"no-hardlit",          - HARDLIT_BIT,				\
     N_("inline constants if it only takes 1 instruction") },		\
  {"4align",              - ALIGN8_BIT,					\
     N_("Set maximum alignment to 4") },				\
  {"8align",	            ALIGN8_BIT,					\
     N_("Set maximum alignment to 8") },				\
  {"div",                   DIV_BIT,					\
     "" },								\
  {"no-div",	          - DIV_BIT,					\
     N_("Do not use the divide instruction") },				\
  {"relax-immediates",      RELAX_IMM_BIT,				\
     "" },								\
  {"no-relax-immediates", - RELAX_IMM_BIT,				\
     N_("Do not arbitary sized immediates in bit operations") },	\
  {"wide-bitfields",        W_FIELD_BIT,				\
     N_("Always treat bitfield as int-sized") },			\
  {"no-wide-bitfields",   - W_FIELD_BIT,				\
     "" },								\
  {"4byte-functions",       OVERALIGN_FUNC_BIT,				\
     N_("Force functions to be aligned to a 4 byte boundary") },	\
  {"no-4byte-functions",  - OVERALIGN_FUNC_BIT,				\
     N_("Force functions to be aligned to a 2 byte boundary") },	\
  {"callgraph-data",        CGDATA_BIT,					\
     N_("Emit call graph information") },				\
  {"no-callgraph-data",   - CGDATA_BIT,					\
     "" },								\
  {"slow-bytes",            SLOW_BYTES_BIT,				\
     N_("Prefer word accesses over byte accesses") },			\
  {"no-slow-bytes",       - SLOW_BYTES_BIT,				\
     "" },								\
  { "no-lsim",              0, "" },			 		\
  {"little-endian",         LITTLE_END_BIT,				\
     N_("Generate little endian code") },				\
  {"big-endian",          - LITTLE_END_BIT,				\
     "" },								\
  {"big-lmem",                   BIG_LMEM_BIT,  \
     N_("Generate code for the M*Core M340") },				\
  {"dump-addressing",                   DUMP_ADDRESSING_BIT,  \
     N_("Dump addressing mode debug information.") },				\
  {"",   	            TARGET_DEFAULT,				\
     "" }								\
}

extern char * quatro_current_function_name;
 
/* Target specific options (as opposed to the switches above).  */
extern const char * quatro_stack_increment_string;
extern const char * s_quatro_address_cost_plus;
extern const char * s_quatro_address_cost_post;
extern const char * s_quatro_address_cost_reg;
extern const char * s_quatro_address_cost_const;

#define	TARGET_OPTIONS							\
{									\
  {"address-cost-plus=", & s_quatro_address_cost_plus,			\
     N_("Cost of plus address")},	\
  {"address-cost-post=", & s_quatro_address_cost_post,			\
     N_("Cost of post +/- address")},	\
  {"address-cost-reg=", & s_quatro_address_cost_reg,  \
     N_("Cost of reg address")},	\
  {"address-cost-const=", & s_quatro_address_cost_const, \
     N_("Cost of plus address")}	\
}

/* The Quatro ABI says that bitfields are unsigned by default. */
/* The EPOC C++ environment does not support exceptions.  */
#define CC1_SPEC ""

/*if (final_sequence != NULL_RTX)				\*/


#define DBR_OUTPUT_SEQEND(FILE)                             \
if (final_sequence != NULL_RTX)                             \
{                                                           \
    int filled, total_slots;                                \
    rtx fin_insn = XVECEXP (final_sequence, 0, 0);          \
    if (GET_CODE (fin_insn) == CALL_INSN                    \
     || GET_CODE (fin_insn) == JUMP_INSN)                   \
        total_slots = 3;                                    \
    else                                                    \
        total_slots = 0;                                    \
    filled = dbr_sequence_length();	                    \
    while (filled++ < total_slots)                          \
    {                                                       \
        fputs("\tNOP; #DELAY\n", FILE);                     \
    }							    \
}


/* Target machine storage Layout.  */

#undef PROMOTE_MODE
#define PROMOTE_MODE(MODE,UNSIGNEDP,TYPE)           \
    if (   GET_MODE_CLASS (MODE) == MODE_INT        \
        && GET_MODE_SIZE (MODE) < UNITS_PER_WORD)   \
    {                                               \
        UNSIGNEDP = 1;                              \
        MODE = SImode;                              \
    }

#undef PROMOTE_FUNCTION_ARGS
#define PROMOTE_FUNCTION_ARGS

#undef PROMOTE_FUNCTION_RETURN
#define PROMOTE_FUNCTION_RETURN

/* Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields.  */
#define BITS_BIG_ENDIAN  0

/* Define this if most significant byte of a word is the lowest numbered.  */
#define BYTES_BIG_ENDIAN (! TARGET_LITTLE_END)

/* Define this if most significant word of a multiword number is the lowest
   numbered.  */
#define WORDS_BIG_ENDIAN (! TARGET_LITTLE_END)

/* Define this macro if WORDS_BIG_ENDIAN is not constant. This must be a constant
   value with the same meaning as WORDS_BIG_ENDIAN, which will be used
   only when compiling ‘libgcc2.c’. Typically the value will be set based on
   preprocessor defines. */
#define LIBGCC2_WORDS_BIG_ENDIAN (! TARGET_LITTLE_END)

/* Number of bits in an addressable storage unit.  */
#define BITS_PER_UNIT  8

/* Width in bits of a "word", which is the contents of a machine register.
   Note that this is not necessarily the width of data type `int';
   if using 16-bit ints on a 68000, this would still be 32.
   But on a machine with 16-bit registers, this would be 16.  */
#define BITS_PER_WORD  32
#define MAX_BITS_PER_WORD 32

/* Width of a word, in units (bytes).  */
#define UNITS_PER_WORD	4

/* Width in bits of a pointer.
   See also the macro `Pmode' defined below.  */
#define POINTER_SIZE  32

/* A C expression for the size in bits of the type `long long' on the
   target machine.  If you don't define this, the default is two
   words.  */
#define LONG_LONG_TYPE_SIZE 64

/* the size of the boolean type -- in C++; */
#define	BOOL_TYPE_SIZE	32

/* Allocation boundary (in *bits*) for storing arguments in argument list.  */
#define PARM_BOUNDARY  	32

/* Doubles must be alogned to an 8 byte boundary.  */
#define FUNCTION_ARG_BOUNDARY(MODE, TYPE) (32)
     
/* Boundary (in *bits*) on which stack pointer should be aligned.  */
#define STACK_BOUNDARY  (32)

/* Largest increment in UNITS we allow the stack to grow in a single operation.  */
extern int quatro_stack_increment;
#define STACK_UNITS_MAXSTEP  4096

/* Allocation boundary (in *bits*) for the code of a function.  */
#define FUNCTION_BOUNDARY  (32 * 8)

/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY  32

/* No data type wants to be aligned rounder than this.  */
#define BIGGEST_ALIGNMENT  (32)

/* The best alignment to use in cases where we have a choice.  */
#define FASTEST_ALIGNMENT 32

/* Every structures size must be a multiple of 8 bits.  */
#define STRUCTURE_SIZE_BOUNDARY 8

/* Look at the fundamental type that is used for a bitfield and use 
   that to impose alignment on the enclosing structure.
   struct s {int a:8}; should have same alignment as "int", not "char".  */
#define	PCC_BITFIELD_TYPE_MATTERS	1

/* Largest integer machine mode for structures.  If undefined, the default
   is GET_MODE_SIZE(DImode).  */
#define MAX_FIXED_MODE_SIZE 32

/* Make strings word-aligned so strcpy from constants will be faster.  */
#define CONSTANT_ALIGNMENT(EXP, ALIGN)  (32)

/* Make arrays of chars word-aligned for the same reasons.  */
#define DATA_ALIGNMENT(TYPE, ALIGN)		(32)
     
/* Set this nonzero if move instructions will actually fail to work
   when given unaligned data.  */
#define STRICT_ALIGNMENT 1

/* Standard register usage.  */

#define FIRST_PSEUDO_REGISTER 48

/* Specify the registers used for certain standard purposes.
   The values of these macros are register numbers.  */


#undef PC_REGNUM /* Define this if the program counter is overloaded on a register.  */
#define STACK_POINTER_REGNUM 39 /* Register to use for pushing function arguments.  */
#define FRAME_POINTER_REGNUM 38 /* When we need FP, use r8.  */

/* The assembler's names for the registers.  RFP need not always be used as
   the Real framepointer; it can also be used as a normal general register.
   Note that the name `fp' is horribly misleading since `fp' is in fact only
   the argument-and-return-context pointer.  */
#define REGISTER_NAMES \
{ \
"R0",  "R1",  "R2",  "R3",  "R4",  "R5",  "R6",  "R7",  \
"R8",  "R9",  "R10", "R11", "R12", "R13", "R14", "R15", \
"R16", "R17", "R18", "R19", "R20", "R21", "R22", "R23", \
"R24", "R25", "R26", "R27", "R28", "R29", "R30", "R31", \
"A0",  "A1",  "A2",  "A3",  "A4",  "A5",  "A6",  "A7",  \
"LA",                                                   \
"EXT_NXT", "INS_EPOS", "INS_WIDTH", "INS_MODE", "INS_IPOS", "LI", "LIT_HIGH" \
}

/* 1 for registers that have pervasive standard uses
   and are not available for the register allocator.  */
#define FIXED_REGISTERS  \
   { 1,  0,  0,  0,  0,  0,  0,  0,  \
     0,  0,  0,  0,  0,  0,  0,  0,  \
     0,  0,  0,  0,  0,  0,  0,  0,  \
     0,  0,  0,  0,  0,  0,  0,  1,  \
     0,  0,  0,  0,  0,  0,  1,  1,  \
     1,                              \
     1,  1,  1,  1,  1,  1,  1       \
   }

/* 1 for registers not available across function calls.
   These must include the FIXED_REGISTERS and also any
   registers that can be used without being saved.
   The latter must include the registers where values are returned
   and the register where structure-value addresses are passed.
   Aside from that, you can include as many other registers as you like.  */

/* RBE: r15 {link register} not available across calls,
 *  But we don't mark it that way here... */
#define CALL_USED_REGISTERS \
   { 1,  1,  1,  1,  1,  1,  1,  1,  \
     1,  1,  1,  1,  1,  1,  1,  1,  \
     1,  1,  1,  1,  1,  1,  1,  1,  \
     1,  1,  1,  1,  1,  1,  1,  1,  \
     1,  1,  1,  1,  1,  1,  1,  1,  \
     1,                              \
     1,  1,  1,  1,  1,  1,  1       \
   }

#define CALL_REALLY_USED_REGISTERS  \
   { 1,  1,  1,  1,  1,  1,  1,  1, \
     1,  1,  1,  1,  1,  1,  1,  1, \
     1,  1,  1,  1,  1,  1,  1,  1, \
     1,  1,  1,  1,  1,  1,  1,  1, \
     1,  1,  1,  1,  1,  1,  1,  1, \
     0,                             \
     0,  0,  0,  0,  0,  0,  0      \
   }

/* Return number of consecutive hard regs needed starting at reg REGNO
   to hold something of mode MODE.
   This is ordinarily the length in words of a value of mode MODE
   but can be less for certain modes in special long registers.

   On the Quatro regs are UNITS_PER_WORD bits wide; */
#define HARD_REGNO_NREGS(REGNO, MODE)  \
   (((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD))

/* Value is 1 if hard register REGNO can hold a value of machine-mode MODE.
   We may keep double values in even registers.  */
#define HARD_REGNO_MODE_OK(REGNO, MODE)  (1)

#if 1
#define REG_ALLOC_ORDER              \
{  21, 22, 23, 24, 25, 26, 27, 28,   \
   29, 30, 15, 16, 17, 18, 19, 20,   \
   1,  2,  3,  4,  5,  6,  7,  8,	 \
   9,  10, 11, 12, 13, 14,\
   31, 32, 33, 34, 35, 36, 37, 38,   \
   39, 40, 41, 42, 43, 44, 45, 46, 47, 0 \
}
#endif

/* Value is 1 if it is a good idea to tie two pseudo registers
   when one has mode MODE1 and one has mode MODE2.
   If HARD_REGNO_MODE_OK could produce different values for MODE1 and MODE2,
   for any hard reg, then this must be 0 for correct output.  */
#define MODES_TIEABLE_P(MODE1, MODE2) \
  ((MODE1) == (MODE2) || GET_MODE_CLASS (MODE1) == GET_MODE_CLASS (MODE2))

/* Value should be nonzero if functions must have frame pointers.
   Zero means the frame pointer need not be set up (and parms may be accessed
   via the stack pointer) in functions that seem suitable.  */
#define FRAME_POINTER_REQUIRED 1

/* Base register for access to arguments of the function.  */
#define ARG_POINTER_REGNUM	FRAME_POINTER_REGNUM

/* Register in which the static-chain is passed to a function.  */
#define STATIC_CHAIN_REGNUM	0

#define INITIAL_FRAME_POINTER_OFFSET(DEPTH)	\
  (DEPTH) = (get_frame_size () + current_function_pretend_args_size		\
    + current_function_outgoing_args_size)

/* Place that structure value return address is placed.  */
#define STRUCT_VALUE 0

/* Define the classes of registers for register constraints in the
   machine description.  Also define ranges of constants.

   One of the classes must always be named ALL_REGS and include all hard regs.
   If there is more than one class, another class must be named NO_REGS
   and contain no registers.
   The name GENERAL_REGS must be the name of a class (or an alias for
   another name such as ALL_REGS).  This is the class of registers
   that is allowed by "g" or "r" in a register constraint.
   Also, registers outside this class are allocated only when
   instructions express preferences for them.

   The classes must be numbered in nondecreasing order; that is,
   a larger-numbered class must never be contained completely
   in a smaller-numbered class.

   For any two classes, it is very desirable that there be another
   class that represents their union.  */

/* The Quatro registers.*/
enum reg_class
{
  NO_REGS,
  GENERAL_REGS,
  ADDRESS_REGS,
  FREE_REGS,
  CONFIG_REGS,
  TRANS_REGS,
  ALL_REGS,
  LIM_REG_CLASSES
};

#define N_REG_CLASSES  (int) LIM_REG_CLASSES

/* Give names of register classes as strings for dump file.   */
#define REG_CLASS_NAMES \
{			\
  "NO_REGS",		\
  "GENERAL_REGS",       \
  "ADDRESS_REGS",       \
  "FREE_REGS",          \
  "CONFIG_REGS",        \
  "TRANS_REGS",         \
  "ALL_REGS"		\
}

/* Define which registers fit in which classes.
   This is an initializer for a vector of HARD_REG_SET
   of length N_REG_CLASSES.  */

/* ??? STACK_POINTER_REGNUM should be excluded from LRW_REGS.  */
#define REG_CLASS_CONTENTS \
{ 				    \
  /* NO_REGS.  */		    \
  { 0x00000000, 0x00000000 },       \
  /* GENERAL_REGS.  */		    \
  { 0xFFFFFFFF, 0x00000000 },       \
  /* ADDRESS_REGS.  */		    \
  { 0x00000000, 0x000000FF },       \
  /* FREE REGS.  */		    \
  { 0x00000000, 0x00000100 },       \
  /* CONFIG_REGS.  */               \
  { 0x00000000, 0x00007E00 },       \
  /* TRANS_REGS.  */                \
  { 0x00000000, 0x00008000 },       \
  /* ALL_REGS.  */		    \
  { 0xFFFFFFFF, 0x0000FFFF }        \
}

/* The same information, inverted:
   Return the class number of the smallest class containing
   reg number REGNO.  This could be a conditional expression
   or could index an array.  */

extern enum reg_class quatro_reg_class_from_regno(int regno);
#define REGNO_REG_CLASS(REGNO) (quatro_reg_class_from_regno(REGNO))

/* When defined, the compiler allows registers explicitly used in the
   rtl to be used as spill registers but prevents the compiler from
   extending the lifetime of these registers.  */
/*#define SMALL_REGISTER_CLASSES 1*/
 
/* The class value for index registers, and the one for base regs.  */
#define INDEX_REG_CLASS  GENERAL_REGS
#define BASE_REG_CLASS	 ADDRESS_REGS

/* Get reg_class from a letter such as appears in the machine 
   description.  */
extern enum reg_class quatro_reg_class_from_letter(char c);
#define REG_CLASS_FROM_LETTER(CHAR) quatro_reg_class_from_letter(CHAR)

#define REGISTER_SCRATCH 31
#define REGISTER_LA 40
#define REGISTER_LIT_HIGH 47

/* The letters I, J, K, L, M, N, O, and P in a register constraint string
   can be used to stand for particular ranges of immediate operands.
   This macro defines what the ranges are.
   C is the letter, and VALUE is a constant value.
   Return 1 if VALUE is in the range specified by C.
	I: loadable by movi (0..127)
	J: arithmetic operand 1..32
	K: shift operand 0..31
	L: negative arithmetic operand -1..-32
	M: powers of two, constants loadable by bgeni
	N: powers of two minus 1, constants loadable by bmaski, including -1
        O: allowed by cmov with two constants +/- 1 of each other
        P: values we will generate 'inline' -- without an 'lrw'

   Others defined for use after reload
        Q: constant 1
	R: a label
        S: 0/1/2 cleared bits out of 32	[for bclri's]
        T: 2 set bits out of 32	[for bseti's]
        U: constant 0
        xxxS: 1 cleared bit out of 32 (complement of power of 2). for bclri
        xxxT: 2 cleared bits out of 32. for pairs of bclris.  */
#define CONST_OK_FOR_I(VALUE) (((int)(VALUE)) >= 0 && ((int)(VALUE)) <= 0x7f)
#define CONST_OK_FOR_J(VALUE) (((int)(VALUE)) >  0 && ((int)(VALUE)) <= 32)
#define CONST_OK_FOR_K(VALUE) (((int)(VALUE)) >= 0 && ((int)(VALUE)) <= 31)
#define CONST_OK_FOR_M(VALUE) (exact_log2 (VALUE) >= 0)
#define CONST_OK_FOR_N(VALUE) (((int)(VALUE)) == -1 || exact_log2 ((VALUE) + 1) >= 0)
#define CONST_OK_FOR_O(VALUE) (CONST_OK_FOR_I(VALUE) || \
                               CONST_OK_FOR_M(VALUE) || \
                               CONST_OK_FOR_N(VALUE) || \
                               CONST_OK_FOR_M((int)(VALUE) - 1) || \
                               CONST_OK_FOR_N((int)(VALUE) + 1))


#define CONST_OK_FOR_MEMLI(VALUE) (((int)(VALUE)) >= -64 && ((int)(VALUE)) <= 63)

#define CONST_OK_FOR_MEMLI_MODE(MODE, VALUE) (GET_MODE_SIZE(MODE) && CONST_OK_FOR_MEMLI((VALUE/GET_MODE_SIZE(MODE))))

#define CONST_OK_FOR_LIT9(c)  (c >=-256 && c < 256)
#define CONST_OK_FOR_LIT16(c)  (c >=-32768 && c <= 32767)
#define CONST_OK_FOR_COMPLIT(c) (c > -16 && c <= 15)

#define CONST_OK_FOR_LETTER_P(VALUE, C)     \
     ((C) == 'I' ? CONST_OK_FOR_I (VALUE)   \
    : (C) == 'J' ? CONST_OK_FOR_J (VALUE)   \
    : (C) == 'L' ? CONST_OK_FOR_LIT9 (VALUE)   \
    : (C) == 'K' ? CONST_OK_FOR_K (VALUE)   \
    : (C) == 'M' ? CONST_OK_FOR_COMPLIT (VALUE)   \
    : (C) == 'N' ? CONST_OK_FOR_N (VALUE)   \
    : (C) == 'O' ? CONST_OK_FOR_O (VALUE)   \
    : 0)

/* Similar, but for floating constants, and defining letters G and H.
   Here VALUE is the CONST_DOUBLE rtx itself.  */
#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C) \
   ((C) == 'G' ? CONST_OK_FOR_I (CONST_DOUBLE_HIGH (VALUE)) \
	      && CONST_OK_FOR_I (CONST_DOUBLE_LOW (VALUE))  \
    : 0)

/* Letters in the range `Q' through `U' in a register constraint string
   may be defined in a machine-dependent fashion to stand for arbitrary
   operand types.  */

#define EXTRA_CONSTRAINT(OP, C)			(0)

/* Given an rtx X being reloaded into a reg required to be
   in class CLASS, return the class of reg to actually use.
   In general this is just CLASS; but on some machines
   in some cases it is preferable to use a more restrictive class.  */
/*enum reg_class quatro_preferred_reload_class(rtx, enum reg_class);*/

#define PREFERRED_RELOAD_CLASS(X, CLASS)   (CLASS)

#define SECONDARY_INPUT_RELOAD_CLASS(CLASS, MODE, X)                          \
   ((CLASS == ADDRESS_REGS) ? GENERAL_REGS : NO_REGS)

#define SECONDARY_OUTPUT_RELOAD_CLASS(CLASS, MODE, X)                         \
   ((CLASS == ADDRESS_REGS) ? GENERAL_REGS : NO_REGS)
   
/* Return the maximum number of consecutive registers
   needed to represent mode MODE in a register of class CLASS. 

   On Quatro this is the size of MODE in words.  */
#define CLASS_MAX_NREGS(CLASS, MODE)  \
     (ROUND_ADVANCE (GET_MODE_SIZE (MODE)))

/* Stack layout; function entry, exit and calling.  */

/* Define the number of register that can hold parameters.
   These two macros are used only in other macro definitions below.  */
#define NPARM_REGS 30
#define FIRST_PARM_REG 1
#define FIRST_RET_REG 1

/* Define this if pushing a word on the stack
   makes the stack pointer a smaller address.  */
#define STACK_GROWS_DOWNWARD  

/* Define this if the nominal address of the stack frame
   is at the high-address end of the local variables;
   that is, each additional local variable allocated
   goes at a more negative offset in the frame.  */
#define FRAME_GROWS_DOWNWARD

/* Offset within stack frame to start allocating local variables at.
   If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
   first local allocated.  Otherwise, it is the offset to the BEGINNING
   of the first local allocated.  */
#define STARTING_FRAME_OFFSET  0

/* If defined, the maximum amount of space required for outgoing arguments
   will be computed and placed into the variable
   `current_function_outgoing_args_size'.  No space will be pushed
   onto the stack for each call; instead, the function prologue should
   increase the stack frame size by this amount.  */
#define ACCUMULATE_OUTGOING_ARGS 1

/* Offset of first parameter from the argument pointer register value.  */
#define FIRST_PARM_OFFSET(FNDECL)  0

/* Value is the number of byte of arguments automatically
   popped when returning from a subroutine call.
   FUNTYPE is the data type of the function (as a tree),
   or for a library call it is an identifier node for the subroutine name.
   SIZE is the number of bytes of arguments passed on the stack.

   On the Quatro, the callee does not pop any of its arguments that were passed
   on the stack.  */
#define RETURN_POPS_ARGS(FUNDECL,FUNTYPE,SIZE) 0

/* Define how to find the value returned by a function.
   VALTYPE is the data type of the value (as a tree).
   If the precise function being called is known, FUNC is its FUNCTION_DECL;
   otherwise, FUNC is 0.  */
#define FUNCTION_VALUE(VALTYPE, FUNC)  quatro_function_value (VALTYPE, FUNC)

/* Don't default to pcc-struct-return, because gcc is the only compiler, and
   we want to retain compatibility with older gcc versions.  */
#define DEFAULT_PCC_STRUCT_RETURN 0

/* how we are going to return big values */
/*
 * #define RETURN_IN_MEMORY(TYPE) \
 *   (TYPE_MODE (TYPE) == BLKmode \
 *    || ((TREE_CODE (TYPE) == RECORD_TYPE || TREE_CODE(TYPE) == UNION_TYPE) \
 *        && !(TYPE_MODE (TYPE) == SImode \
 * 	    || (TYPE_MODE (TYPE) == BLKmode \
 * 		&& TYPE_ALIGN (TYPE) == BITS_PER_WORD \
 * 		&& int_size_in_bytes (TYPE) == UNITS_PER_WORD))))
 */ 

/* How many registers to use for struct return.  */
#define RETURN_IN_MEMORY(TYPE) quatro_return_in_memory (TYPE)

/* Define how to find the value returned by a library function
   assuming the value has mode MODE.  */
#define LIBCALL_VALUE(MODE)  gen_rtx (REG, MODE, FIRST_RET_REG)

/* 1 if N is a possible register number for a function value.
   On the Quatro, only r1 can return results.  */
#define FUNCTION_VALUE_REGNO_P(REGNO)  ((REGNO) == FIRST_RET_REG)

#define	MUST_PASS_IN_STACK(MODE,TYPE)  \
  quatro_must_pass_on_stack (MODE, TYPE)

/* 1 if N is a possible register number for function argument passing.  */
#define FUNCTION_ARG_REGNO_P(REGNO)  quatro_function_arg_regno_p(REGNO)

/* Define a data type for recording info about an argument list
   during the scan of that argument list.  This data type should
   hold all necessary information about the function itself
   and about the args processed so far, enough to enable macros
   such as FUNCTION_ARG to determine where the next arg should go.

   On Quatro, this is a single integer, which is a number of words
   of arguments scanned so far (including the invisible argument,
   if any, which holds the structure-value-address).
   Thus NARGREGS or more means all following args should go on the stack.  */
#define CUMULATIVE_ARGS  int

#define ROUND_ADVANCE(SIZE)	\
  ((SIZE + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

/* Round a register number up to a proper boundary for an arg of mode 
   MODE. 
   
   We round to an even reg for things larger than a word.  */
#define ROUND_REG(X, MODE) 			(X)


/* Initialize a variable CUM of type CUMULATIVE_ARGS
   for a call to a function whose data type is FNTYPE.
   For a library call, FNTYPE is 0.

   On Quatro, the offset always starts at 0: the first parm reg is always
   the same reg.  */
#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, INDIRECT)  \
  ((CUM) = 0)

/* Update the data in CUM to advance over an argument
   of mode MODE and data type TYPE.
   (TYPE is null for libcalls where that information may not be
   available.)  */
#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)	   \
 ((CUM) = (ROUND_REG ((CUM), (MODE))			   \
	   + ((NAMED) * quatro_num_arg_regs (MODE, TYPE))))

/* Define where to put the arguments to a function.  */
#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED) \
  quatro_function_arg (CUM, MODE, TYPE, NAMED)

/* A C expression that indicates when an argument must be passed by
   reference.  If nonzero for an argument, a copy of that argument is
   made in memory and a pointer to the argument is passed instead of
   the argument itself.  The pointer is passed in whatever way is
   appropriate for passing a pointer to that type.  */
#define FUNCTION_ARG_PASS_BY_REFERENCE(CUM, MODE, TYPE, NAMED) \
  MUST_PASS_IN_STACK (MODE, TYPE)

/* For an arg passed partly in registers and partly in memory,
   this is the number of registers used.
   For args passed entirely in registers or entirely in memory, zero.
   Any arg that starts in the first NPARM_REGS regs but won't entirely
   fit in them needs partial registers on the Quatro.  */
#define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED) \
  quatro_function_arg_partial_nregs (CUM, MODE, TYPE, NAMED)

/* Perform any needed actions needed for a function that is receiving a
   variable number of arguments.  */
#define SETUP_INCOMING_VARARGS(ASF, MODE, TYPE, PAS, ST) 

/* Call the function profiler with a given profile label.  */
#define FUNCTION_PROFILER(STREAM,LABELNO)		\
{							\
  fprintf (STREAM, "	trap	1\n");			\
  fprintf (STREAM, "	.align	5\n");			\
  fprintf (STREAM, "	.long	LP%d\n", (LABELNO));	\
}

/* EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in
   functions that have frame pointers.
   No definition is equivalent to always zero.  */
#define EXIT_IGNORE_STACK 0

/* Output assembler code for a block containing the constant parts
   of a trampoline, leaving space for the variable parts.

   On the MCore, the trapoline looks like:
   	lrw	r1,  function
     	lrw	r13, area
   	jmp	r13
   	or	r0, r0
    .literals                                                */
#define TRAMPOLINE_TEMPLATE(FILE)  		\
{						\
  fprintf ((FILE), "	.byte trampoline!");	\
}

/* Length in units of the trampoline for entering a nested function.  */
#define TRAMPOLINE_SIZE  16

/* Alignment required for a trampoline in units.  */
/* OTI-4110 TRAMPOLINE_ALIGN has been changed to TRAMPOLINE_ALIGNMENT */
#if 0
#define TRAMPOLINE_ALIGN  8
#else
#define TRAMPOLINE_ALIGNMENT 8
#endif

/* Emit RTL insns to initialize the variable parts of a trampoline.
   FNADDR is an RTX for the address of the function's pure code.
   CXT is an RTX for the static chain value for the function.  */
#define INITIALIZE_TRAMPOLINE(TRAMP, FNADDR, CXT)

/* Macros to check register numbers against specific register classes.  */

/* These assume that REGNO is a hard or pseudo reg number.
   They give nonzero only if REGNO is a hard reg of the suitable class
   or a pseudo reg currently allocated to a suitable hard reg.
   Since they use reg_renumber, they are safe only once reg_renumber
   has been allocated, which happens in local-alloc.c.  */

/* A C expression which is nonzero if register number NUM is suitable for use
   as a base register in operand addresses.  It may be either a suitable hard
   register or a pseudo register that has been allocated such a hard reg.  */
#define REGNO_OK_FOR_BASE_P(REGNO) \
    	((REGNO<40 && REGNO>=32)\
         || (reg_renumber[REGNO]<40 && reg_renumber[REGNO]>=32))

#define REGNO_OK_FOR_INDEX_P(REGNO)   \
    	((REGNO<32 && REGNO>=0)\
         || (reg_renumber[REGNO]<32 && reg_renumber[REGNO]>=0))


/* Maximum number of registers that can appear in a valid memory 
   address.  */
#define MAX_REGS_PER_ADDRESS 2

/* Recognize any constant value that is a valid address.  */
#define CONSTANT_ADDRESS_P(X)  ((GET_CODE (X) == LABEL_REF || GET_CODE (X) == SYMBOL_REF || GET_CODE (X) == CONST_INT))

#define LEGITIMATE_CONSTANT_P(X) (CONSTANT_P(X) && GET_CODE (X) != CONST)

#define LEGITIMIZE_ADDRESS(X, OLDX, MODE, WIN)			\
  do								\
    {								\
      rtx newx = quatro_legitimize_address (X, OLDX, MODE);	\
      if (newx)							\
	{							\
	  (X) = newx;						\
	  goto WIN;						\
	}							\
    }								\
  while (0)

/* The macros REG_OK_FOR..._P assume that the arg is a REG rtx
   and check its validity for a certain class.
   We have two alternate definitions for each of them.
   The usual definition accepts all pseudo regs; the other rejects
   them unless they have been allocated suitable hard regs.
   The symbol REG_OK_STRICT causes the latter definition to be used.  */

#ifndef REG_OK_STRICT

/* Nonzero if X is a hard reg that can be used as a base reg
   or if it is a pseudo reg.  */
#define REG_OK_FOR_BASE_P(X) \
    	((REGNO_REG_CLASS(REGNO(X))==BASE_REG_CLASS) \
            || (REGNO (X) >= FIRST_PSEUDO_REGISTER))

/* Nonzero if X is a hard reg that can be used as an index
   or if it is a pseudo reg.  */
#define REG_OK_FOR_INDEX_P(X) \
	((REGNO_REG_CLASS(REGNO(X))==INDEX_REG_CLASS) \
        || (REGNO(X) >= FIRST_PSEUDO_REGISTER))
#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, LABEL) if(quatro_legit_addr(0,MODE, X)) goto LABEL;
#else

/* Nonzero if X is a hard reg that can be used as a base reg.  */
#define REG_OK_FOR_BASE_P(X)  (REGNO_OK_FOR_BASE_P(REGNO (X)))

/* Nonzero if X is a hard reg that can be used as an index.  */
#define REG_OK_FOR_INDEX_P(X) (REGNO_OK_FOR_INDEX_P(REGNO(X)))
                               
#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, LABEL) if(quatro_legit_addr(1,MODE, X)) goto LABEL;


#endif
/* GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression that is a
   valid memory address for an instruction.  The MODE argument is the
   machine mode for the MEM expression that wants to use this address.
*/

#define HAVE_POST_INCREMENT 1

#define HAVE_POST_DECREMENT 1

#define LEGITIMIZE_RELOAD_ADDRESS(X,MODE,OPNUM,TYPE,IND_L,WIN)                    \
do                                                                                \
{                                                                                 \
    rtx new_x = quatro_legitimize_reload_address                                  \
	    ((X), (MODE), (OPNUM), (TYPE), (IND_L));                              \
    if (new_x)	                                                                  \
    {                                                                             \
	    X = new_x;                                                            \
	    goto WIN;                                                             \
    }                                                                             \
} while (0);

/* Go to LABEL if ADDR (a legitimate address expression)
   has an effect that depends on the machine mode it is used for.  */


#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL) do{if(GET_CODE (ADDR) == PLUS || GET_CODE (ADDR) == POST_INC || GET_CODE (ADDR) == POST_DEC) goto LABEL; }while(0)


#if 0
do{if(GET_CODE (ADDR) == MINUS || GET_CODE (ADDR) == PLUS || GET_CODE (ADDR) == POST_INC || GET_CODE (ADDR) == POST_DEC) goto LABEL; }while(0)
#endif

/* Specify the machine mode that this machine uses
   for the index in the tablejump instruction.  */
#define CASE_VECTOR_MODE SImode

/* 'char' is signed by default.  */
#define DEFAULT_SIGNED_CHAR  0

/* The type of size_t unsigned int.  */
#define SIZE_TYPE "unsigned int"

/* Don't cse the address of the function being compiled.  */
#define NO_RECURSIVE_FUNCTION_CSE 1

/* Max number of bytes we can move from memory to memory
   in one reasonably fast instruction.  */
#define MOVE_MAX 4

/* Define if operations between registers always perform the operation
   on the full register even if a narrower mode is specified.  */
#define WORD_REGISTER_OPERATIONS

/* Define if loading in MODE, an integral mode narrower than BITS_PER_WORD
   will either zero-extend or sign-extend.  The value of this macro should
   be the code that says which one of the two operations is implicitly
   done, NIL if none.  */
#define LOAD_EXTEND_OP(MODE) ZERO_EXTEND

/* Nonzero if access to memory by bytes is slow and undesirable.  */
#define SLOW_BYTE_ACCESS 1

/* We assume that the store-condition-codes instructions store 0 for false
   and some other value for true.  This is the value stored for true.  */
#define STORE_FLAG_VALUE 1

/* Immediate shift counts are truncated by the output routines (or was it
   the assembler?).  Shift counts in a register are truncated by ARM.  Note
   that the native compiler puts too large (> 32) immediate shift counts
   into a register and shifts by the register, letting the ARM decide what
   to do instead of doing that itself.  */
#define SHIFT_COUNT_TRUNCATED 1

/* All integers have the same format so truncation is easy.  */
#define TRULY_NOOP_TRUNCATION(OUTPREC,INPREC)  1

/* Define this if addresses of constant functions
   shouldn't be put through pseudo regs where they can be cse'd.
   Desirable on machines where ordinary constants are expensive
   but a CALL with constant address is cheap.  */
/* why is this defined??? -- dac */
#define NO_FUNCTION_CSE 1

/* Chars and shorts should be passed as ints.  */
#define PROMOTE_PROTOTYPES 1

/* The machine modes of pointers and functions.  */
#define Pmode          SImode
#define FUNCTION_MODE  Pmode


/* The relative costs of various types of constants.  Note that cse.c defines
   REG = 1, SUBREG = 2, any node = (2 + sum of subnodes).  */

/* Provide the cost for an address calculation.	*/

#define	ADDRESS_COST(RTX)  (quatro_address_cost(RTX))

#define RTX_COSTS(X, CODE, OUTER_CODE)			\
  case DIV:						\
  case UDIV:						\
  case MOD:						\
  case UMOD:						\
    return COSTS_N_INSNS (100);				\
  case MULT:						\
    return COSTS_N_INSNS (2);				\
  case FLOAT:						\
  case FIX:						\
    return 100;

#define CONST_COSTS(RTX, CODE, OUTER_CODE)      \
  case CONST_INT:				\
    if (INTVAL(RTX) < 256 && INTVAL(RTX) >= 0)  \
	   return 0;                                \
    return (INTVAL(RTX) & ~0x7fff)? 2 : 1;	\
  case CONST: 					\
    return 10;					\
  case LABEL_REF:				\
	return 5;					\
  case SYMBOL_REF:				\
    return 10;					\
  case CONST_DOUBLE:				\
      return 20;

#define REGISTER_MOVE_COST(MODE, SRCCLASS, DSTCLASS) ((SRCCLASS==ADDRESS_REGS && DSTCLASS==ADDRESS_REGS)?4:1)

/* Compute extra cost of moving data between one register class
   and another.  All register moves are cheap.  */

#define MEMORY_MOVE_COST(M, CLASS, IN) (16)


/* Implicit library calls should use memcpy, not bcopy, etc.  */
#define TARGET_MEM_FUNCTIONS


/* Switch to the text or data segment.  */
#define TEXT_SECTION_ASM_OP  "\t.text"
#define DATA_SECTION_ASM_OP  "\t.data"


/*The debug register numbers for RVD look like PC, R1A, R1B, R1C, R1D, R2A, ...*/
#define DBX_REGISTER_NUMBER(REGNO) ((REGNO*4) + 1/*PC*/)

/* The prefix to add to user-visible assembler symbols.

   For System V Release 3 the convention is to prepend a leading
   underscore onto user-level symbol names.  */

#undef USER_LABEL_PREFIX
#define USER_LABEL_PREFIX ""

#define UCMPDI2_LIBCALL "__ucmpdi2"
#define CMPDI2_LIBCALL  "__cmpdi2"
#define NEGDI2_LIBCALL  "__negdi2"

#define INIT_TARGET_OPTABS 				\
  do							\
    { 							\
      cmp_optab->handlers[(int) DImode].libfunc		\
	= init_one_libfunc (CMPDI2_LIBCALL);            \
      ucmp_optab->handlers[(int) DImode].libfunc        \
	= init_one_libfunc (UCMPDI2_LIBCALL);           \
      neg_optab->handlers[(int) DImode].libfunc		\
	= init_one_libfunc (NEGDI2_LIBCALL);		\
    }							\
  while (0)


#define REGISTER_TARGET_PRAGMAS(PFILE) do {			\
  cpp_register_pragma (PFILE, 0, "subReg", quatro_pr_reg);	\
} while (0)



 
/* This is how to output an internal numbered label where
   PREFIX is the class of label and NUM is the number within the class.

   For most svr3 systems, the convention is that any symbol which begins
   with a period is not put into the linker symbol table by the assembler.  */

#undef ASM_OUTPUT_INTERNAL_LABEL
#define ASM_OUTPUT_INTERNAL_LABEL(FILE,PREFIX,NUM)	\
  asm_fprintf (FILE, "_%s%d:\n", PREFIX, NUM)

/* This is how to store into the string LABEL
   the symbol_ref name of an internal numbered label where
   PREFIX is the class of label and NUM is the number within the class.
   This is suitable for output with `assemble_name'.

   For most svr3 systems, the convention is that any symbol which begins
   with a period is not put into the linker symbol table by the assembler.  */

#undef ASM_GENERATE_INTERNAL_LABEL
#define ASM_GENERATE_INTERNAL_LABEL(LABEL,PREFIX,NUM)	\
  sprintf (LABEL, "*_%s%d", PREFIX, NUM)

#undef ASM_OUTPUT_ADDR_VEC_ELT
#define ASM_OUTPUT_ADDR_VEC_ELT(STREAM, VALUE)           \
  quatro_output_addr_vec_elt ((STREAM), (VALUE))
	
#undef ASM_FORMAT_PRIVATE_NAME
#define ASM_FORMAT_PRIVATE_NAME(OUTPUT, NAME, LABELNO)	\
( (OUTPUT) = (char *) alloca (strlen ((NAME)) + 10),	\
  sprintf ((OUTPUT), "%s%d", (NAME), (LABELNO)))

#define ASM_LONG ".long"
#define TEXT_SECTION_ASM_OP  "\t.text"
#define DATA_SECTION_ASM_OP  "\t.data"

#define ASM_COMMENT_START "\t#"

#define ASM_APP_ON	"#inline asm begin\n"
#define ASM_APP_OFF	"#inline asm end\n"

#define FILE_ASM_OP     "\t.file\n"

#undef ASM_FILE_START
#define ASM_FILE_START(FILE)   \
  quatro_asm_file_start ((FILE), main_input_filename)

#define ASM_OUTPUT_SKIP(FILE,SIZE) \
  fprintf ((FILE), "\t.space %d\n", (SIZE))

#undef ASM_OUTPUT_COMMON
#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".comm ", (FILE)),			\
  assemble_name ((FILE), (NAME)),		\
  fprintf ((FILE), ",%u\n", (SIZE)))


#undef ASM_OUTPUT_LOCAL
#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)	\
  do {							\
    data_section ();					\
    ASM_OUTPUT_ALIGN ((FILE), 5);	\
    ASM_OUTPUT_LABEL ((FILE), (NAME));			\
    fprintf ((FILE), "\t.set .,.+%u\n", (ROUNDED));	\
  } while (0)

/* Output a label definition.  */
#define ASM_OUTPUT_LABEL(FILE,NAME)  \
  do { assemble_name (FILE, NAME); fputs (":\n", FILE); } while (0)

#define ASM_OUTPUT_ALIGN(FILE,LOG)	\
  if ((LOG) != 0)			\
    fprintf (FILE, "\t.align %d\t#ASM_OUTPUT_ALIGN\n", 1 << (LOG))

/* This is how to output an assembler line defining a `double'.  */
#define ASM_OUTPUT_DOUBLE(FILE,VALUE)			\
  do							\
    {							\
      char dstr[30];					\
      REAL_VALUE_TO_DECIMAL ((VALUE), "%.20e", dstr);	\
      fprintf (FILE, "\t.double %s\n", dstr);		\
    }							\
  while (0)


/* This is how to output an assembler line defining a `float' constant.  */
#define ASM_OUTPUT_FLOAT(FILE,VALUE)			\
  do							\
    {							\
      char dstr[30];					\
      REAL_VALUE_TO_DECIMAL ((VALUE), "%.20e", dstr);	\
      fprintf (FILE, "\t.float %s\n", dstr);		\
    }							\
  while (0)

#define ASM_OUTPUT_INT(STREAM, EXP)  	\
  (fprintf (STREAM, "\t.long\t"),      	\
   output_addr_const (STREAM, (EXP)),  	\
   fputc ('\n', STREAM))		

#define ASM_OUTPUT_SHORT(STREAM, EXP)  \
  (fprintf (STREAM, "\t.short\t"),     \
   output_addr_const (STREAM, (EXP)),  \
   fputc ('\n', STREAM))		

#define ASM_OUTPUT_CHAR(STREAM, EXP)  	\
  (fprintf (STREAM, "\t.byte\t"),      	\
   output_addr_const (STREAM, (EXP)),  	\
   fputc ('\n', STREAM))

#define ASM_OUTPUT_BYTE(STREAM, VALUE)  	\
  fprintf (STREAM, "\t.byte\t%d\n", VALUE)  	\

/* Output a globalising directive for a label.  */
#define GLOBAL_ASM_OP "\t.global\t"       

/* Target characters.  */
#define TARGET_ESC	033
#define TARGET_BELL	007
#define TARGET_BS	010
#define TARGET_TAB	011
#define TARGET_NEWLINE	012
#define TARGET_VT	013
#define TARGET_FF	014
#define TARGET_CR	015

#define PRINT_OPERAND_PUNCT_VALID_P(CODE) ((CODE) == '#')

/* Print operand X (an rtx) in assembler syntax to file FILE.
   CODE is a letter or dot (`z' in `%z0') or 0 if no letter was specified.
   For `%' followed by punctuation, CODE is the punctuation and X is null.  */
#define PRINT_OPERAND(STREAM, X, CODE)  quatro_print_operand (STREAM, X, CODE)


#define PRINT_OPERAND_ADDRESS(STREAM,X)  quatro_print_operand_address (STREAM, X)
/* Print a memory address as an operand to reference that memory location.  */

/* Support const sections and the ctors and dtors sections for g++.
   Note that there appears to be two different ways to support const
   sections at the moment.  You can either #define the symbol
   READONLY_DATA_SECTION (giving it some code which switches to the
   readonly data section) or else you can #define the symbols
   EXTRA_SECTIONS, EXTRA_SECTION_FUNCTIONS, SELECT_SECTION, and
   SELECT_RTX_SECTION.  We do both here just to be on the safe side.
   However, use of the const section is turned off by default
   unless the specific tm.h file turns it on by defining
   USE_CONST_SECTION as 1.  */

/* Define a few machine-specific details of the implementation of
   constructors.

   The __CTORS_LIST__ goes in the .init section.  Define CTOR_LIST_BEGIN
   and CTOR_LIST_END to contribute to the .init section an instruction to
   push a word containing 0 (or some equivalent of that).

   Define ASM_OUTPUT_CONSTRUCTOR to push the address of the constructor.  */

#define USE_CONST_SECTION	1

#define INIT_SECTION_ASM_OP     "\t.section\t.init"
#define FINI_SECTION_ASM_OP     "\t.section .fini"
#define CONST_SECTION_ASM_OP	"\t.section\t.data"
#define CTORS_SECTION_ASM_OP	INIT_SECTION_ASM_OP
#define DTORS_SECTION_ASM_OP    FINI_SECTION_ASM_OP

/* CTOR_LIST_BEGIN and CTOR_LIST_END are machine-dependent
   because they push on the stack.  */

#ifndef STACK_GROWS_DOWNWARD

/* Constructor list on stack is in reverse order.  Go to the end of the
   list and go backwards to call constructors in the right order.  */
#define DO_GLOBAL_CTORS_BODY					\
do {								\
  func_ptr *p, *beg = alloca (0);				\
  for (p = beg; *p; p++)					\
    ;								\
  while (p != beg)						\
    (*--p) ();							\
} while (0)

#else

/* Constructor list on stack is in correct order.  Just call them.  */
#define DO_GLOBAL_CTORS_BODY					\
do {								\
  func_ptr *p, *beg = alloca (0);				\
  for (p = beg; *p; )						\
    (*p++) ();							\
} while (0)

#endif /* STACK_GROWS_DOWNWARD */

/* Add extra sections .rodata, .init and .fini.  */

#undef EXTRA_SECTIONS
#define EXTRA_SECTIONS in_const, in_init, in_fini

#undef EXTRA_SECTION_FUNCTIONS
#define EXTRA_SECTION_FUNCTIONS					\
  CONST_SECTION_FUNCTION					\
  INIT_SECTION_FUNCTION						\
  FINI_SECTION_FUNCTION

#define INIT_SECTION_FUNCTION					\
void								\
init_section ()							\
{								\
  if (in_section != in_init)					\
    {								\
      fprintf (asm_out_file, "%s\n", INIT_SECTION_ASM_OP);	\
      in_section = in_init;					\
    }								\
}

#define FINI_SECTION_FUNCTION					\
void								\
fini_section ()							\
{								\
  if (in_section != in_fini)					\
    {								\
      fprintf (asm_out_file, "%s\n", FINI_SECTION_ASM_OP);	\
      in_section = in_fini;					\
    }								\
}

#define READONLY_DATA_SECTION() const_section ()

#define CONST_SECTION_FUNCTION						\
void									\
const_section ()							\
{									\
  if (!USE_CONST_SECTION)						\
    text_section();							\
  else if (in_section != in_const)					\
    {									\
      fprintf (asm_out_file, "%s\n", CONST_SECTION_ASM_OP);		\
      in_section = in_const;						\
    }									\
}

/* The ctors and dtors sections are not normally put into use 
   by EXTRA_SECTIONS and EXTRA_SECTION_FUNCTIONS as defined in svr3.h,
   but it can't hurt to define these macros for whatever systems use them.  */
#define CTORS_SECTION_FUNCTION						\
void									\
ctors_section ()							\
{									\
  if (in_section != in_ctors)						\
    {									\
      fprintf (asm_out_file, "%s\n", CTORS_SECTION_ASM_OP);		\
      in_section = in_ctors;						\
    }									\
}

#define DTORS_SECTION_FUNCTION						\
void									\
dtors_section ()							\
{									\
  if (in_section != in_dtors)						\
    {									\
      fprintf (asm_out_file, "%s\n", DTORS_SECTION_ASM_OP);		\
      in_section = in_dtors;						\
    }									\
}

/* A C statement or statements to switch to the appropriate
   section for output of RTX in mode MODE.  RTX is some kind
   of constant in RTL.  The argument MODE is redundant except
   in the case of a `const_int' rtx.  Currently, these always
   go into the const section.  */

#define PREDICATE_CODES                                                         \
  { "quatro_config_operand",            { REG }},                               \
  { "quatro_trans_operand",             { REG }},                               \
  { "quatro_arith_lit9_operand",        { CONST_INT, REG, SUBREG }},            \
  { "const_ok_for_ulit16_operand",      { CONST_INT }},                         \
  { "quatro_arith_complit_operand",     { CONST_INT, REG, SUBREG }},            \
  { "const_ok_for_lit9_operand",        { CONST_INT }},                         \
  { "const_ok_for_shift_operand",       { CONST_INT }},                         \
  { "quatro_non_lit9_operand",          { CONST_INT }},                         \
  { "quatro_non_lit16_operand",         { CONST_INT }},                         \
  { "address_register_operand",         { REG }},                               \
  { "call_address_operand",             { REG, SYMBOL_REF, LABEL_REF, CONST }},

#endif /* __QUATRO__H */


