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

#ifndef _opcode_h
#define _opcode_h

#define LKUP_TBL1 5
#define LKUP_TBL2 7 
#define LKUP_TBL3 9
#define LKUP_TBL4 7
#define LKUP_TBL5 10
#define LKUP_TBL6 1

struct op {
  char mnemonic [5];
  char uniqChar [9];
  unsigned char opcode;
};

struct direct {
  char mnemonic [5];
  unsigned char opcode;
};
                   
/*for instructions of type <instruction, data>*/

struct op lkupTbl1[] = {
  "LDI",  "A,",   0x00,  /* LDI A,data */
  "LDI",  "B,",   0x01,  /* LDI B,data */
  "LDI",  "X,",   0x02,  /* LDI X,data */
  "LD",   "A,",   0x10,  /* LD A, addr */
  "LDI",  "M,",   0x03   /* LDI M, data */		
};

/*for instructions of type <instruction> (no data)*/
struct op lkupTbl2[] = {
  "STR",  "[X],",   0x21,  /* STR [X],A */
  "STRL", "[M,X],", 0x22,  /* STRL [M,X],A */  
  "JMP",  "X",      0x66,  /* JMP X */
  "CLR",  "X",      0x70,  /* CLR X */
  "CLR",  "B",      0x71,  /* CLR B */
  "CLR",  "A",      0x72,  /* CLR A */
  "CLR",  "M",      0x73   /* CLR M */	
};

/*for instructions of type <instruction> (no data)*/
struct op lkupTbl3[] = {
	"LD",   "[X]",   0x11,  /* LD A, [X] */
	"LDL",  "[M,X]", 0x12,  /* LDL A,[M,X] */
	"XCHG", "B",     0x30,  /* XCHG A,B */
	"XCHG", "M",     0x31,  /* XCHG A,M */
	"XCHG", "X",     0x32,  /* XCHG B,X */
	"ADC",  "B",     0x42,  /* ADC A,B */
	"ADC",  "A",     0x43,  /* ADC A,A */
	"CPY",  "B",     0x33,  /* CPY B,X */
	"CPY",  "X",     0x34   /* CPY X,B */
};

/*for instructions of type <instruction, address>*/
struct direct lkupTbl4[] = {
	"BRA",  0x60,  /* BRA addr */
	"BC",   0x61,  /* BC addr */
	"BNC",  0x62,  /* BNC addr */
	"BZ",   0x63,  /* BZ addr */
	"BNZ",  0x64,  /* BNZ addr */
	"JMP",  0x65,  /* JMP addr */
	"JSR",  0x66   /* JSR addr */
};

/*for instructions of type <instruction> (no data)*/
struct direct lkupTbl5[] = {
	"DEC",  0x40,  /* DEC B */
	"INC",  0x41,  /* INC X */ 
	"SBC",  0x44,  /* SBC A,B */
	"CMP",  0x45,  /* CMP A,B */
	"AND",  0x50,  /* AND A,B */
	"OR",   0x51,  /* OR A,B */
	"XOR",  0x52,  /* XOR A,B */
	"NOP",  0x74,  /* NOP */
	"RET",  0x75,  /* RET */	
	"RTS",  0x77   /* RTS */
};

/*for instructions of type <address, register>*/
struct direct lkupTbl6[] = {
	"STR",  0x20  /* STR addr,A */
};             
#endif        