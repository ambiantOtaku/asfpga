/*
asfpga version 1.00e
====================
asfpga is an assembler written for use in FPGA design. It can be easily modified
for your instruction set. The ultimate goal of this software is to allow
a FPGA designer to easily write assembly code for a custom instruction set.

The current version allows to create a listing file, a memory dump file which can be used in debugging HDL code using $readmemh() or equivalent routine, and a binary file which can be used to program a EPROM.

Copyright (C) Hari Krishnan 2002-2003, hkrishna@bayou.uh.edu
This program is free software; you can redistribute it and/or modify it under     the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*/
#include "error.h"

void usageError(char message[]) {
        printf("asfpga version 1.00e Copyright (C) 2003 Hari Krishnan\n"); 
        printf("asfpga comes with ABSOLUTELY NO WARRANTY; for details type\n");
        printf("For details read asfpga.txt\n");
        printf("This is free software, and you are welcome to redistribute\n");                    
        printf("it under certain conditions\n\n");
	printf("%s\n", message);
	printf("asfpga v. 1.00e\n");
	printf("--------------\n");
	printf("Usage: asfpga inputfile\n");
	exit(1);
}

void fatalError(char message[]) {
        printf("asfpga version 1.00e Copyright (C) 2003 Hari Krishnan\n"); 
        printf("asfpga comes with ABSOLUTELY NO WARRANTY; for details type\n");
        printf("For details read asfpga.txt\n");
        printf("This is free software, and you are welcome to redistribute\n");                    
        printf("it under certain conditions\n\n");
	printf("%s\n", message);
	exit(1);
}