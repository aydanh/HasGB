#include "timer.h"

TIMER::TIMER() {}
TIMER::~TIMER() {}

void TIMER::reset()
{
	t_cycles = 0;
	r_clock = 0;
	m_clock = 0;
	d_clock = 0;

	divTimer = 0;
	timer = 0;
	timerModulo = 0;
	timerStart = 0;
	timerFreq = 0;

}

void TIMER::tick(int cycle, int mode)
{
	if(timerStart) {
		r_clock += cycle*mode;
		while(r_clock >= 16)
		{
			m_clock++;
			r_clock -= 16;
	
			switch(timerFreq) {
				case 0:
				{
					if(m_clock >= 64)//1000
					{
						timer++;
						m_clock = 0;
						if(timer == 0) {
							timer = timerModulo;
							(*intrFlag) |= 0x04;
						}
					}
				}
				break;
				case 1:
					if(m_clock >= 1) //15.6
					{
						timer++;
						m_clock = 0;
						if(timer == 0) {
							timer = timerModulo;
							(*intrFlag) |= 0x04;
						}
					}
				break;
				case 2:
					if(m_clock >= 4) //62.4
					{
						timer++;
						m_clock = 0;
						if(timer == 0) {
							timer = timerModulo;
							(*intrFlag) |= 0x04;
						}
					}
				break;
				case 3:
					if(m_clock >= 16) //256
					{
						timer++;
						m_clock = 0;
						if(timer == 0) {
							timer = timerModulo;
							(*intrFlag) |= 0x04;
						}
					}
				break;
			}
		}
	}
	d_clock += cycle*mode;
	if(d_clock >= 256) { divTimer++; d_clock -= 256; }
}

u8 TIMER::read(u16 addr)
{
	switch(addr)
	{
		case DIV:
			return divTimer;
		break;

		case TIMA:
			return timer;
		break;

		case TMA:
			return timerModulo;
		break;

		case TAC:
		{
			u8 n;
			n = (timerStart)?0x04:0;
			n += timerFreq;
			n |= 0xF8;
			return n;
		}
		break;
	}
}

void TIMER::write(u16 addr, u8 val)
{
	switch(addr)
	{
		case DIV:
			divTimer = 0;
		break;

		case TIMA:
			timer = val;
		break;

		case TMA:
			timerModulo = val;
		break;

		case TAC:
		{
			timerStart = (val&0x04)?1:0;
			timerFreq = val&0x3;
		}
		break;
	}
}