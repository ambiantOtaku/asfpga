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
#include "asfpga.h"
#include "opcode.h"
#include "error.h"

void parseFile(char * argv) {
  
  FILE * binFile;
  FILE * inputFile;
  FILE * listFile;
  FILE * readMemFile;
  
  char * opcode;
  char insBuf[81];
  char operand1[81];
  char operand2[81];
  char buf[81];
  
  char inFile[81];
  char lstFile[81];
  char bnFile[81];
  char fileName[81];  
  char readMemF[81];

  unsigned int addr;
  
  int lineNo;
  int skipFlag;
  int orgFlag;
  int i;
  
  strcpy(inFile, argv);
  sscanf(inFile, "%[^.]", fileName);
  strcpy(lstFile, fileName);
  strcpy(bnFile, fileName);
  strcpy(readMemF, fileName);
  strcat(lstFile, ".lst");
  strcat(bnFile, ".bin");
  strcat(readMemF, ".mem");
  
  if((inputFile = fopen(argv, "r")) == NULL) 
  	fatalError("Unable to read input file");
  
  if((listFile = fopen(lstFile, "w")) == NULL) 
  	fatalError("Unable to create .lst file");
  
  if((binFile = fopen(bnFile, "wb")) == NULL) 
  	fatalError("Unable to create .bin file");

  if((readMemFile = fopen(readMemF, "w")) == NULL) 
  	fatalError("Unable to create .mem file");
  
  	
  lineNo = 1;
  lblCount = 0;
  printLabel(listFile);
  getLabels(inputFile);
 
  if((inputFile = fopen(argv, "r")) == NULL) 
  	fatalError("Unable to read input file");
  
  while((fgets(buf, 80, inputFile)) != NULL) { 
  	  
    skipFlag = 0;
    orgFlag = 0;
    strcpy(operand1, "00");
    strcpy(operand2, "00");
  
    sscanf(buf, "%s%s%s", insBuf, operand1, operand2);             
    orgFlag = strcmp(insBuf, "ORG");
    
    if(buf[0] == ';' || orgFlag == 0) {
     	if(orgFlag == 0) {
	  addr = atoh(operand1);	
 	  fprintf(listFile, "%.3d\t%x\t\t%s", lineNo, addr, buf);
 	  skipFlag = 1;
 		}
 	
 		else {
    	fprintf(listFile, "%.3d\t%s", lineNo, buf);
    	skipFlag = 1;
   	}
  }
  if(buf[0] == '#') {
	fprintf(listFile, "%.3d\t%s", lineNo, buf);
	skipFlag = 1;
 }
 

  if((strcmp(operand1, "EQU") == 0)) {
   	strcpy(labelDef[lblCount].labelName, insBuf);
   	labelDef[lblCount].addr = atoh(operand2);
   	fprintf(listFile, "%.3d\t\t%s", lineNo, buf);
   	skipFlag = 1;
   	lblCount++;
  }
    
   
  if(skipFlag == 0) {
   	opcode = getOp(insBuf, operand1, operand2, addr);
		writeMemFile(readMemFile, opcode, addr);
    	if((strcmp(opcode, "invalid") == 0)) {
    		printf("Invalid instruction found on %d\n", lineNo);
    		exit(1);
    	}
    	fprintf(listFile, "%.3d\t%.3x\t%s\t%s", lineNo, addr, opcode, buf);
    	addr = addr + 2;
  }
  lineNo++;
 }
  
  fwrite(binData, sizeof(binData), 1, binFile);
  fprintf(listFile, "%c", '\n');
  for(i = 0; i < lblCount; i++) 
  	fprintf(listFile, "%s%c[%.3x]\n", labelDef[i].labelName, '=', labelDef[i].addr);  
  fclose(binFile);
  fclose(listFile);
  
}

char * getOp(char insBuf[], char operand1[], char operand2[], unsigned int addr) {
  
  int i;
  int j;
  char * insCode;
  
  insCode = (char *) malloc(81 * sizeof(char));
  
  for(j = 0; j < 50; j++) {	
   	if((strcmp(operand1, labelDef[j].labelName) == 0)) {
   		sprintf(operand1, "%.2x", labelDef[j].addr);
   		//printf("%.2x\n",labelDef[j].addr);
	}

   	if((strcmp(operand2, labelDef[j].labelName) == 0)) 
   		sprintf(operand2, "%.2x", labelDef[j].addr); 
 }
 
  for(i = 0; i < LKUP_TBL1; i++) {
   if((strncmp(insBuf, lkupTbl1[i].mnemonic, 5) == 0) && 
   		(strncmp(operand1, lkupTbl1[i].uniqChar, 5) == 0)) {
   	if((strcmp(insBuf, "LD") == 0) && (strcmp(operand2, "[X]") == 0))
   				continue;
   	binData[addr-0x800] = lkupTbl1[i].opcode;
   	sprintf(insCode, "%.2x", lkupTbl1[i].opcode);
   	//printf("%x%s%s\n", addr, insCode, operand2);
   	binData[(addr-0x800) + 1] = atoh(operand2);
   	//printf("%.2x\n", atoh(operand2));
   	strcat(insCode, operand2);
  	return insCode;
		}
	} 		
  
  for(i = 0; i < LKUP_TBL2; i++) {
   if((strncmp(insBuf, lkupTbl2[i].mnemonic, 5) == 0) && 
   		(strncmp(operand1, lkupTbl2[i].uniqChar, 5) == 0)) {
   	binData[addr-0x800] = lkupTbl2[i].opcode;
   	sprintf(insCode, "%.2x", lkupTbl2[i].opcode);
   	//printf("%x%s%s\n", addr, insCode, "00");
  	binData[(addr-0x800) + 1] = atoh("00");
   	strcat(insCode, "00");
   	return insCode;
		}
	}
	
	for(i = 0; i < LKUP_TBL3; i++) {
   if((strncmp(insBuf, lkupTbl3[i].mnemonic, 5) == 0) && 
   		(strncmp(operand2, lkupTbl3[i].uniqChar, 5) == 0)) {
   	binData[addr-0x800] = lkupTbl3[i].opcode;
   	sprintf(insCode, "%.2x", lkupTbl3[i].opcode);
   	//printf("%x%s%s\n", addr, insCode, "00");
   	binData[(addr-0x800) + 1] = atoh("00");
   	strcat(insCode, "00");
   	return insCode;
		}
	}
  
  for(i = 0; i < LKUP_TBL4; i++) {		
   if((strncmp(insBuf, lkupTbl4[i].mnemonic, 5) == 0)) {
   	binData[addr-0x800] = lkupTbl4[i].opcode;
   	sprintf(insCode, "%.2x", lkupTbl4[i].opcode);
   	//printf("%x%s%s\n", addr, insCode, operand1);
   	binData[(addr-0x800) + 1] = atoh(operand1);
   	strcat(insCode, operand1);
   	return insCode;
		}
	}
  
  for(i = 0; i < LKUP_TBL5; i++) {
   if((strncmp(insBuf, lkupTbl5[i].mnemonic, 5) == 0)) {
   	binData[addr-0x800] = lkupTbl5[i].opcode;
   	sprintf(insCode, "%.2x", lkupTbl5[i].opcode);
   	//printf("%x%s%s\n", addr, insCode, "00");
   	binData[(addr-0x800) + 1] = atoh("00");
   	strcat(insCode, "00");
   	return insCode;
		}
	}
  
  for(i = 0; i < LKUP_TBL6; i++) {
   if((strncmp(insBuf, lkupTbl6[i].mnemonic, 5) == 0)) {
   	binData[addr-0x800] = lkupTbl6[i].opcode;
   	operand1[2] = '\0';
   	sprintf(insCode, "%.2x", lkupTbl6[i].opcode);
   	//printf("%x%s%s\n", addr, insCode, operand1);
   	binData[(addr-0x800) + 1] = atoh(operand1);
   	strcat(insCode, operand1);
   	return insCode;
		}
	}
	return "invalid";
}
void getLabels(FILE * inputFile) {
	
	char buf[81];
	char opcode[81];
	char instr[81];
	unsigned int currAddr;
	int lineNo;
	int skipFlag;
	unsigned int base;
	
	lineNo = 0;
	currAddr = 0;
	skipFlag = 0;	

	while((fgets(buf, 80, inputFile)) != NULL) {
		skipFlag = 0;
		sscanf(buf, "%s%s", opcode, instr);
		if((strcmp(opcode, "ORG")) == 0) {
			currAddr = atoh(instr);
			base = currAddr;
			skipFlag = 1;
			lineNo = 0;
		}
		if((strcmp(instr, "EQU")) == 0) {
			skipFlag = 1;
		}
		if(buf[0] == ';') {
			skipFlag = 1;
		}
		if(buf[0] == '#') {
			sscanf(buf, "%s", labelDef[lblCount].labelName);
			if(lineNo > 0)
				currAddr = base + (lineNo * 2);
			labelDef[lblCount].addr = (currAddr & 0xFF);
			skipFlag = 1;
			lblCount = lblCount + 1;

		}
		if(skipFlag != 1) {
		  lineNo++;	
	
		}
	}	
}

int atoh(char *pcHexStr)
{
    int lNumber;
    int iStrLen, i, j;
    char *validHex="0123456789ABCDEF";
    char *pc;

    iStrLen=strlen(pcHexStr);

    if (iStrLen==0)
        return 0L;

    pc=pcHexStr;

    lNumber=0L;
    for (i=iStrLen-1; i >= 0; i --)
    {
        for (j=0; j < strlen(validHex) ; j++)
        {
            if (validHex[j]==toupper(pc[i]))
            {

                lNumber += j*power(16, (iStrLen-i-1));
                break;
            }
        }
        if (j >= strlen(validHex))
            return 0L;
    }
    return(lNumber);
    
} 

int power(int base, int n)
{
    int i;
    long  p;

    p=1;
    for(i=1; i<=n; i++)
        p = p * base;
    return p;
} 

 void printLabel(FILE * listFile) {
 	int i;
	time_t timeType;
	struct tm * currTime;

	time(&timeType);
	currTime = localtime(&timeType);

 	for(i = 0; i < 81; i++) fprintf(listFile,"%.80c", '%');
 	fprintf(listFile,"\n%s\n", "%AS8636 v. 1.00a");
 	fprintf(listFile,"%s\n", "%Listing File");
 	fprintf(listFile,"%c%s", '%', asctime(currTime));
 	for(i = 0; i < 81; i++) fprintf(listFile,"%.80c", '%');
 	fprintf(listFile, "%c", '\n');
 	
} 	

void writeMemFile(FILE * readMemFile, char * opcode, unsigned int addr){
	
	char op[3];
	char instr[3];

	op[0] = opcode[0];
	op[1] = opcode[1];
	op[2] = '\0';
	
	instr[0] = opcode[2];
	instr[1] = opcode [3]; 
	instr[2] = '\0';

   	 fprintf(readMemFile, "@%x\t%s\n", (addr), op);
	 fprintf(readMemFile, "@%x\t%s\n", (addr+1), instr);
//begin dram mem write
//   	 fprintf(readMemFile, "@%x\t%s\n", (addr) - 0x800, op);
//	 fprintf(readMemFile, "@%x\t%s\n", (addr+1) -0x800, instr);

	 return;
        
}
