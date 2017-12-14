# asfpga
asfpga version 1.00e
====================
asfpga is an assembler written for use in FPGA design. It can be easily modified
for your instruction set. The ultimate goal of this software is to allow
a FPGA designer to easily write assembly code for a custom instruction set.

The current version allows to create a listing file, a memory dump file which can be used in debugging HDL code using $readmemh() or equivalent routine, and a binary file which can be used to program a EPROM.

Copyright (C) Hari Krishnan 2012-2018, hkrishn3 @T jhu d.ot edu
This program is free software; you can redistribute it and/or modify it under     the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA


features:
--------
* produces listing, memory dump and EPROM files
* EQU and ORG labels supported
* purely ANSI C compliant

usage:
------
asfpga filename.asm

* comments should be preceeded by a ';'
* all labels should be preceeded by a '#'
* empty lines in the source code are not allowed => should 
  put ';' to indicate a carriage return
* instructions such as 'LDI M, LED2', should have a space after comma
* all characters *must* be upper case => turn CAPS on while coding :-)

input:
------
filename.asm

outputs:
--------
filename.lst -> listing file
filename.mem -> memory dump file
filename.bin -> eprom file

changelog: 
----------                      
Jan 21st:
initial public release v 1.00e
Nov 24th: 
added label for listing file
Nov 23rd:
added 'EQU' feature
Nov 22nd:
added 'ORG' feature
Nov 21st:
initial beta release

todo:
-----                         
remove need for inserting ';' for blank line,
add comments, and indent source code,
test features,
add symbol table

Nov 21st:
add ORG & EQU feature
