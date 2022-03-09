#ifndef _TIMER_H
#define _TIMER_H

#include "enum.h"

#ifndef BYTE_TYPE_DEFS
#define BYTE_TYPE_DEFS
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
#endif

class TIMER
{
private:
	int t_cycles;
	int r_clock;
	int m_clock;
	int d_clock;

	u8 divTimer;
	u8 timer;
	u8 timerModulo;
	int timerStart;
	int timerFreq;

public:
	u8* intrFlag;

public:
	TIMER();
	~TIMER();

	void reset();
	void tick(int cycle, int mode);

	u8 read(u16 addr);
	void write(u16 addr, u8 val);

};

#endif