/* Assembly functions for libgcc2.
   Copyright (C) 2003 Free Software Foundation, Inc.
   Contributed by Zoran Corporation.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file into combinations with other programs,
and to distribute those combinations without any restriction coming
from the use of this file.  (The General Public License restrictions
do apply in other respects; for example, they cover modification of
the file, and distribution when not linked into a combine
executable.)

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.

Copyright (c) 2016, The Linux Foundation. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 and
only version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#ifdef  L_modsi3
    .file "libgcc_modsi3.S"
    .section .text
    .align 32    #ASM_OUTPUT_ALIGN
    .global __modsi3
     # R1.a = R1.a % R2.a
__modsi3:
    # If we're dividing by zero, call the error entry
    # in the DSP interrupt table (0x20)
    IF(R2.a != 0) GOTO _modstart;
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    CALL (0x20);
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
_modstart:
    # Get sign of result into R5 and force operands (R1 & R2) to be positive
    # (Sign of result is the sign of the numerator)
    LA = -1;
    R3 = SELECT(R2 < 0, LA, 1);
    R5 = SELECT(R1 < 0, LA, 1);
    R2 = R2 * R3;
    R1 = R1 * R5;
    # Calculate number of bits to shift denominator into R7
    R6 = CNTLZ(R2);
    R3 = 32;
    LA = CNTLZ(R1);
    R4 = 0;
    R7 = R6 - LA;
    # Shift left the denominator the appropriate number of places
    LA = R3 + LA;
    R8 = R6 * -1 + LA;
    SPECREG(EXT_NXT) = R2;
    R2 = EXTR(R2, R8) + 0;
    SPECREG(EXT_NXT) = 0;
    IF(R7.a < 0) GOTO _moddone;
_modloop:
    LA = 1;
    R3 = R4 * 2 + LA;
    R4 = R4 * 2;
    R6 = R1 - R2;    # Can't subtract straight into LA - need to
    LA = R6;         # truncate 48-bit LA to 32 bits
    IF(R7.a > 0, R7.a+=-1) GOTO _modloop;
    R2 = EXTL(R2, 1, 32);          # Execute in branch delay
    R4 = SELECT(LA < 0, R4, R3);   # Execute in branch delay
    R1 = SELECT(LA >= 0, LA, R1);  # Execute in branch delay
_moddone:
    RETURN;
    R1 = R1 * R5;                  # Execute in branch delay
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
#endif /* L_modsi3 */

#ifdef  L_divsi3
    .file "libgcc_divsi3.S"
    .section .text
    .align 32    #ASM_OUTPUT_ALIGN
    .global __divsi3
     # R1.a = R1.a / R2.a
__divsi3:
    # If we're dividing by zero, call the error entry
    # in the DSP interrupt table (0x20)
    IF(R2.a != 0) GOTO _divstart;
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    CALL (0x20);
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
_divstart:
    # Get sign of result into R5 and force operands (R1 & R2) to be positive
    LA = -1;
    R3 = SELECT(R2 < 0, LA, 1);
    R4 = SELECT(R1 < 0, LA, 1);
    R2 = R2 * R3;
    R1 = R1 * R4;
    R5 = R3 * R4;
    # Calculate number of bits to shift denominator into R7
    R6 = CNTLZ(R2);
    R3 = 32;
    LA = CNTLZ(R1);
    R4 = 0;
    R7 = R6 - LA;
    # Shift left the denominator the appropriate number of places
    LA = R3 + LA;
    R8 = R6 * -1 + LA;
    SPECREG(EXT_NXT) = R2;
    R2 = EXTR(R2, R8) + 0;
    SPECREG(EXT_NXT) = 0;
    IF(R7.a < 0) GOTO _divdone;
_divloop:
    LA = 1;
    R3 = R4 * 2 + LA;
    R4 = R4 * 2;
    R6 = R1 - R2;    # Can't subtract straight into LA - need to
    LA = R6;         # truncate 48-bit LA to 32 bits
    IF(R7.a > 0, R7.a+=-1) GOTO _divloop;
    R2 = EXTL(R2, 1, 32);          # Execute in branch delay
    R4 = SELECT(LA < 0, R4, R3);   # Execute in branch delay
    R1 = SELECT(LA >= 0, LA, R1);  # Execute in branch delay
_divdone:
    RETURN;
    R1 = R4 * R5;                  # Execute in branch delay
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
#endif /* L_divsi3 */

#ifdef  L_umodsi3
    .file "libgcc_umodsi3.S"
    .section .text
    .align 32    #ASM_OUTPUT_ALIGN
    .global __umodsi3
     # R1.a = R1.a % R2.a
__umodsi3:
    # If we're dividing by zero, call the error entry
    # in the DSP interrupt table (0x20)
    IF(R2.a != 0) GOTO _umodstart;
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    CALL (0x20);
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
_umodstart:
    # Calculate number of bits to shift denominator into R7
    R6 = CNTLZ(R2);
    R3 = 32;
    LA = CNTLZ(R1);
    R4 = 0;
    R7 = R6 - LA;
    # Shift left the denominator the appropriate number of places
    LA = R3 + LA;
    R8 = R6 * -1 + LA;
    SPECREG(EXT_NXT) = R2;
    R2 = EXTR(R2, R8) + 0;
    SPECREG(EXT_NXT) = 0;
    IF(R7.a < 0) GOTO _umoddone;
_umodloop:
    LA = 1;
    R3 = R4 * 2 + LA;
    R4 = R4 * 2;
    R6 = R1 - R2;    # Can't subtract straight into LA - need to
    LA = R6;         # truncate 48-bit LA to 32 bits
    IF(R7.a > 0, R7.a+=-1) GOTO _umodloop;
    R2 = EXTL(R2, 1, 32);          # Execute in branch delay
    R4 = SELECT(LA < 0, R4, R3);   # Execute in branch delay
    R1 = SELECT(LA >= 0, LA, R1);  # Execute in branch delay
_umoddone:
    RETURN;    # Remainder sitting in R1
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
#endif /* L_umodsi3 */

#ifdef  L_udivsi3
    .file "libgcc_udivsi3.S"
    .section .text
    .align 32    #ASM_OUTPUT_ALIGN
    .global __udivsi3
     # R1.a = R1.a / R2.a unsigned
__udivsi3:
    # If we're dividing by zero, call the error entry
    # in the DSP interrupt table (0x20)
    IF(R2.a != 0) GOTO _udivstart;
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    CALL (0x20);
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
_udivstart:
    # Calculate number of bits to shift denominator into R7
    R6 = CNTLZ(R2);
    R3 = 32;
    LA = CNTLZ(R1);
    R4 = 0;
    R7 = R6 - LA;
    # Shift left the denominator the appropriate number of places
    LA = R3 + LA;
    R8 = R6 * -1 + LA;
    SPECREG(EXT_NXT) = R2;
    R2 = EXTR(R2, R8) + 0;
    SPECREG(EXT_NXT) = 0;
    IF(R7.a < 0) GOTO _udivdone;
_udivloop:
    LA = 1;
    R3 = R4 * 2 + LA;
    R4 = R4 * 2;
    R6 = R1 - R2;    # Can't subtract straight into LA - need to
    LA = R6;         # truncate 48-bit LA to 32 bits
    IF(R7.a > 0, R7.a+=-1) GOTO _udivloop;
    R2 = EXTL(R2, 1, 32);          # Execute in branch delay
    R4 = SELECT(LA < 0, R4, R3);   # Execute in branch delay
    R1 = SELECT(LA >= 0, LA, R1);  # Execute in branch delay
_udivdone:
    RETURN;
    R1 = R4;                       # Execute in branch delay
    NOP;                           # Fill branch delay slot
    NOP;                           # Fill branch delay slot
#endif /* L_udivsi3 */
