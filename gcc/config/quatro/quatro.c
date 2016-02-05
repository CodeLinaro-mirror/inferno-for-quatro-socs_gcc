/* Output routines for Quatro OTI-4110 processor.
   Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004 Free Software Foundation, Inc.
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

#include "config.h"
#include "system.h"
#include "rtl.h"
#include "cpplib.h"
#include "tree.h"
#include "toplev.h"
#include "c-pragma.h"
#include "tm_p.h"
#include "assert.h"
#include "quatro.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "output.h"
#include "insn-attr.h"
#include "flags.h"
#include "obstack.h"
#include "expr.h"
#include "reload.h"
#include "recog.h"
#include "function.h"
#include "ggc.h"
#include "toplev.h"
#include "target.h"
#include "target-def.h"
#ifdef __CYGWIN__
#include <sys/cygwin.h>
#endif

/* Maximum size we are allowed to grow the stack in a single operation.
   If we want more, we must do it in increments of at most this size.
   If this value is 0, we don't check at all.  */
const char * quatro_stack_increment_string = 0;
const char * s_quatro_address_cost_plus = 0;
const char * s_quatro_address_cost_post = 0;
const char * s_quatro_address_cost_reg = 0;
const char * s_quatro_address_cost_const = 0;

int          quatro_stack_increment = STACK_UNITS_MAXSTEP;

/* For dumping information about frame sizes.  */
char * quatro_current_function_name = 0;
long   quatro_current_compilation_timestamp = 0;

/* Global variables for machine-dependent things.  */
/* Saved operands from the last compare to use in the conditional statement  */
rtx arch_compare_op0;
rtx arch_compare_op1;

/* Cached operands, and operator to compare for use in set/branch/trap
   on condition codes.  */
rtx branch_cmp[2];

/* Provides the class number of the smallest class containing
   reg number.  */
enum reg_class
quatro_reg_class_from_regno(regno)
    int regno;
{
    if (regno < 32)
        return (GENERAL_REGS);
    if (regno < 40)
        return (ADDRESS_REGS);
    if (regno < 41)
        return (FREE_REGS);
    if (regno < 47)
        return (CONFIG_REGS);
    if (regno < 48)
        return (TRANS_REGS);
    return (NO_REGS);
}

enum reg_class
quatro_reg_class_from_letter(c)
    char c;
{
    switch(c)
    {
    case 'a':
        return (ADDRESS_REGS);
    case 'h':
        return (FREE_REGS);
    case 'r':
    case 'f':
        return (GENERAL_REGS);
    case 'x':
        return (ALL_REGS);
    case 'q':
        return (CONFIG_REGS);
    case 't':
        return (TRANS_REGS);
    default:
        return (NO_REGS);
    }
}

/* structure defining the quatro frame layout  */
struct quatro_frame
{
    int arg_size;                   /* stdarg spills (bytes) */
    int reg_size;                   /* non-volatile reg saves (bytes) */
    int reg_mask;                   /* non-volatile reg saves */
    int local_size;                 /* locals */
    int outbound_size;              /* arg overflow on calls out */
    int pad_outbound;
    int pad_local;
    int pad_reg;
    /* Describe the steps we'll use to grow it.  */
#define MAX_STACK_GROWS 4           /* gives us some spare space */
    int growth[MAX_STACK_GROWS];
    int arg_offset;
    int reg_offset;
    int reg_growth;
    int local_growth;
};

static rtx              handle_structs_in_regs          PARAMS ((enum machine_mode, tree, int));
static void             quatro_mark_dllexport           PARAMS ((tree));
static void             quatro_mark_dllimport           PARAMS ((tree));
static int              quatro_dllexport_p              PARAMS ((tree));
static int              quatro_dllimport_p              PARAMS ((tree));
static const char *     quatro_strip_name_encoding      PARAMS ((const char *));
static int              quatro_calc_live_regs           PARAMS ((char used[FIRST_PSEUDO_REGISTER]));

struct gcc_target targetm = TARGET_INITIALIZER;


#undef TARGET_STRIP_NAME_ENCODING
#define TARGET_STRIP_NAME_ENCODING quatro_strip_name_encoding


static int
quatro_calc_live_regs (used)
    char used[FIRST_PSEUDO_REGISTER];
{
    int reg;
    int count = 0;
    for (reg = 0; reg < FIRST_PSEUDO_REGISTER; reg++)
    {
        if (regs_ever_live[reg] && !call_used_regs[reg])
        {
            count++;
            used[reg] = 1;
        }
        else
        {
            used[reg] = 1;
        }
    }
    return count;
}

#define _PTC_ '/'
#define _PTS_ "/"

static int GetFullPathName(path, cbfn, fullname)
	char* path; int cbfn; char* fullname;
{
	char   dir[PATH_MAX];
	char  *file, *pfn, *pd;
	int    dirl, filel;
	int    needptc = 0;
	
	if(! path) 
	{
		return 0;
	}
	if(path[0] != _PTC_)
	{
		getcwd(dir, PATH_MAX);
        /* Change any Windows "\" directory separators to Unix "/" */
        pd = strchr(dir, '\\');
        while ( pd )
          {
            *pd = _PTC_;
            pd = strchr(dir, '\\');
          }
	}
	else
	{
		dir[0] = '\0';
	}
	file = path;
	pd   = dir + strlen(dir) - 1;
	
	if(pd <= dir)
	{
		file = path;
		dir[0]  = '\0';
	}
	else if(path[0] != _PTC_)
	{
		pfn = file;
		
		while((pfn = strstr(pfn, "..")) != NULL)
		{
			pfn += 2;
			while(*pfn == _PTC_)
				pfn++;
			file = pfn;
		
			while(pd >= dir)
			{
				if(*pd == _PTC_)
				{
					*pd = '\0';
					break;
				}
				pd--;
			}
		}
	}
	else
	{
		file    = path;
		dir[0]  = '\0';
	}
	dirl  = strlen(dir);
	filel = strlen(file);
	if(file[0] != _PTC_ && (! dirl || (dir[dirl - 1] != _PTC_)))
	{
		filel++;
		needptc = TRUE;
	}
	if(dirl + filel < cbfn)
	{
		strcpy(fullname, dir);
		if(needptc)
			strcat(fullname, _PTS_);
		strcat(fullname, file);
		file = fullname;
		
		pfn = file + strlen(file) - 1;
		
		while(pfn >= file)
		{
			if(*pfn == _PTC_)
			{
				pfn++;
				break;
			}
			pfn--;
		}
	}
	return dirl + filel;
}


/* Output the file name directive with full path. */
void
quatro_asm_file_start(stream, file_name)
    FILE * stream;
    const char *file_name;
{
    char *res, full_file_name[PATH_MAX + 1];

# ifdef __CYGWIN__
    if ( cygwin_conv_to_full_win32_path (file_name, full_file_name) == 0 )
# elif defined(__MINGW32__)
    if ( GetFullPathName(file_name, PATH_MAX, full_file_name, NULL ) != 0 )
# else
    if ( realpath (file_name, full_file_name) )
# endif
    {
        res = full_file_name;
    }
    else
    {
        /* The command failed, so just use the original
         * non-absolute-path filename
         */
        fprintf (stderr, "WARNING: Can't create full-path filename - using original filename: %s\n", file_name);
        res = (char *) file_name;
    }

    /* Write the file directive */
    fprintf (stream, "\t.file\t");
    output_quoted_string (stream, res);
    fputc ('\n', stream);
}

/* Print the operand address in x to the stream.  */
void
quatro_print_operand_address (stream, x)
    FILE * stream;
    rtx x;
{
    switch (GET_CODE (x))
    {
    case REG:
        fprintf (stream, "%s.a", reg_names[REGNO (x)]);
        break;
    case POST_DEC:
        fprintf (stream, "%s.a", reg_names[REGNO (XEXP (x, 0))]);
        break;
    case POST_INC:
        fprintf (stream, "%s.a", reg_names[REGNO (XEXP (x, 0))]);
        break;
    case PLUS:
        {
            rtx base = XEXP (x, 0);
            rtx index = XEXP (x, 1);
            
            if (GET_CODE (base) != REG)
            {
                /* Ensure that BASE is a register (one of them must be).  */
                rtx temp = base;
                base = index;
                index = temp;
            }
            switch (GET_CODE (index))
            {
            case CONST_INT:
                fprintf (stream, "%s.a + %d", reg_names[REGNO(base)],
                    INTVAL (index));
                break;
            case REG:
                if(REGNO_REG_CLASS(REGNO(base))==ADDRESS_REGS)
                    fprintf (stream, "%s.a + %s.a", reg_names[REGNO(base)],
                    reg_names[REGNO(index)]);
                else
                    fprintf (stream, "%s.a + %s.a", reg_names[REGNO(index)],
                    reg_names[REGNO(base)]);
                break;
            default:
                debug_rtx (x);
                abort ();
            }
        }
        break;
    default:
        output_addr_const (stream, x);
        break;
    }
}

/* Print operand x (an rtx) in assembler syntax to file stream
   according to modifier code.

   'R'  print the next register or memory location along, ie the lsw in
        a double word value
   'O'  print a constant without the #
   'M'  print a constant as its negative
   'P'  print log2 of a power of two
   'Q'  print log2 of an inverse of a power of two
   'U'  print register for ldm/stm instruction
   'X'  print byte number for xtrbN instruction.  */
void
quatro_print_operand (stream, x, code)
     FILE * stream;
     rtx x;
     int code;
{
    REAL_VALUE_TYPE rv;
    HOST_WIDE_INT num;
    switch (code)
    {
    case 'f':
        if (GET_CODE (x) != CONST_DOUBLE)
            fatal_insn ("bad insn to quatro_print_operand, 'f' modifier:", x);
        REAL_VALUE_FROM_CONST_DOUBLE (rv, x);
        REAL_VALUE_TO_TARGET_SINGLE (rv, num);
        fprintf (stream, "%ld", num);
        break;
    case 'N':
        if (INTVAL(x) == -1)
            fprintf (stream, "32");
        else
            fprintf (stream, "%d", exact_log2 (INTVAL (x) + 1));
        break;
    case 'P':
        fprintf (stream, "%d", exact_log2 (INTVAL (x)));
        break;
    case 'Q':
        fprintf (stream, "%d", exact_log2 (~INTVAL (x)));
        break;
    case 'O':
        fprintf (stream, "%d", INTVAL (x));
        break;
    case 'U':
        fprintf (stream, "%s-%s", reg_names[REGNO (x)],
            reg_names[REGNO (x) + 3]);
        break;
    case 'x':
        fprintf (stream, "0x%x", INTVAL (x));
        break;
    case 'X':
        fprintf (stream, "%d", 3 - INTVAL (x) / 8);
        break;
    case '#':
        {
            /* To fill default delay slots for control transfers: CALL and conditional / unconditional GOTOs */
            if (NULL_RTX == final_sequence)
            {
                int slots = 0;
                int max_slots = 3;
                while (slots++ < max_slots)
                {
                    fprintf (stream, "\n\tNOP;\t\t#Fill default delay slots");
                }
            }
        }
        break;
    default:
        switch (GET_CODE (x))
        {
        case REG:
            fputs (reg_names[REGNO (x)], (stream));
            break;
        case MEM:
            output_address (XEXP (x, 0));
            break;
        default:
            output_addr_const (stream, x);
            break;
        }
        break;
    }
}

/* Acceptable arguments to the call insn.  */
int
call_address_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
    return (   symbolic_operand (op, mode)
            || (GET_CODE (op) == CONST_INT && LEGITIMATE_CONSTANT_P (op))
            || (GET_CODE (op) == REG));
}

/* Predicate returns true if the register is a valid address register.  */
int
address_register_operand (op, mode)
     rtx op;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
    return (   (GET_CODE (op) == REG)
            && REGNO_REG_CLASS (REGNO (op)) == ADDRESS_REGS);
}

int
call_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
    if (GET_CODE (op) != MEM)
        return 0;
    op = XEXP (op, 0);
    return call_address_operand (op, mode);
}

/* Predicate returns true for valid symbolic operands.  */
int
symbolic_operand (op, mode)
     rtx op;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
    switch (GET_CODE (op))
    {
    case SYMBOL_REF:
    case LABEL_REF:
    case CONST_INT:
        return 1;
    default:
        return 0;
    }
}

const char *
quatro_output_movsi(operands)
    rtx operands[];
{
    rtx dst = operands[0];
    rtx src = operands[1];
    
    if (GET_CODE (dst) == REG)
    {
        switch (GET_CODE (src))
        {
        case REG:
            if (REGNO_REG_CLASS (REGNO (dst)) == GENERAL_REGS
                && (   REGNO_REG_CLASS (REGNO (src)) == CONFIG_REGS
                || REGNO_REG_CLASS (REGNO (src)) == TRANS_REGS))
            {
                return "%0 = SPECREG(%1);";
            }
            else if ((   REGNO_REG_CLASS (REGNO (dst)) == CONFIG_REGS
                || REGNO_REG_CLASS (REGNO (dst)) == TRANS_REGS)
                &&   REGNO_REG_CLASS (REGNO (src)) == GENERAL_REGS)
            {
                return "SPECREG(%0) = %1;";
            }
            if (REGNO_REG_CLASS (REGNO (src)) == FREE_REGS)
                return "%0 = %1;";
            return "%0.a = %1.a;";                /* r-r*/
            break;
        case MEM:
            if(GET_CODE (XEXP (src, 0))==PLUS)
            {
                rtx x = XEXP (src, 0);
                rtx base = XEXP (x, 0);
                rtx index = XEXP (x, 1);
                
                if (GET_CODE (base) != REG)
                {
                    /* Ensure that BASE is a register (one of them must be).  */
                    rtx temp = base;
                    base = index;
                    index = temp;
                }
                if(GET_CODE (index)==CONST_INT)
                {
                    rtx newops[3];
                    newops[0] = dst;
                    newops[1] = base;
                    if(INTVAL(index)%4) abort();
                    newops[2] = GEN_INT(INTVAL(index)/4);
                    output_asm_insn ("%0.a = MEML(%1.a + %2); #si index mode", newops);
                    return "";
                }
                if (GET_CODE (index) == MULT)
                {
                    rtx op0, op1, newops[3];
                    op0 = XEXP (index, 0);
                    op1 = XEXP (index, 1);
                    if (GET_CODE (op0) == CONST_INT)
                    {
                        rtx temp = op0;
                        op0 = op1;
                        op1 = temp;
                    }
                    newops[0] = dst;
                    newops[1] = base;
                    newops[2] = op0;
                    output_asm_insn ("%0.a = MEML(%1.a + %2.a); #register index mode", newops);
                    return "";
                }
            }
            else if  (GET_CODE (XEXP (src, 0))==POST_INC)
            {
                return "%0.a = MEML(%1), %1+=1; #POST_INC";
            }
            else if  (GET_CODE (XEXP (src, 0))==POST_DEC)
            {
                return "%0.a = MEML(%1), %1+=-1; #POST_DEC";
            }
            return "%0.a = MEML(%1); #MEM";                 /* r-m */
            break;
            break;
        case CONST_INT:
            if (REGNO_REG_CLASS (REGNO (dst)) == CONFIG_REGS)
            {
                return "SPECREG(%0) = %1;";
            }
            return "%0.a = %1;";
            break;
        case LABEL_REF:
        case SYMBOL_REF:
        /*!!!Need to add a new type to relocs.  The new type will
        pack the high short of the symbol into LITH_HIGH and the
        low short in to the load instruction.  This works for now
        because for the most part we only ref LMEM and 16 bits is
            enough. */
            if(TARGET_BIG_LMEM)
                return "SPECREG(LIT_HIGH.a)=0;\n\t%0.a = LIT_HIGH.a + %1; # SYMBOL_REF";
            else
            {
                /* return "SPECREG(LIT_HIGH.a)=0;\n\t%0.a = LIT_HIGH.a + %1; # SYMBOL_REF"; */
                return "%0.a = %1;";
            }
            break;
        default:
            abort();
        }
    }
    else
    {
        if (GET_CODE (src) == REG)
        {
            if (GET_CODE (dst) == CONST_INT)
            {
                return "MEML(%0) = %1.a;";
            }
            if (GET_CODE (dst) == MEM)
            {
                if(GET_CODE (XEXP (dst, 0))==PLUS)
                {
                    rtx x = XEXP (dst, 0);
                    rtx base = XEXP (x, 0);
                    rtx index = XEXP (x, 1);
                    
                    if (GET_CODE (base) != REG)
                    {
                        /* Ensure that BASE is a register (one of them must be).  */
                        rtx temp = base;
                        base = index;
                        index = temp;
                    }
                    if(GET_CODE (index)==CONST_INT)
                    {
                        rtx newops[3];
                        newops[2] = src;
                        newops[0] = base;
                        newops[1] = GEN_INT(INTVAL(index)/4);
                        output_asm_insn ("MEML(%0.a + %1) = %2.a; #Indexed", newops);
                        return "";
                    }
                    if (GET_CODE (index) == MULT)
                    {
                        rtx op0, op1, newops[3];
                        op0 = XEXP (index, 0);
                        op1 = XEXP (index, 1);
                        if (GET_CODE (op0) == CONST_INT)
                        {
                            rtx temp = op0;
                            op0 = op1;
                            op1 = temp;
                        }
                        newops[0] = base;
                        newops[1] = op0;
                        newops[2] = src;
                        output_asm_insn ("MEML(%0.a + %1.a) = %2.a; #register index mode", newops);
                        return "";
                    }
                }
                else if  (GET_CODE (XEXP (dst, 0))==POST_INC)
                {
                    return "MEML(%0) = %1.a, %0+=1; #POST_INC";
                }
                else if  (GET_CODE (XEXP (dst, 0))==POST_DEC)
                {
                    return "MEML(%0) = %1.a, %0+=-1; #POST_DEC";
                }
                return "MEML(%0) = %1.a;";
            }           
        }
        else
            abort();
    }   
    return "";
}

const char *
quatro_output_movsi_specreg(operands)
    rtx operands[];
{
  rtx dst = operands[0];
  rtx src = operands[1];

  if ( GET_CODE (dst) == REG )
    {
      if ( GET_CODE (src) == REG )
        {
          if ( (REGNO_REG_CLASS (REGNO (dst)) == GENERAL_REGS) &&
               (REGNO_REG_CLASS (REGNO (src)) == CONFIG_REGS) )
            {
              return "%0 = SPECREG(%1);";
            }
          else if ( (REGNO_REG_CLASS (REGNO (dst)) == CONFIG_REGS) &&
                    (REGNO_REG_CLASS (REGNO (src)) == GENERAL_REGS) )
            {
              return "SPECREG(%0) = %1;";
            }
          else
            {
              /* We shouldn't get here */
              abort();
            }
        }
      else if ( GET_CODE (src) == CONST_INT )
        {
          if ( REGNO_REG_CLASS (REGNO (dst)) == CONFIG_REGS )
            {
                return "SPECREG(%0) = %1;";
            }
          else
            {
              /* We shouldn't get here */
              abort();
            }
        }
    }
  else
    {
      /* We shouldn't get here */
      abort();
    }

    return "";
}

const char *
quatro_output_move (insn, operands, mode)
    rtx insn ATTRIBUTE_UNUSED;
    rtx operands[];
    enum machine_mode mode;
    {
        rtx dst = operands[0];
        rtx src = operands[1];

        switch (mode)
        {
        default:
        case SImode:
            abort ();
            break;
        case HImode:
            if (GET_CODE (dst) == REG)
            {
                if (GET_CODE (src) == REG)
                {
                    if (REGNO_REG_CLASS (REGNO (dst)) == GENERAL_REGS
                        && (   REGNO_REG_CLASS (REGNO (src)) == CONFIG_REGS
                        || REGNO_REG_CLASS (REGNO (src)) == TRANS_REGS))
                    {
                        return "%0 = SPECREG(%1);";
                    }
                    else if ((   REGNO_REG_CLASS (REGNO (dst)) == CONFIG_REGS
                        || REGNO_REG_CLASS (REGNO (dst)) == TRANS_REGS)
                        &&   REGNO_REG_CLASS (REGNO (src)) == GENERAL_REGS)
                    {
                        return "SPECREG(%0) = %1;";
                    }
                    if (REGNO_REG_CLASS (REGNO (src)) == FREE_REGS)
                        return "%0 = %1;";
                    return "%0.a = %1.a;";                /* r-r*/
                }
                else if (GET_CODE (src) == MEM)
                {
                    if (GET_CODE (XEXP (src, 0)) == PLUS)
                    {
                        rtx x = XEXP (src, 0);
                        rtx base = XEXP (x, 0);
                        rtx index = XEXP (x, 1);
                        if (GET_CODE (base) != REG)
                        {
                            rtx temp = base;
                            base = index;
                            index = temp;
                        }
                        if (GET_CODE (index) == CONST_INT)
                        {
                            rtx newops[3];
                            newops[0] = dst;
                            newops[1] = base;
                            if (INTVAL (index) % 2)
                                abort();
                            newops[2] = GEN_INT (INTVAL (index) / 2);
                            output_asm_insn ("%0.a = MEMW(%1.a + %2); #hi index mode", newops);
                            return "";
                        }
                        if (GET_CODE (index) == MULT)
                        {
                            rtx op0, op1, newops[3];
                            op0 = XEXP (index, 0);
                            op1 = XEXP (index, 1);
                            if (GET_CODE (op0) == CONST_INT)
                            {
                                rtx temp = op0;
                                op0 = op1;
                                op1 = temp;
                            }
                            newops[0] = dst;
                            newops[1] = base;
                            newops[2] = op0;
                            output_asm_insn ("%0.a = MEMW(%1.a + %2.a); #register index mode", newops);
                            return "";
                        }
                    }
                    else if  (GET_CODE (XEXP (src, 0))==POST_INC)
                    {
                        return "%0.a = MEMW(%1), %1+=1; #POST_INC";
                    }
                    else if  (GET_CODE (XEXP (src, 0))==POST_DEC)
                    {
                        return "%0.a = MEMW(%1), %1+=-1; #POST_DEC";
                    }
                    if (GET_CODE (XEXP (src, 0)) == LABEL_REF)
                        abort();              /* a-R */
                    else
                        return "%0.a = MEMW(%1);";                 /* r-m */
                }
		else if (GET_CODE (src) == CONST_INT)
                {
                    return "%0.a = %1;";
                }
                else if((GET_CODE (src) == LABEL_REF) ||
                    (GET_CODE (src) == SYMBOL_REF))
                {
                    return "%0.a = MEMW(%1);";
                }
                else
                {
                    abort();
                }
            }
            else
            {
                if (GET_CODE (src) == REG)
                {
                    if (GET_CODE (dst) == CONST_INT)
                    {
                        return "MEMW(%0) = %1.a;";
                    }
                    if (GET_CODE (dst) == MEM)
                    {
                        if (GET_CODE (XEXP (dst, 0)) == PLUS)
                        {
                            rtx x = XEXP (dst, 0);
                            rtx base = XEXP (x, 0);
                            rtx index = XEXP (x, 1);
                            
                            if (GET_CODE (base) != REG)
                            {
                                rtx temp = base;
                                base = index;
                                index = temp;
                            }
                            if (GET_CODE (index) == CONST_INT)
                            {
                                rtx newops[3];
                                newops[0] = base;
                                if (INTVAL (index) % 2)
                                    abort();
                                newops[1] = GEN_INT (INTVAL (index) / 2);
                                newops[2] = src;
                                output_asm_insn ("MEMW(%0.a + %1) = %2.a; #hi index mode", newops);
                                return "";
                            }
                            if (GET_CODE (index) == MULT)
                            {
                                rtx op0, op1, newops[3];
                                op0 = XEXP (index, 0);
                                op1 = XEXP (index, 1);
                                if (GET_CODE (op0) == CONST_INT)
                                {
                                    rtx temp = op0;
                                    op0 = op1;
                                    op1 = temp;
                                }
                                newops[0] = base;
                                newops[1] = op0;
                                newops[2] = src;
                                output_asm_insn ("MEMW(%0.a + %1.a) = %2.a; #register index mode", newops);
                                return "";
                            }
                        }
                        else if  (GET_CODE (XEXP (dst, 0))==POST_INC)
                        {
                            return "MEMW(%0) = %1.a, %0+=1; #POST_INC";
                        }
                        else if  (GET_CODE (XEXP (dst, 0))==POST_DEC)
                        {
                            return "MEMW(%0) = %1.a, %0+=-1; #POST_DEC";
                        }
                        return "MEMW(%0) = %1.a;";
                    }
                }
                else
                    abort();
            }
            /* m-r */
            break;
    case QImode:
        if (GET_CODE (dst) == REG)
        {
            if (GET_CODE (src) == REG)
            {
                if (REGNO_REG_CLASS (REGNO (dst)) == GENERAL_REGS
                    && (   REGNO_REG_CLASS (REGNO (src)) == CONFIG_REGS
                    || REGNO_REG_CLASS (REGNO (src)) == TRANS_REGS))
                {
                    return "%0 = SPECREG(%1);";
                }
                else if ((   REGNO_REG_CLASS (REGNO (dst)) == CONFIG_REGS
                    || REGNO_REG_CLASS (REGNO (dst)) == TRANS_REGS)
                    &&   REGNO_REG_CLASS (REGNO (src)) == GENERAL_REGS)
                {
                    return "SPECREG(%0) = %1;";
                }
                if (REGNO_REG_CLASS (REGNO (src)) == FREE_REGS)
                    return "%0 = %1;";
                return "%0.a = %1.a;";                /* r-r*/
            }
            else if (GET_CODE (src) == MEM)
            {
                if (GET_CODE (XEXP (src, 0)) == PLUS)
                {
                    rtx x = XEXP (src, 0);
                    rtx base = XEXP (x, 0);
                    rtx index = XEXP (x, 1);
                    if (GET_CODE (base) != REG)
                    {
                        rtx temp = base;
                        base = index;
                        index = temp;
                    }
                    if (GET_CODE (index) == CONST_INT)
                    {
                        rtx newops[3];
                        newops[0] = dst;
                        newops[1] = base;
                        newops[2] = index;
                        output_asm_insn ("%0.a = MEMB(%1.a + %2); #qi index mode", newops);
                        return "";
                    }
                    if (GET_CODE (index) == MULT)
                    {
                        rtx op0, op1, newops[3];
                        op0 = XEXP (index, 0);
                        op1 = XEXP (index, 1);
                        if (GET_CODE (op0) == CONST_INT)
                        {
                            rtx temp = op0;
                            op0 = op1;
                            op1 = temp;
                        }
                        newops[0] = dst;
                        newops[1] = base;
                        newops[2] = op0;
                        output_asm_insn ("%0.a = MEMB(%1.a + %2.a); #register index mode", newops);
                        return "";
                    }
                }
                else if  (GET_CODE (XEXP (src, 0)) == POST_INC)
                {
                    return "%0.a = MEMB(%1), %1+=1; #POST_INC";
                }
                else if  (GET_CODE (XEXP (src, 0)) == POST_DEC)
                {
                    return "%0.a = MEMB(%1), %1+=-1; #POST_DEC";
                }
                if (GET_CODE (XEXP (src, 0)) == LABEL_REF)
                    abort();              /* a-R */
                else
                    return "%0.a = MEMB(%1);";                 /* r-m */
            }
            else if (GET_CODE (src) == CONST_INT)
            {
                return "%0.a = %1;";
                /*If const greater than 16 bits then break into two insn?*/
            }
            else if((GET_CODE (src) == LABEL_REF) ||
                (GET_CODE (src) == SYMBOL_REF))
            {
                return "%0.a = MEMB(%1);";
            }
            else
            {
                abort();
            }
        }
        else
        {
            if (GET_CODE (src) == REG)
            {
                if (GET_CODE (dst) == CONST_INT)
                {
                    return "MEMB(%0) = %1.a;";
                }
                if (GET_CODE (dst) == MEM)
                {
                    if (GET_CODE (XEXP (dst, 0)) == PLUS)
                    {
                        rtx x = XEXP (dst, 0);
                        rtx base = XEXP (x, 0);
                        rtx index = XEXP (x, 1);
                        
                        if (GET_CODE (base) != REG)
                        {
                            rtx temp = base;
                            base = index;
                            index = temp;
                        }
                        if (GET_CODE (index) == CONST_INT)
                        {
                            rtx newops[3];
                            newops[0] = base;
                            newops[1] = index;
                            newops[2] = src;
                            output_asm_insn ("MEMB(%0.a + %1) = %2.a; #qi index mode", newops);
                            return "";
                        }
                        if (GET_CODE (index) == MULT)
                        {
                            rtx op0, op1, newops[3];
                            op0 = XEXP (index, 0);
                            op1 = XEXP (index, 1);
                            if (GET_CODE (op0) == CONST_INT)
                            {
                                rtx temp = op0;
                                op0 = op1;
                                op1 = temp;
                            }
                            newops[0] = base;
                            newops[1] = op0;
                            newops[2] = src;
                            output_asm_insn ("MEMB(%0.a + %1.a) = %2.a; #register index mode", newops);
                            return "";
                        }
                    }
                    else if  (GET_CODE (XEXP (dst, 0))==POST_INC)
                    {
                        return "MEMB(%0) = %1.a, %0+=1; #POST_INC";
                    }
                    else if  (GET_CODE (XEXP (dst, 0))==POST_DEC)
                    {
                        return "MEMB(%0) = %1.a, %0+=-1; #POST_DEC";
                    }
                    return "MEMB(%0) = %1.a;";
                }
            }
            else
                abort();
        }
        /* m-r */
        break;
    }
    abort ();
}

const char *
quatro_output_movsf (insn, operands, mode)
    rtx insn ATTRIBUTE_UNUSED;
    rtx operands[];
    enum machine_mode mode;
{
    rtx dst = operands[0];
    rtx src = operands[1];
    
    if (GET_CODE (dst) == REG)
    {
        switch (GET_CODE (src))
        {
        case REG:
            return "%0.a = %1.a;";                /* r-r*/
            break;
        case MEM:
            if(GET_CODE (XEXP (src, 0))==PLUS)
            {
                rtx x = XEXP (src, 0);
                rtx base = XEXP (x, 0);
                rtx index = XEXP (x, 1);
                
                if (GET_CODE (base) != REG)
                {
                    /* Ensure that BASE is a register (one of them must be).  */
                    rtx temp = base;
                    base = index;
                    index = temp;
                }
                if(GET_CODE (index) == CONST_INT)
                {
                    rtx newops[3];
                    newops[0] = dst;
                    newops[1] = base;
                    if (INTVAL (index) % 4) abort ();
                    newops[2] = GEN_INT (INTVAL (index) / 4);
                    output_asm_insn ("%0.a = MEML(%1.a + %2); #sf index mode", newops);
                    return "";
                }
                if (GET_CODE (index) == MULT)
                {
                    rtx op0, op1, newops[3];
                    op0 = XEXP (index, 0);
                    op1 = XEXP (index, 1);
                    if (GET_CODE (op0) == CONST_INT)
                    {
                        rtx temp = op0;
                        op0 = op1;
                        op1 = temp;
                    }
                    newops[0] = dst;
                    newops[1] = base;
                    newops[2] = op0;
                    output_asm_insn ("%0.a = MEML(%1.a + %2.a); #register index mode", newops);
                    return "";
                }
            }
            else if  (GET_CODE (XEXP (src, 0))==POST_INC)
            {
                return "%0.a = MEML(%1), %1+=1; #POST_INC";
            }
            else if  (GET_CODE (XEXP (src, 0))==POST_DEC)
            {
                return "%0.a = MEML(%1), %1+=-1; #POST_DEC";
            }
            return "%0.a = MEML(%1); #MEM";                 /* r-m */
            break;
            break;
        case CONST_DOUBLE:
            if (REGNO_REG_CLASS (REGNO (dst)) == CONFIG_REGS)
            {
                return "SPECREG(%0) = %1;";
            }
            return "%0.a = %f1;";
            break;
        case LABEL_REF:
        case SYMBOL_REF:
        /*!!!Need to add a new type to relocs.  The new type will
        pack the high short of the symbol into LITH_HIGH and the
        low short in to the load instruction.  This works for now
        because for the most part we only ref LMEM and 16 bits is
            enough. */
            if(TARGET_BIG_LMEM)
                return "SPECREG(LIT_HIGH.a)=0;\n\t%0.a = LIT_HIGH.a + %1; # SYMBOL_REF";
            else
            {
                /* return "SPECREG(LIT_HIGH.a)=0;\n\t%0.a = LIT_HIGH.a + %1; # SYMBOL_REF"; */
                return "%0.a = %1;";
            }
            break;
        default:
            abort();
        }
    }
    else
    {
        if (GET_CODE (src) == REG)
        {
            if (GET_CODE (dst) == CONST_INT)
            {
                return "MEML(%0) = %1.a;";
            }
            if (GET_CODE (dst) == MEM)
            {
                if(GET_CODE (XEXP (dst, 0))==PLUS)
                {
                    rtx x = XEXP (dst, 0);
                    rtx base = XEXP (x, 0);
                    rtx index = XEXP (x, 1);
                    
                    if (GET_CODE (base) != REG)
                    {
                        /* Ensure that BASE is a register (one of them must be).  */
                        rtx temp = base;
                        base = index;
                        index = temp;
                    }
                    if(GET_CODE (index)==CONST_INT)
                    {
                        rtx newops[3];
                        newops[2] = src;
                        newops[0] = base;
                        newops[1] = GEN_INT(INTVAL(index)/4);
                        output_asm_insn ("MEML(%0.a + %1) = %2.a; #Indexed", newops);
                        return "";
                    }
                    if (GET_CODE (index) == MULT)
                    {
                        rtx op0, op1, newops[3];
                        op0 = XEXP (index, 0);
                        op1 = XEXP (index, 1);
                        if (GET_CODE (op0) == CONST_INT)
                        {
                            rtx temp = op0;
                            op0 = op1;
                            op1 = temp;
                        }
                        newops[0] = base;
                        newops[1] = op0;
                        newops[2] = src;
                        output_asm_insn ("MEML(%0.a + %1.a) = %2.a; #register index mode", newops);
                        return "";
                    }
                }
                else if  (GET_CODE (XEXP (dst, 0))==POST_INC)
                {
                    return "MEML(%0) = %1.a, %0+=1; #POST_INC";
                }
                else if  (GET_CODE (XEXP (dst, 0))==POST_DEC)
                {
                    return "MEML(%0) = %1.a, %0+=-1; #POST_DEC";
                }
                return "MEML(%0) = %1.a;";
            }           
        }
        else
            abort();
    }   
    return "";
}

/* Predicate returns true if the operand is a general register or  
   a valid signed 9 bit immediate value.  */
int
quatro_arith_lit9_operand (op, mode)
    rtx op;
    enum machine_mode mode;
{
    if (register_operand (op, mode))
        return 1;
    if (GET_CODE (op) == CONST_INT && CONST_OK_FOR_LIT9 (INTVAL (op)))
        return 1;
    return 0;
}

/* Predicate returns true if the operand is a general register or  
   a valid signed 16 bit immediate value.  */
int
quatro_arith_lit16_operand (op, mode)
    rtx op;
    enum machine_mode mode;
{
    if (register_operand (op, mode))
        return 1;
    if (GET_CODE (op) == CONST_INT && CONST_OK_FOR_LIT16 (INTVAL (op)))
        return 1;
    return 0;
}

/* Predicate returns true if the operand is a valid signed 9 bit immediate value.  */
int
const_ok_for_lit9_operand (op, mode)
    rtx op;
    enum machine_mode mode ATTRIBUTE_UNUSED;
{
    if (GET_CODE (op) == CONST_INT && CONST_OK_FOR_LIT9 (INTVAL (op)))
        return 1;
    return 0;
}

/* Predicate returns true if the operand is a valid unsigned 16 bit immedate value.  */
int
const_ok_for_ulit16_operand (op, mode)
    rtx op;
    enum machine_mode mode ATTRIBUTE_UNUSED;
{
    if (GET_CODE (op) == CONST_INT)
    {
        unsigned HOST_WIDE_INT uval = (unsigned HOST_WIDE_INT) INTVAL (op);
        if (65536 > uval)
            return 1;
    }
    return 0;
}

/* Predicate returns true if the oeprand is a valid memory offset for SImode.  */
int
const_ok_for_memlisi_operand (op, mode)
    rtx op;
    enum machine_mode mode ATTRIBUTE_UNUSED;
{
    if (GET_CODE (op) == CONST_INT)
    {
        int i = INTVAL(op);
        if((63*4) >= i && i >= (-64*4))
            return 1;
    }
    return 0;
}

/* Predicate returns true if the operand is a valid unsigned shift
 * immediate value (0..31).
 */
int
const_ok_for_shift_operand (op, mode)
    rtx op;
    enum machine_mode mode ATTRIBUTE_UNUSED;
{
    if (GET_CODE (op) == CONST_INT && CONST_OK_FOR_K (INTVAL (op)))
        return 1;
    return 0;
}

/* Predicate returns true if the operand is a valid memory offset.  */
int
quatro_memli_operand (op, mode)
    rtx op;
    enum machine_mode mode;
{
    if (GET_CODE (op) == CONST_INT)
    {
        int val = INTVAL (op);
        val = val / GET_MODE_SIZE (mode);
        if(CONST_OK_FOR_MEMLI(val))
            return 1;
    }
    return 0;
}

/* Predicate returns true if the operand is a valid literal for  
   comparison instrucitons.  */
int
quatro_arith_complit_operand (op, mode)
    rtx op;
    enum machine_mode mode;
{
    if (register_operand (op, mode))
        return 1;
    if (GET_CODE (op) == CONST_INT && CONST_OK_FOR_COMPLIT (INTVAL (op)))
        return 1;
    return 0;
}

/* This code is borrowed from the SH port.  */

/* The MCORE cannot load a large constant into a register, constants have to
   come from a pc relative load.  The reference of a pc relative load
   instruction must be less than 1k infront of the instruction.  This
   means that we often have to dump a constant inside a function, and
   generate code to branch around it.

   It is important to minimize this, since the branches will slow things
   down and make things bigger.

   Worst case code looks like:

   lrw   L1,r0
   br    L2
   align
   L1:   .long value
   L2:
   ..

   lrw   L3,r0
   br    L4
   align
   L3:   .long value
   L4:
   ..

   We fix this by performing a scan before scheduling, which notices which
   instructions need to have their operands fetched from the constant table
   and builds the table.

   The algorithm is:

   scan, find an instruction which needs a pcrel move.  Look forward, find the
   last barrier which is within MAX_COUNT bytes of the requirement.
   If there isn't one, make one.  Process all the instructions between
   the find and the barrier.

   In the above example, we can tell that L3 is within 1k of L1, so
   the first move can be shrunk from the 2 insn+constant sequence into
   just 1 insn, and the constant moved to L3 to make:

   lrw          L1,r0
   ..
   lrw          L3,r0
   bra          L4
   align
   L3:.long value
   L4:.long value

   Then the second move becomes the target for the shortening process.  */

typedef struct
{
    rtx value;          /* Value in table.  */
    rtx label;          /* Label of value.  */
} pool_node;

/* The maximum number of constants that can fit into one pool, since
   the pc relative range is 0...1020 bytes and constants are at least 4
   bytes long.  We subtact 4 from the range to allow for the case where
   we need to add a branch/align before the constant pool.  */

#define MAX_COUNT 1016
#define MAX_POOL_SIZE (MAX_COUNT/4)
static pool_node pool_vector[MAX_POOL_SIZE];
static int pool_size;

/*
There are three parts to the calling convention for the quatro.  Part
one will be done by the compiler.  Parts two and three will be done be
the assembly language programmer.

* Part one.  Caller.
  1. Push caller saved registers on stack.  The called procedure can
     over write registers.  If the caller wants to use any registers
     after the making a call, it must save the values before the call.
  4. Push callee arguments on the stack.  The first 31 arguments are
     passed in registers any additional arguments are passed on the
     stack.
  5. Execute CALL instruction.  CALL jumps to callee's first
     instruction and saves return address in hardware stack.

* Part two.  Callee prologue.
  3. Push the current FP on the stack.
  2. Set FP to the current SP.
  1. Allocate space on the stack for local variables.

* Part three.  Callee epilogue.
  1. Set SP to FP.  Set FP to the value stored in FP.
  2. Put return value if any into R1.a
  3. Execute RETURN instruction.
*/

/* Function to generate the prologue code for a function call.  */
void
quatro_expand_prolog ()
{
    HOST_WIDE_INT size = get_frame_size ();
    rtx scratch;
    rtx mem;
    rtx insn;
    scratch  = gen_rtx (REG, SImode, 31);
    
    if(size != 0)
    {
        /* get old frame pointer into register */
        insn = emit_insn(gen_movsi (scratch, frame_pointer_rtx));
        RTX_FRAME_RELATED_P (insn) = 1;
        /* save old frame pointer onto stack */
        mem = gen_rtx_MEM (SImode, stack_pointer_rtx);
        insn =  emit_insn(gen_movsi (mem, scratch));
        RTX_FRAME_RELATED_P (insn) = 1;
        /* move old stack pointer into frame pointer */
        /* insn = emit_insn (gen_areg_add (frame_pointer_rtx, stack_pointer_rtx, GEN_INT (0))); */
		insn = emit_insn(gen_movsi (scratch, stack_pointer_rtx));
        RTX_FRAME_RELATED_P (insn) = 1;
		insn =  emit_insn(gen_movsi (frame_pointer_rtx,scratch));
        RTX_FRAME_RELATED_P (insn) = 1;
		

        /* increase frame size to accomodate storage space for old frame pointer */
        size = size + 4;
        if (!size)
        {
            /* do nothing */
        }
        else if (size <= 64 * 16)
        {
            HOST_WIDE_INT chunk = 0;
            /* Stack size within [-64, 63]: generate LONG instruction */
            if (size % 4)
                abort();
            do
            {
                if (size > quatro_stack_increment)
                    chunk = quatro_stack_increment;
                else
                    chunk = size;
                insn = emit_insn (gen_areg_add (stack_pointer_rtx, stack_pointer_rtx, GEN_INT (-chunk)));
                RTX_FRAME_RELATED_P (insn) = 1;
                size -= chunk;           
            } while (size > 0);
        }
        else if (size < 32768)
        {
            /* Stack size within [-32768, 32767]: */
            /* generate LA=simm16;                */
            /* R=A;                               */
            /* R=R+LA;                            */
            /* A=R                                */
            rtx reg_LA = gen_rtx_REG (SImode, REGISTER_LA);
            rtx reg_scratch = gen_rtx_REG (SImode, REGISTER_SCRATCH);
            insn = emit_move_insn (reg_LA, GEN_INT (size));
            RTX_FRAME_RELATED_P (insn) = 1;
            insn = emit_move_insn (reg_scratch, stack_pointer_rtx);
            RTX_FRAME_RELATED_P (insn) = 1;
            insn = emit_insn (gen_subsi3 (reg_scratch, reg_scratch, reg_LA));
            RTX_FRAME_RELATED_P (insn) = 1;
            insn = emit_move_insn (stack_pointer_rtx, reg_scratch);
            RTX_FRAME_RELATED_P (insn) = 1;
        }
        else
        {
            /* Stack size outside [-32768, 32767]: */
            /* generate LIT_HIGH=hi;               */
            /* LA=LIT_HIGH+lo;                     */
            /* R=A;                                */
            /* R=R+LA;                             */
            /* A=R                                 */
            rtx reg_LA = gen_rtx_REG (SImode, REGISTER_LA);
            rtx reg_LIT_HIGH = gen_rtx_REG (SImode, REGISTER_LIT_HIGH);
            rtx reg_scratch = gen_rtx_REG (SImode, REGISTER_SCRATCH);
            insn = emit_insn (gen_unspec_specreg (reg_LIT_HIGH, GEN_INT (size >> 16)));
            RTX_FRAME_RELATED_P (insn) = 1;
            emit_insn (gen_unspec_add (reg_LA, reg_LIT_HIGH, GEN_INT (size & 0xFFFF)));
            RTX_FRAME_RELATED_P (insn) = 1;
            insn = emit_move_insn (reg_scratch, stack_pointer_rtx);
            RTX_FRAME_RELATED_P (insn) = 1;
            insn = emit_insn (gen_subsi3 (reg_scratch, reg_scratch, reg_LA));
            RTX_FRAME_RELATED_P (insn) = 1;
            insn = emit_move_insn (stack_pointer_rtx, reg_scratch);
            RTX_FRAME_RELATED_P (insn) = 1;
        }
    }
}

void
quatro_expand_epilog ()
{
    HOST_WIDE_INT size = get_frame_size ();
    rtx scratch;
    rtx mem;
    rtx insn;
    
    fprintf (asm_out_file, "\n# Frame Size: %d\n", size + 4);
    fprintf (asm_out_file, "# Frame pointer: %s\n\n", ((frame_pointer_needed) ? "NEEDED" : "NOT NEEDED"));
    
    if(size != 0)
    {
        /* use R31 as sratch register */
        scratch  = gen_rtx (REG, SImode, 31);
        /* restore stack pointer to current frame pointer */
        insn = emit_insn (gen_areg_add (frame_pointer_rtx, frame_pointer_rtx, GEN_INT (0)));
		RTX_FRAME_RELATED_P (insn) = 1;
		insn = emit_insn(gen_movsi (scratch, frame_pointer_rtx));
        RTX_FRAME_RELATED_P (insn) = 1;
		insn = emit_insn(gen_movsi (stack_pointer_rtx,scratch));
        RTX_FRAME_RELATED_P (insn) = 1;
        
        /* read frame pointer from stack */
        mem = gen_rtx_MEM (SImode, frame_pointer_rtx);
        insn = gen_movsi (scratch, mem);
        RTX_FRAME_RELATED_P (insn) = 1;
        emit_insn(insn);
        /* restore frame pointer */
        insn = gen_movsi (frame_pointer_rtx, scratch);
        RTX_FRAME_RELATED_P (insn) = 1;
        emit_insn(insn);
    }
}

int
quatro_must_pass_on_stack (mode, type)
     enum machine_mode mode ATTRIBUTE_UNUSED;
     tree type;
{
    if (type == NULL)
        return 0;
    /* If the argugment can have its address taken, it must
    be placed on the stack.  */
    if (TREE_ADDRESSABLE (type))
        return 1;
    return 0;
}

/* Compute the number of word sized registers needed to
   hold a function argument of mode MODE and type TYPE.  */
int
quatro_num_arg_regs (mode, type)
     enum machine_mode mode;
     tree type;
{
    int size;
    
    if (MUST_PASS_IN_STACK (mode, type))
        return 0;
    if (type && mode == BLKmode)
        size = int_size_in_bytes (type);
    else
        size = GET_MODE_SIZE (mode);
    return ROUND_ADVANCE (size);
}

static rtx
handle_structs_in_regs (mode, type, reg)
     enum machine_mode mode;
     tree type;
     int  reg;
{
    int size;
    /* A structure whoes size is not a whole multiple of bytes is passed packed into 
    registers (or spilled onto the stack if not enough registers are available)
    with the last few bytes of the structure being packed, left-justified, 
    into the last register/stack slot. GCC handles this correctly if the last 
    word is in a stack slot, but we have to generate a special, PARALLEL RTX 
    if the last word is in an argument register.  */
    if (type
        && TYPE_MODE (type) == BLKmode
        && TREE_CODE (TYPE_SIZE (type)) == INTEGER_CST
        && (size = int_size_in_bytes (type)) > UNITS_PER_WORD
        && (size % UNITS_PER_WORD != 0)
        && (reg + quatro_num_arg_regs (mode, type) <= (FIRST_PARM_REG + NPARM_REGS)))
    {
        rtx    arg_regs [NPARM_REGS];
        int    nregs;
        rtx    result;
        rtvec  rtvec;
        
        for (nregs = 0; size > 0; size -= UNITS_PER_WORD)
        {
            arg_regs [nregs] =
                gen_rtx_EXPR_LIST (SImode, gen_rtx_REG (SImode, reg ++),
                GEN_INT (nregs * UNITS_PER_WORD));
            nregs ++;
        }
        assert (ARRAY_SIZE (arg_regs) == 30);
        rtvec = gen_rtvec (nregs, arg_regs[0], arg_regs[1], arg_regs[2],
            arg_regs[3], arg_regs[4], arg_regs[5],arg_regs[6],arg_regs[7],arg_regs[8],arg_regs[9],arg_regs[10],
            arg_regs[11],arg_regs[12],arg_regs[13],arg_regs[14],arg_regs[15],arg_regs[16],arg_regs[17],arg_regs[18],
            arg_regs[19],arg_regs[20],arg_regs[21],arg_regs[22],arg_regs[23],arg_regs[24],arg_regs[25],arg_regs[26],
            arg_regs[27],arg_regs[28],arg_regs[29]);
        
        result = gen_rtx_PARALLEL (mode, rtvec);
        return result;
    }
    return gen_rtx_REG (mode, reg);
}

/* Generate an RTX which defines where to find the return value of a function.  */
rtx
quatro_function_value (valtype, func)
     tree valtype;
     tree func ATTRIBUTE_UNUSED;
{
    enum machine_mode mode;
    int unsigned_p;
    
    mode = TYPE_MODE (valtype);
    PROMOTE_MODE (mode, unsigned_p, valtype);
    return handle_structs_in_regs (mode, valtype, FIRST_RET_REG);
}

/* Define where to put the arguments to a function.
   Value is zero to push the argument on the stack,
   or a hard register in which to store the argument.

   MODE is the argument's machine mode.
   TYPE is the data type of the argument (as a tree).
    This is null for libcalls where that information may
    not be available.
   CUM is a variable of type CUMULATIVE_ARGS which gives info about
    the preceding args and about the function being called.
   NAMED is nonzero if this argument is a named parameter
    (otherwise it is an extra parameter matching an ellipsis).

   On MCore the first args are normally in registers
   and the rest are pushed.  Any arg that starts within the first
   NPARM_REGS words is at least partially passed in a register unless
   its data type forbids.  */
rtx
quatro_function_arg (cum, mode, type, named)
     CUMULATIVE_ARGS   cum;
     enum machine_mode mode;
     tree              type;
     int               named;
{
    int arg_reg;
    
    if (! named)
        return 0;
    if (MUST_PASS_IN_STACK (mode, type))
        return 0;
    arg_reg = ROUND_REG (cum, mode);
    if (arg_reg < NPARM_REGS)
        return handle_structs_in_regs (mode, type, FIRST_PARM_REG + arg_reg);
    return 0;
}

/* Implements the FUNCTION_ARG_PARTIAL_NREGS macro.
   Returns the number of argument registers required to hold *part* of
   a parameter of machine mode MODE and type TYPE (which may be NULL if
   the type is not known).  If the argument fits entirly in the argument
   registers, or entirely on the stack, then 0 is returned.  CUM is the
   number of argument registers already used by earlier parameters to
   the function.  */
int
quatro_function_arg_partial_nregs (cum, mode, type, named)
     CUMULATIVE_ARGS   cum;
     enum machine_mode mode;
     tree              type;
     int               named;
{
  int reg = ROUND_REG (cum, mode);

  if (named == 0)
    return 0;

  if (MUST_PASS_IN_STACK (mode, type))
    return 0;

  /* REG is not the *hardware* register number of the register that holds
     the argument, it is the *argument* register number.  So for example,
     the first argument to a function goes in argument register 0, which
     translates (for the MCore) into hardware register 2.  The second
     argument goes into argument register 1, which translates into hardware
     register 3, and so on.  NPARM_REGS is the number of argument registers
     supported by the target, not the maximum hardware register number of
     the target.  */
  if (reg >= NPARM_REGS)
    return 0;

  /* If the argument fits entirely in registers, return 0.  */
  if (reg + quatro_num_arg_regs (mode, type) <= NPARM_REGS)
    return 0;

  /* The argument overflows the number of available argument registers.
     Compute how many argument registers have not yet been assigned to
     hold an argument.  */
  reg = NPARM_REGS - reg;

  /* Return partially in registers and partially on the stack.  */
  return reg;
}

/* Return non-zero if SYMBOL is marked as being dllexport'd.  */
int
quatro_dllexport_name_p (symbol)
     const char * symbol;
{
  return symbol[0] == '@' && symbol[1] == 'e' && symbol[2] == '.';
}

/* Return non-zero if SYMBOL is marked as being dllimport'd.  */
int
quatro_dllimport_name_p (symbol)
     const char * symbol;
{
  return symbol[0] == '@' && symbol[1] == 'i' && symbol[2] == '.';
}

/* Mark a DECL as being dllexport'd.  */
static void
quatro_mark_dllexport (decl)
     tree decl;
{
  const char * oldname;
  char * newname;
  rtx    rtlname;
  tree   idp;

  rtlname = XEXP (DECL_RTL (decl), 0);

  if (GET_CODE (rtlname) == SYMBOL_REF)
    oldname = XSTR (rtlname, 0);
  else if (   GET_CODE (rtlname) == MEM
       && GET_CODE (XEXP (rtlname, 0)) == SYMBOL_REF)
    oldname = XSTR (XEXP (rtlname, 0), 0);
  else
    abort ();

  if (quatro_dllexport_name_p (oldname))
    return;  /* Already done.  */

  newname = alloca (strlen (oldname) + 4);
  sprintf (newname, "@e.%s", oldname);

  /* We pass newname through get_identifier to ensure it has a unique
     address.  RTL processing can sometimes peek inside the symbol ref
     and compare the string's addresses to see if two symbols are
     identical.  */
  /* ??? At least I think that's why we do this.  */
  idp = get_identifier (newname);

  XEXP (DECL_RTL (decl), 0) =
    gen_rtx (SYMBOL_REF, Pmode, IDENTIFIER_POINTER (idp));
}

/* Mark a DECL as being dllimport'd.  */
static void
quatro_mark_dllimport (decl)
     tree decl;
{
  const char * oldname;
  char * newname;
  tree   idp;
  rtx    rtlname;
  rtx    newrtl;

  rtlname = XEXP (DECL_RTL (decl), 0);

  if (GET_CODE (rtlname) == SYMBOL_REF)
    oldname = XSTR (rtlname, 0);
  else if (   GET_CODE (rtlname) == MEM
       && GET_CODE (XEXP (rtlname, 0)) == SYMBOL_REF)
    oldname = XSTR (XEXP (rtlname, 0), 0);
  else
    abort ();

  if (quatro_dllexport_name_p (oldname))
    abort (); /* This shouldn't happen.  */
  else if (quatro_dllimport_name_p (oldname))
    return; /* Already done.  */

  /* ??? One can well ask why we're making these checks here,
     and that would be a good question.  */

  /* Imported variables can't be initialized.  */
  if (TREE_CODE (decl) == VAR_DECL
      && !DECL_VIRTUAL_P (decl)
      && DECL_INITIAL (decl))
    {
      error_with_decl (decl, "initialized variable `%s' is marked dllimport");
      return;
    }

  /* `extern' needn't be specified with dllimport.
     Specify `extern' now and hope for the best.  Sigh.  */
  if (TREE_CODE (decl) == VAR_DECL
      /* ??? Is this test for vtables needed?  */
      && !DECL_VIRTUAL_P (decl))
    {
      DECL_EXTERNAL (decl) = 1;
      TREE_PUBLIC (decl) = 1;
    }

  newname = alloca (strlen (oldname) + 11);
  sprintf (newname, "@i.__imp_%s", oldname);

  /* We pass newname through get_identifier to ensure it has a unique
     address.  RTL processing can sometimes peek inside the symbol ref
     and compare the string's addresses to see if two symbols are
     identical.  */
  /* ??? At least I think that's why we do this.  */
  idp = get_identifier (newname);

  newrtl = gen_rtx (MEM, Pmode,
            gen_rtx (SYMBOL_REF, Pmode,
                 IDENTIFIER_POINTER (idp)));
  XEXP (DECL_RTL (decl), 0) = newrtl;
}

static int
quatro_dllexport_p (decl)
     tree decl;
{
    if (   TREE_CODE (decl) != VAR_DECL
        && TREE_CODE (decl) != FUNCTION_DECL)
        return 0;
    return lookup_attribute ("dllexport", DECL_ATTRIBUTES (decl)) != 0;
}

static int
quatro_dllimport_p (decl)
     tree decl;
{
    if (   TREE_CODE (decl) != VAR_DECL
        && TREE_CODE (decl) != FUNCTION_DECL)
        return 0;
    return lookup_attribute ("dllimport", DECL_ATTRIBUTES (decl)) != 0;
}

/* Cover function to implement ENCODE_SECTION_INFO.  */
void
quatro_encode_section_info (decl)
     tree decl;
{
  /* This bit is copied from arm.h.  */
  if (optimize > 0
      && TREE_CONSTANT (decl)
      && (!flag_writable_strings || TREE_CODE (decl) != STRING_CST))
    {
      rtx rtl = (TREE_CODE_CLASS (TREE_CODE (decl)) != 'd'
                 ? TREE_CST_RTL (decl) : DECL_RTL (decl));
      SYMBOL_REF_FLAG (XEXP (rtl, 0)) = 1;
    }

  /* Mark the decl so we can tell from the rtl whether the object is
     dllexport'd or dllimport'd.  */
  if (quatro_dllexport_p (decl))
    quatro_mark_dllexport (decl);
  else if (quatro_dllimport_p (decl))
    quatro_mark_dllimport (decl);

  /* It might be that DECL has already been marked as dllimport, but
     a subsequent definition nullified that.  The attribute is gone
     but DECL_RTL still has @i.__imp_foo.  We need to remove that.  */
  else if ((TREE_CODE (decl) == FUNCTION_DECL
        || TREE_CODE (decl) == VAR_DECL)
       && DECL_RTL (decl) != NULL_RTX
       && GET_CODE (DECL_RTL (decl)) == MEM
       && GET_CODE (XEXP (DECL_RTL (decl), 0)) == MEM
       && GET_CODE (XEXP (XEXP (DECL_RTL (decl), 0), 0)) == SYMBOL_REF
       && quatro_dllimport_name_p (XSTR (XEXP (XEXP (DECL_RTL (decl), 0), 0), 0)))
    {
      const char * oldname = XSTR (XEXP (XEXP (DECL_RTL (decl), 0), 0), 0);
      tree idp = get_identifier (oldname + 9);
      rtx newrtl = gen_rtx (SYMBOL_REF, Pmode, IDENTIFIER_POINTER (idp));

      XEXP (DECL_RTL (decl), 0) = newrtl;

      /* We previously set TREE_PUBLIC and DECL_EXTERNAL.
     ??? We leave these alone for now.  */
    }
}
/* OTI-4110 implementation for TARGET_STRIP_NAME_ENCODING, taken from mcore.c gcc version 3.3 */
static const char *
quatro_strip_name_encoding (str)
     const char *str;
{
  return str + (str[0] == '@' ? 3 : 0);
}

/* Cover function for UNIQUE_SECTION.  */
void
quatro_unique_section (decl, reloc)
     tree decl;
     int reloc ATTRIBUTE_UNUSED;
{
  int len;
  char * name;
  char * string;
  const char * prefix;

  name = IDENTIFIER_POINTER (DECL_ASSEMBLER_NAME (decl));

  /* Strip off any encoding in name.  */
  name = (* targetm.strip_name_encoding) (name);

  /* The object is put in, for example, section .text$fo.
     The linker will then ultimately place them in .text
     (everything from the $ on is stripped).  */
  if (TREE_CODE (decl) == FUNCTION_DECL)
    prefix = ".text$";
  /* For compatability with EPOC, we ignore the fact that the
     section might have relocs against it.  */
  /* OTI-4110 changing DECL_READONLY_SECTION to decl_readonly_section as used in mcore.c gcc version 3.3 */
  /*
  else if (DECL_READONLY_SECTION (decl, 0))
  */
  else if (decl_readonly_section (decl, 0))
    prefix = ".rdata$";
  else
    prefix = ".data$";

  len = strlen (name) + strlen (prefix);
  string = alloca (len + 1);

  sprintf (string, "%s%s", prefix, name);

  DECL_SECTION_NAME (decl) = build_string (len, string);
}

/* Predicate returns true in case of naked functions.  */
int
quatro_naked_function_p ()
{
    return lookup_attribute ("naked", DECL_ATTRIBUTES (current_function_decl)) != NULL_TREE;
}

/* How many registers to use for struct return.  */
int
quatro_return_in_memory(type)
    tree type;
{
	return (   (TYPE_MODE (type) == BLKmode && (int_size_in_bytes (type) > (2 * UNITS_PER_WORD)))
		|| TREE_CODE (type) == RECORD_TYPE);
}

/* 1 if N is a possible register number for a function value.
   On the MCore, only r4 can return results.  */
int
quatro_function_value_regno_p(regno)
    int regno;
{
    return((regno) == FIRST_RET_REG);
}

/* 1 if N is a possible register number for function argument passing.  */
int
quatro_function_arg_regno_p(regno)
    int regno;
{
    return ((regno) >= FIRST_PARM_REG && (regno) < (NPARM_REGS + FIRST_PARM_REG));
}

/* Emit the common code for doing conditional branches.
   operand[0] is the label to jump to.
   The comparison operands are saved away by cmp{si,di,sf,df}.  */
void
gen_conditional_branch (operands, test_code)
     rtx operands[];
     enum rtx_code test_code;
{
    rtx cmp0 = arch_compare_op0;
    rtx cmp1 = arch_compare_op1;
    rtx zero = const0_rtx;
    rtx result, sign_comp_res, keep_polarity_label;
    rtx keep_polarity_label_ref;
    enum machine_mode mode = SImode;
    enum rtx_code signed_test_code;
    rtx label = gen_rtx_LABEL_REF (VOIDmode, operands[0]);

    switch (GET_CODE (cmp1))
    {
    case SYMBOL_REF:
        cmp1 = force_reg (mode, cmp1);
        break;
    case CONST_INT:
        /*Force the const into a register and fall through.*/
        if(!CONST_OK_FOR_LIT9 (INTVAL (cmp1)))
            cmp1 = force_reg (mode,cmp1);
	break;
    case REG:
    case SUBREG:
	break;
    default:
	abort ();
    }

    if ( (test_code == GEU) || (test_code == GTU) ||
         (test_code == LEU) || (test_code == LTU) )
      {
        /* Must generate special case for unsigned comparison
         * since the DSP can't do unsigned comparisons directly.
         * If the two unsigned operands have the same "sign",
         * then a signed compare will produce the same result.
         * If they have different "signs", reversing the polarity
         * of the signed compare will produce the correct result.
         */

# if 0
        /* Add a tag so we know when unsigned comparisons occur */
        printf("Unsigned compare: File = %s\n", DECL_SOURCE_FILE(current_function_decl));
        printf("                  Function = %s\n", XSTR (XEXP (DECL_RTL (current_function_decl), 0), 0));
        printf("                  Line number = %d\n", lineno);
# endif

        switch ( test_code )
          {
          case GEU:
            signed_test_code = GE;
            break;
          case GTU:
            signed_test_code = GT;
            break;
          case LEU:
            signed_test_code = LE;
            break;
          case LTU:
            signed_test_code = LT;
            break;
          default:
            /* How can we possibly get here? */
            abort();
          }

        keep_polarity_label = gen_label_rtx ();
        keep_polarity_label_ref = gen_rtx_LABEL_REF (VOIDmode,
                                                     keep_polarity_label);

        /* Calculate difference between two arguments */
        result = gen_reg_rtx (mode);
        emit_insn (gen_subsi3 (result, cmp0, cmp1));

        /* XOR the two arguments to find if the two unsigned
         * values have the same "sign"
         */
        sign_comp_res = gen_reg_rtx (mode);
	    emit_insn (gen_xorsi3 (sign_comp_res, cmp0, cmp1));

        emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx,
                          gen_rtx_IF_THEN_ELSE (VOIDmode,
                            gen_rtx (GE, mode, sign_comp_res, zero),
                            keep_polarity_label_ref, pc_rtx)));

        /* Arguments have different signs, so reverse polarity
         * of the difference before testing
         */
        emit_insn (gen_mulsi3 (result, result, GEN_INT(-1)));

        emit_label(keep_polarity_label);
        emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx,
                          gen_rtx_IF_THEN_ELSE (VOIDmode,
                            gen_rtx (signed_test_code, mode, result, zero),
                            label, pc_rtx)));
      }
    else
      {
        /* allocate a pseudo to calculate the value in.  */

        if (GET_CODE (cmp1) == CONST_INT && INTVAL (cmp1) == 0)
          {
            result = cmp0;	    
          }
        else
          {
            result = gen_reg_rtx (mode);
            emit_insn (gen_subsi3 (result, cmp0, cmp1));
          }
        emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx,
                          gen_rtx_IF_THEN_ELSE (VOIDmode,
                            gen_rtx (test_code, mode, result, zero),
                            label, pc_rtx)));
      }
}

/* Generate assembly instructions for arithmetic shift right.
 * Because this is an arithmetic shift, we extend the sign bit.
 */
void
quatro_gen_ashrsi3( operands )
     rtx operands[];
{
  rtx sign;

  /* Shift the sign bit down from MSbit to LSbit */
  sign = gen_reg_rtx (SImode);
  emit_move_insn (gen_rtx_REG(SImode, EXT_NXT), GEN_INT(0));
  emit_insn (gen_unspec_extract (sign, operands[1], GEN_INT(31)));

  /* Multiply the sign bit by -1 to sign-extend and write
   * into EXT_NXT before extraction
   */
  emit_insn (gen_mulsi3 (gen_rtx_REG(SImode, EXT_NXT), sign, GEN_INT(-1)));

  if ( GET_CODE (operands[2]) == CONST_INT )
    {
      /* Shift amount is a constant */
      emit_insn (gen_unspec_extract (operands[0], operands[1], operands[2]));
    }
  else if ( GET_CODE (operands[2]) == REG )
    {
      /* Shift amount is stored in a register */
      emit_insn (gen_unspec_rightextract (operands[0], operands[1], operands[2]));
    }
  else
    {
      /* Don't know if we can get here, but just in case */
      abort();
    }
}

/* Generate assembly instructions for logical shift right.
 * Because this is a logical shift, we shift zeroes in from the left.
 */
void
quatro_gen_lshrsi3( operands )
     rtx operands[];
{
  /* Set EXT_NXT to 0 before extraction to prevent carrying
   * of sign bit
   */
  emit_move_insn (gen_rtx_REG(SImode, EXT_NXT), GEN_INT (0));

  if ( GET_CODE (operands[2]) == CONST_INT )
    {
      /* Shift amount is a constant */
      emit_insn (gen_unspec_extract (operands[0], operands[1], operands[2]));
    }
  else if ( GET_CODE (operands[2]) == REG )
    {
      /* Shift amount is stored in a register */
      emit_insn (gen_unspec_rightextract (operands[0], operands[1], operands[2]));
    }
  else
    {
      /* Don't know if we can get here, but just in case */
      abort();
    }
}

/* Generate assembly instructions for arithmetic shift left.
 */
void
quatro_gen_ashlsi3( operands )
     rtx operands[];
{
  if ( GET_CODE (operands[2]) == CONST_INT )
    {
      /* Shift amount is a constant */
      emit_insn (gen_unspec_shiftleft (operands[0], operands[1], operands[2]));
    }
  else if ( GET_CODE (operands[2]) == REG )
    {
      /* Shift amount is stored in a register */
      emit_move_insn (gen_rtx_REG(SImode, INS_IPOS), operands[2]);
      emit_move_insn (gen_rtx_REG(SImode, INS_EPOS), GEN_INT (0));
      emit_move_insn (gen_rtx_REG(SImode, INS_WIDTH), GEN_INT (32));
      emit_move_insn (gen_rtx_REG(SImode, INS_MODE), GEN_INT (0));
      emit_move_insn (gen_rtx_REG(SImode, LI), GEN_INT (0));
      emit_insn (gen_unspec_insert (operands[0], operands[1]));
    }
  else
    {
      /* Don't know if we can get here, but just in case */
      abort();
    }
}

/* Routine to print unconditional control transfers.  */
const char *
output_jump(operands)
    rtx *operands;
{
    if(REG_P (operands[0]))
    {
        output_asm_insn("#jumping via register %0", operands);
        output_asm_insn("CMEML(40) = %0.a;", operands);
        output_asm_insn("%0.a = CMEML(40);", operands);
        output_asm_insn("RETURN;", operands);
        output_asm_insn("NOP;", operands);
        output_asm_insn("NOP;", operands);
        output_asm_insn("NOP;", operands);
    }
    else
    {
        output_asm_insn("GOTO %0;#output_jump %#", operands);
    }
    return(""); 
}


const char *
output_call_value_via_reg(operands)
    rtx *operands;
{
    output_asm_insn("#calling via register %1", operands);
    output_asm_insn("CMEML(40) = %1.a;", operands);
    output_asm_insn("%1.a = CMEML(52);", operands);
    output_asm_insn("%1.a = %1.a + 20;", operands);
    output_asm_insn("CMEML(40) = %1.a;", operands);
    output_asm_insn("RETURN;", operands);
    output_asm_insn("NOP;", operands);
    output_asm_insn("NOP;", operands);
    output_asm_insn("NOP;", operands);
    return("");
}

const char *
output_call_via_reg(operands)
    rtx *operands;
{
    output_asm_insn("CMEML(40) = %0.a;", operands);
    output_asm_insn("%0.a = CMEML(52);", operands);
    output_asm_insn("%0.a = %0.a + 20;", operands);
    output_asm_insn("CMEML(40) = %0.a;", operands);
    output_asm_insn("RETURN;", operands);
    output_asm_insn("NOP;", operands);
    output_asm_insn("NOP;", operands);
    output_asm_insn("NOP;", operands);
    return("");
}

/* Nonzero if X is a hard reg that can be used as a base reg
   or if it is a pseudo reg.  */
int
reg_ok_for_base_p(X)
    rtx X;
{
    return (   (REGNO_REG_CLASS(REGNO(X)) == BASE_REG_CLASS)
            || (REGNO (X) >= FIRST_PSEUDO_REGISTER));
}

/* Nonzero if X is a hard reg that can be used as an index
   or if it is a pseudo reg.  */
int
reg_ok_for_index_p(X)
    rtx X;
{
    return (   (REGNO_REG_CLASS(REGNO(X))==INDEX_REG_CLASS)
            || (REGNO(X) >= FIRST_PSEUDO_REGISTER));
}

/* Nonzero if X is a hard reg that can be used as a base reg.  */
int
reg_ok_for_base_p_strict(X)
    rtx X;
{
    return (REGNO_OK_FOR_BASE_P(REGNO (X)));
}

/* Nonzero if X is a hard reg that can be used as an index.  */
int
reg_ok_for_index_p_strict(X)
    rtx X;
{
    return (REGNO_OK_FOR_INDEX_P(REGNO(X)));
}

/* Nonzero if X is a valid base register rtx.  */
int
is_base_register_rtx(strict, X)
    int strict;
    rtx X;
{
    if (strict)
        return ((GET_CODE (X) == REG) && reg_ok_for_base_p_strict (X));
    else
        return ((GET_CODE (X) == REG) && reg_ok_for_base_p (X));
}

/* Nonzero if X is a valid index register rtx.  */
int
is_index_register_rtx(strict, X)
    int strict;
    rtx X;
{
    if (strict)
        return ((GET_CODE (X) == REG) && reg_ok_for_index_p_strict (X));
    else
        return ((GET_CODE (X) == REG) && reg_ok_for_index_p (X));
}

/*
   GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression that is a
   valid memory address for an instruction.  The MODE argument is the
   machine mode for the MEM expression that wants to use this address.  */

int
quatro_legit_addr(strict, MODE, X)
    int strict;
    enum machine_mode MODE;
    rtx X;
{
    int legit = 0;
    
    if(GET_CODE (X) == CONST_INT && MODE == SImode)
        legit = 1;
    else  if(   (GET_CODE (X) == PLUS)
             && is_base_register_rtx (strict, XEXP(X,0))
             && is_index_register_rtx(strict,XEXP(X,1)))
        legit = 1;
    else if(   (GET_CODE (X) == PLUS)
            && is_base_register_rtx (strict,XEXP(X,1))
            && is_index_register_rtx (strict,XEXP(X,0)))
        legit = 1;
    else if(   (GET_CODE (X) == PLUS)
            && (   (is_base_register_rtx (strict,XEXP(X,0)) && GET_CODE (XEXP(X,1)) == CONST_INT)
                || (is_base_register_rtx (strict,XEXP(X,1)) && GET_CODE (XEXP(X,0)) == CONST_INT)))
    {
        int index_val = 0;
        if (GET_CODE (XEXP(X,0)) == CONST_INT)
            index_val = INTVAL (XEXP(X, 0));
        else
            index_val = INTVAL (XEXP(X, 1));
        switch (MODE)
        {
        case SImode:
	case SFmode:
            /* [-256, 255] is the legitimate range for the CONST_INT for SImode */
			/*BEGIN CHANGE 8250
              if ((-64 * 4) <= index_val && (64 * 4) > index_val && (index_val % 4 == 0))*/
			  if (index_val % 4 == 0)
			/*END CHANGE 8250*/
                legit = 1;
            break;
        case HImode:
            /* [-128, 127] is the legitimate range for the CONST_INT for SImode */
            if ((-64 * 2) <= index_val && (64 * 2) > index_val && (index_val % 2 == 0))
                legit = 1;
            break;
        case QImode:
            /* [-64, 63] is the legitimate range for the CONST_INT for SImode */
            if (-64 <= index_val && 64 > index_val)
                legit = 1;
            break;
        default:
            break;
        }
    }
    else if (GET_CODE (X) == PLUS)
    {
        rtx op0 = XEXP (X, 0);
        enum rtx_code code0 = GET_CODE (op0);
        rtx op1 = XEXP (X, 1);
        enum rtx_code code1 = GET_CODE (op1);
        rtx rtx_mult = NULL_RTX;
        HOST_WIDE_INT scale_val = 0;
        
        if (REG == code1)
        {
            op0 = XEXP (X, 1);
            code0 = GET_CODE (op0);
            op1 = XEXP (X, 0);
            code1 = GET_CODE (op1);
        }
        if (!(REG == code0 && is_base_register_rtx (strict, op0)) || MULT != code1)
        {
            legit = 0;
            return legit;
        }
        rtx_mult = op1;
        op0 = XEXP (rtx_mult, 0);
        code0 = GET_CODE (op0);
        op1 = XEXP (rtx_mult, 1);
        code1 = GET_CODE (op1);
        
        if (REG == code1)
        {
            op0 = XEXP (rtx_mult, 1);
            code0 = GET_CODE (op0);
            op1 = XEXP (rtx_mult, 0);
            code1 = GET_CODE (op1);
        }
        if (!(REG == code0 && is_index_register_rtx (strict, op0)) || CONST_INT != code1)
        {
            legit = 0;
            return legit;
        }
        scale_val = INTVAL (op1);
        if (1 != scale_val && 2 != scale_val && 4 != scale_val)
        {
            legit = 0;
            return legit;
        }
        legit = 1;
    }
    else if(   (GET_CODE (X) == POST_DEC)
            && (is_base_register_rtx(strict,XEXP (X, 0))))
        legit = 1;
    else if(   (GET_CODE (X) == POST_INC)
            && (is_base_register_rtx(strict,XEXP (X, 0))))
        legit = 1;
    else if (is_base_register_rtx(strict,(X)))
        legit = 1;
    if(TARGET_DUMP_ADDRESSING)
    {
        fprintf(stderr,"legit_addr:%s %d %d\n", GET_MODE_NAME(MODE), strict, legit);
        debug_rtx (X);
    }
    return legit;
}

/* address costs.  */
int
quatro_address_cost (addr)
     rtx addr;
{
    int cost = 8;
    switch (GET_CODE (addr))
    {
    case REG:
        cost = 8;
        break;
    case POST_INC:
    case POST_DEC:
    case PRE_INC:
    case PRE_DEC:
        cost = 1;
        break;
    case SYMBOL_REF:
        cost = 10;
        break;
    case LABEL_REF:
        cost = 2;
        break;
    case CONST:
        cost = 10;
        break;
    case CONST_INT:
        cost = 2;
        break;
    case PLUS:
        cost = 1;
        break;
    default:
        break;
    }
    if(TARGET_DUMP_ADDRESSING)
    {
        fprintf(stderr,"address_cost %d\n", cost);
        debug_rtx (addr);
    }
    return cost;
}

/* routine to handle negative immediate values in subsi3 */
const char *
quatro_sub_handle(op)
    rtx op[];
{
    rtx newops[3];
    
    newops[0] = op[0]; /* Destination */
    newops[1] = op[1]; /* Source Reg */
    newops[2] = op[2]; /* Source Reg */

    if (GET_CODE (op[2]) == REG && GET_CODE (op[1]) == REG)
    {
        return "%0 = %1 - %2;#Register Form";
    }
    if (GET_CODE(op[2]) == REG)
    {
        rtx tmp = op[1];
        op[1] = op[2];
        op[2] = tmp;
    }
    if (INTVAL(op[2]) < 0)
    {
        int index_val = INTVAL(op[2]);
        if(-63 <= index_val && -1 >= index_val)
        {
            newops[2] = GEN_INT (INTVAL (op[2]) * (-1));
            output_asm_insn ("%0 = %1 + %2; #Indexed_form1 ", newops);
            return "";
        }
        else if(-32767 <= index_val && -64 >= index_val)
        {
            newops[2] = GEN_INT(INTVAL(op[2]) * (-1));
            output_asm_insn ("LA = %2;", newops);
            output_asm_insn ("%0 = %1 + LA; #Indexed_form1 ", newops);
            return "";
        }
        else if(-32767 > index_val)
        {
            newops[2] = GEN_INT((-1 * INTVAL(op[2]))&0xFFFF);
            newops[3] = GEN_INT((-1 * INTVAL(op[2]))>>16);
            output_asm_insn ("SPECREG(LIT_HIGH) = %3;", newops);
            output_asm_insn ("LA = LIT_HIGH + %2;", newops);
            output_asm_insn ("%0 = %1 + LA;", newops);
            return "";
        }
    }
    else
    {
        output_asm_insn ("%0 = %1 - %2; #Indexed_form2", newops);
        return "";
    }
    abort();
}

/* Nonzero if operand op is a valid configuration operand required for shift instructions.  */
int
quatro_config_operand (op, mode)
    rtx op;
    enum machine_mode mode ATTRIBUTE_UNUSED;
{
    return (GET_CODE (op) == REG && REGNO_REG_CLASS (REGNO (op)) == CONFIG_REGS);
}

/* Nonzero if operand op is a valid transient operand such as LA register.  */
int
quatro_trans_operand (op, mode)
    rtx op;
    enum machine_mode mode ATTRIBUTE_UNUSED;
{
    return (GET_CODE (op) == REG && REGNO_REG_CLASS (REGNO (op)) == TRANS_REGS);
}

/* Legitimize stack addresses and convert offsets from FP into offset from SP
   when profitable.  */
rtx
quatro_legitimize_address (x, oldx, mode)
    rtx x;
    rtx oldx ATTRIBUTE_UNUSED;
    enum machine_mode mode ATTRIBUTE_UNUSED;
{
    rtx new_x = NULL_RTX;
    if (   GET_CODE (x) == PLUS
        && GET_CODE (XEXP (x, 0)) == REG
        && REGNO (XEXP (x, 0)) == FRAME_POINTER_REGNUM
        && GET_CODE (XEXP (x, 1)) == CONST_INT
        && !(CONST_OK_FOR_LIT9 (INTVAL (XEXP (x, 1)))))
    {
        HOST_WIDE_INT offset_from_fp = INTVAL (XEXP (x, 1));
        HOST_WIDE_INT frame_size = get_frame_size();
        HOST_WIDE_INT offset_from_sp = offset_from_fp + frame_size;
        HOST_WIDE_INT mode_adjusted_sp_offset = offset_from_sp / GET_MODE_SIZE (mode);
        if (CONST_OK_FOR_MEMLI (mode_adjusted_sp_offset))
        {
            rtx reg_SP = gen_rtx_REG (SImode, STACK_POINTER_REGNUM);
            rtx reg_offset_sp = GEN_INT (offset_from_sp);
            if (0 == mode_adjusted_sp_offset)
                new_x = reg_SP;
            else
                new_x = gen_rtx_PLUS (GET_MODE (x), reg_SP, reg_offset_sp);
        }
    }
    return new_x;
}

/* Legitimize reload addresses. Used during reload pass.  */
rtx
quatro_legitimize_reload_address (x, mode, opnum, type, ind_levels)
    rtx x;
    enum machine_mode mode ATTRIBUTE_UNUSED;
    int opnum ATTRIBUTE_UNUSED;
    int type ATTRIBUTE_UNUSED;
    int ind_levels ATTRIBUTE_UNUSED;
{
    rtx new_x = NULL_RTX;
    if (   GET_CODE (x) == PLUS
        && GET_CODE (XEXP (x, 0)) == REG
        && REGNO (XEXP (x, 0)) == FRAME_POINTER_REGNUM
        && GET_CODE (XEXP (x, 1)) == CONST_INT)
    {
        /* offset from frame pointer */
        HOST_WIDE_INT offset_from_fp = INTVAL (XEXP (x, 1));
        /* mode independent offset from frame pointer */
        HOST_WIDE_INT mode_independent_fp_offset = offset_from_fp / GET_MODE_SIZE (mode);
        /* frame size */
        HOST_WIDE_INT frame_size = get_frame_size();
        /* offset from stack pointer */
        HOST_WIDE_INT offset_from_sp = offset_from_fp + frame_size;
        /* mode independent offset from stack pointer */
        HOST_WIDE_INT mode_adjusted_sp_offset = offset_from_sp / GET_MODE_SIZE (mode);
        /* frame pointer register rtx */
        rtx reg_FP = gen_rtx_REG (SImode, FRAME_POINTER_REGNUM);
        /* stack pointer register rtx */
        rtx reg_SP = gen_rtx_REG (SImode, STACK_POINTER_REGNUM);

	if (CONST_OK_FOR_MEMLI (mode_adjusted_sp_offset))
        {
            rtx reg_offset_sp = GEN_INT (offset_from_sp);
            if (0 == mode_adjusted_sp_offset)
                new_x = reg_SP;
            else
                new_x = gen_rtx_PLUS (GET_MODE (x), reg_SP, reg_offset_sp);
        }
        else if (CONST_OK_FOR_LIT16 (mode_independent_fp_offset))
        {
            new_x = gen_rtx_PLUS (GET_MODE (x), reg_FP, GEN_INT (mode_independent_fp_offset));
            push_reload (XEXP (new_x, 1), NULL_RTX, &XEXP (new_x, 1), NULL,
                INDEX_REG_CLASS, SImode, VOIDmode, 0, 0, opnum, type);
        }
        else if (CONST_OK_FOR_LIT16 (mode_adjusted_sp_offset))
        {
            new_x = gen_rtx_PLUS (GET_MODE (x), reg_SP, GEN_INT (mode_adjusted_sp_offset));
            push_reload (XEXP (new_x, 1), NULL_RTX, &XEXP (new_x, 1), NULL,
                INDEX_REG_CLASS, SImode, VOIDmode, 0, 0, opnum, type);
        }
    }
    return new_x;
}

/* Predicate returns true if the constant is not a valid signed 9 bit literal  */  
int
quatro_non_lit9_operand (op, mode)
    rtx op;
    enum machine_mode mode ATTRIBUTE_UNUSED;
{
    return (GET_CODE (op) == CONST_INT && !(CONST_OK_FOR_LIT9 (INTVAL (op))));
}

/* Predicate returns true if the constant is not a valid signed 16 bit literal  */
int
quatro_non_lit16_operand (op, mode)
    rtx op;
    enum machine_mode mode ATTRIBUTE_UNUSED;
{
    return (GET_CODE (op) == CONST_INT && !(CONST_OK_FOR_LIT16 (INTVAL (op))));
}

/* Function to print the jump table elements  */
void
quatro_output_addr_vec_elt (file, value)
    FILE *file;
    int value;
{
    fprintf (file, "\t.long\t_L%d\n", value);
}

#define SYNTAX_ERROR(msgid) do {					\
  warning (msgid);					\
  warning ("ignoring malformed #pragma subReg");	\
  return;						\
} while (0)

void
quatro_pr_reg (pfile)
     cpp_reader *pfile ATTRIBUTE_UNUSED;
{
  tree x, num;
  enum cpp_ttype type;
  int maxRegisterUsage = 0;
  int index = 0;

  /* If we get here, generic code has already scanned the pragma directive and the word subReg*/
     
  if (c_lex (&num) != CPP_NUMBER)
    SYNTAX_ERROR ("missing number");

  if (TREE_CODE (num) == INTEGER_CST)
  {
	  maxRegisterUsage = TREE_INT_CST_LOW (num);	
	  
	  for (index = maxRegisterUsage+1; index <=29 ; index++)
	  {
			call_used_regs[index] = 0;
			call_really_used_regs [index] = 0;
			CLEAR_HARD_REG_BIT (call_used_reg_set, index);
			CLEAR_HARD_REG_BIT(regs_invalidated_by_call,index);
	  }
  }
  else
  {
	  warning ("not a valid number at end of #pragma subReg");
  }
  
  if (c_lex (&x) != CPP_EOF)
    warning ("junk at end of #pragma subReg");
 
}
