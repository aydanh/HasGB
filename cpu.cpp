#include "cpu.h"

#define A r.rAF.hl.h
#define F r.rAF.hl.l
#define B r.rBC.hl.h
#define C r.rBC.hl.l
#define D r.rDE.hl.h
#define E r.rDE.hl.l
#define H r.rHL.hl.h
#define L r.rHL.hl.l
#define AF r.rAF.w
#define BC r.rBC.w
#define DE r.rDE.w
#define HL r.rHL.w
#define PC r.rPC.w
#define SP r.rSP.w

CPU::CPU() {}
CPU::~CPU() {}

void CPU::reset() 
{
	PC = 0x100;
	SP = 0xFFFE;
	AF = 0x11B0;
	BC = 0x0013;
	DE = 0x00D8;
	HL = 0x014D;

	IME = true;
	halt = false;
	haltFlag = 0;
	
	opCycle = 0;
	gbMode = &mmu->gbMode;
	speedMode = 1;
}

int CPU::step() 
{
		if(IME) { if(checkIntr()){ return 16; } }
		else if(halt) { if((haltFlag != (*mmu->intrFlag)) && ((*mmu->intrFlag)&(*mmu->intrMask)&0x1F)) { halt = false; haltFlag = 0; PC++;} }
	
		u8 opcode = mmu->read(PC++);
		opCycle = opCyclesData[opcode];
	
		switch(opcode)
		{
#include "ops.h"
			case CB: { stepCB(); } break;
			default: { printf("Unknown opcode %#4x encountered at %#4x\n", opcode, (PC)-1); }
		}
		
		return opCycle/speedMode;

}

void CPU::stepCB()
{
	u8 opcode = mmu->read(PC++);
	opCycle = opCyclesCBData[opcode];
	
	switch(opcode)
	{
#include "opsCB.h"
		default: { printf("Unknown opcode %d encountered at %d\n", opcode, (PC)-1); }
	}
}


int CPU::checkIntr()
{
	     if(((*mmu->intrFlag)&0x01) && ((*mmu->intrMask)&0x01)) { (*mmu->intrFlag) = ((*mmu->intrFlag)&0xFE);	IME = false;	if(halt) { PC++; halt = false;}	SP -= 2;	mmu->writeB(SP, PC);		PC = 0x0040;	return 1; }
	else if(((*mmu->intrFlag)&0x02) && ((*mmu->intrMask)&0x02)) { (*mmu->intrFlag) = ((*mmu->intrFlag)&0xFD);	IME = false;	if(halt) { PC++; halt = false;}	SP -= 2;	mmu->writeB(SP, PC);		PC = 0x0048; 	return 1; }
	else if(((*mmu->intrFlag)&0x04) && ((*mmu->intrMask)&0x04)) { (*mmu->intrFlag) = ((*mmu->intrFlag)&0xFB);	IME = false;	if(halt) { PC++; halt = false;}	SP -= 2;	mmu->writeB(SP, PC);		PC = 0x0050; 	return 1; }
	//else if(((*mmu->intrFlag)&0x08) && ((*mmu->intrMask)&0x08)) { (*mmu->intrFlag) = ((*mmu->intrFlag)&0xF7);	IME = false;	if(halt) { PC++; halt = false;}	SP -= 2;	mmu->writeB(SP, PC);		PC = 0x0058; 	return 1; }
	else if(((*mmu->intrFlag)&0x10) && ((*mmu->intrMask)&0x10)) { (*mmu->intrFlag) = ((*mmu->intrFlag)&0xEF);	IME = false;	if(halt) { PC++; halt = false;}	SP -= 2;	mmu->writeB(SP, PC);		PC = 0x0060; 	return 1; }
}

void CPU::setMem(MMU* n_mmu)
{
	mmu = n_mmu;
}


void CPU::printState()
{
	printf("Next OP %#4x\n", mmu->read(PC));
	printf("PC %#4x\n", PC);
	printf("A %#4x\n", A);
	printf("F %#4x\n", F);
	printf("B %#4x\n", B);
	printf("C %#4x\n", C);
	printf("D %#4x\n", D);
	printf("E %#4x\n", E);
	printf("H %#4x\n", H);
	printf("L %#4x\n", L);
	printf("SP %#4x\n", SP);
	printf("IF %#4x\n", (*mmu->intrFlag));
	printf("IE %#4x\n", (*mmu->intrMask));
	printf("IME %#4x\n",IME);
}
u16 CPU::getNextInstAddr()
{
	return (PC + opBytesData[mmu->read(PC)]);
}
u16 CPU::getPC()
{
	return PC;
}
int CPU::getSpeedMode()
{
	return speedMode;
}
/*
int CPU::getHalt()
{
	if(IME) { return (!halt || ((*mmu->intrFlag)&(*mmu->intrMask)&0x1F)); }
	else{ return (!halt || (*mmu->intrFlag) != haltFlag); }

}*/