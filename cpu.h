#ifndef _CPU_H
#define _CPU_H

#include <stdio.h>
#include "table.h"
#include "enum.h"
#include "mmu.h"

#ifndef BYTE_TYPE_DEFS
#define BYTE_TYPE_DEFS
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
#endif

typedef struct { u8 l,h; } u8pair;	//l-h order change for high endian PC						
typedef union { u8pair hl; u16 w; } reg;
typedef struct { reg rAF; reg rBC; reg rDE; reg rHL; reg rPC; reg rSP; } regSet;

class CPU
{
private:
	regSet r;
	
	bool IME;
	bool halt;
	u8 haltFlag;
	
	int opCycle;
	int* gbMode;
	int speedMode;

	MMU* mmu;

public:
	CPU();
	~CPU();

	void reset();
	int step();
	void stepCB();
	int checkIntr();

	void setMem(MMU* n_mmu);

	void printState();
	u16 getNextInstAddr();
	u16 getPC();
	int getSpeedMode();
	//int getHalt();
};

#endif