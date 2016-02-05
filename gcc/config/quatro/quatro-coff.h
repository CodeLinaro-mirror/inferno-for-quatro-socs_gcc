/*

Copyright (c) 2016, The Linux Foundation. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License version 2 and
only version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#ifndef __QUATRO_COFF__H
#define __QUATRO_COFF__H

#define TARGET_VERSION fputs (" (ZORAN quatro-coff)", stderr)

/* Define if the object format being used is COFF or a superset.  */
#define OBJECT_FORMAT_COFF
 
/*RVD wants the SDB format*/
#define SDB_DEBUGGING_INFO

/*Keep DBX around for when using GDB*/
#define DBX_DEBUGGING_INFO

#undef PREFERRED_DEBUGGING_TYPE
#define PREFERRED_DEBUGGING_TYPE SDB_DEBUG

#endif /* __QUATRO_COFF__H */


